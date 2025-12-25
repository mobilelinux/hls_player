#include "hls_player.h"
#include "tkc/log.h"
#include <SDL.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

struct _hls_player_t {
  char *url;
  player_state_t state;

  AVFormatContext *fmt_ctx;
  AVCodecContext *video_dec_ctx;
  AVCodecContext *audio_dec_ctx;
  int video_stream_idx;
  int audio_stream_idx;
  struct SwsContext *sws_ctx;
  struct SwrContext *swr_ctx;
  SDL_AudioDeviceID audio_dev;
  bool_t audio_initialized;
  int audio_channels;
  int audio_sample_rate;

  pthread_t thread;
  bool_t running;
  bool_t quit;

  hls_player_on_frame_t on_frame;
  void *on_frame_ctx;

  double position;
  double duration;
};

static void *player_thread(void *arg);

hls_player_t *hls_player_create(void) {
  hls_player_t *player = (hls_player_t *)calloc(1, sizeof(hls_player_t));
  return player;
}

ret_t hls_player_set_url(hls_player_t *player, const char *url) {
  return_value_if_fail(player != NULL, RET_BAD_PARAMS);
  if (player->url) {
    free(player->url);
  }
  log_debug("set url: %s\n", url);
  player->url = tk_strdup(url);
  return RET_OK;
}

ret_t hls_player_play(hls_player_t *player) {
  return_value_if_fail(player != NULL, RET_BAD_PARAMS);
  if (player->state == PLAYER_STATE_PLAYING) {
    return RET_OK;
  }

  if (player->state == PLAYER_STATE_STOPPED) {
    player->quit = FALSE;
    player->running = TRUE;
    pthread_create(&player->thread, NULL, player_thread, player);
  } else if (player->state == PLAYER_STATE_PAUSED) {
    if (player->audio_dev != 0) {
      SDL_PauseAudioDevice(player->audio_dev, 0);
    }
  }

  player->state = PLAYER_STATE_PLAYING;
  return RET_OK;
}

ret_t hls_player_pause(hls_player_t *player) {
  return_value_if_fail(player != NULL, RET_BAD_PARAMS);
  if (player->state == PLAYER_STATE_PLAYING) {
    player->state = PLAYER_STATE_PAUSED;
    if (player->audio_dev != 0) {
      SDL_PauseAudioDevice(player->audio_dev, 1);
    }
  }
  return RET_OK;
}

ret_t hls_player_stop(hls_player_t *player) {
  return_value_if_fail(player != NULL, RET_BAD_PARAMS);
  player->quit = TRUE;
  if (player->audio_dev != 0) {
    SDL_ClearQueuedAudio(player->audio_dev);
    SDL_PauseAudioDevice(player->audio_dev, 1);
  }
  if (player->running) {
    pthread_join(player->thread, NULL);
    player->running = FALSE;
  }
  player->state = PLAYER_STATE_STOPPED;
  return RET_OK;
}

ret_t hls_player_destroy(hls_player_t *player) {
  hls_player_stop(player);
  if (player->url)
    free(player->url);
  free(player);
  return RET_OK;
}

player_state_t hls_player_get_state(hls_player_t *player) {
  return player ? player->state : PLAYER_STATE_STOPPED;
}

double hls_player_get_position(hls_player_t *player) {
  return player ? player->position : 0;
}

double hls_player_get_duration(hls_player_t *player) {
  return player ? player->duration : 0;
}

void hls_player_set_on_frame(hls_player_t *player,
                             hls_player_on_frame_t on_frame, void *ctx) {
  if (player) {
    player->on_frame = on_frame;
    player->on_frame_ctx = ctx;
  }
}

static void *player_thread(void *arg) {
  hls_player_t *player = (hls_player_t *)arg;
  AVPacket *pkt = av_packet_alloc();
  AVFrame *frame = av_frame_alloc();
  AVFrame *frame_rgb = av_frame_alloc();
  AVFrame *audio_frame = av_frame_alloc();
  uint8_t *buffer = NULL;
  int ret;

  log_debug("play url: %s\n", player->url);

  // Open input
  if (avformat_open_input(&player->fmt_ctx, player->url, NULL, NULL) < 0) {
    log_error("Could not open source file %s\n", player->url);
    goto end;
  }

  if (avformat_find_stream_info(player->fmt_ctx, NULL) < 0) {
    log_error("Could not find stream information\n");
    goto end;
  }

  // Find streams
  player->video_stream_idx = -1;
  player->audio_stream_idx = -1;
  for (unsigned int i = 0; i < player->fmt_ctx->nb_streams; i++) {
    enum AVMediaType type = player->fmt_ctx->streams[i]->codecpar->codec_type;
    if (type == AVMEDIA_TYPE_VIDEO && player->video_stream_idx == -1) {
      player->video_stream_idx = i;
    } else if (type == AVMEDIA_TYPE_AUDIO && player->audio_stream_idx == -1) {
      player->audio_stream_idx = i;
    }
  }

  if (player->video_stream_idx == -1 && player->audio_stream_idx == -1) {
    log_error("Could not find any video or audio stream\n");
    goto end;
  }

  // Open video codec (only if video stream exists)
  int width = 0;
  int height = 0;
  if (player->video_stream_idx != -1) {
    AVCodecParameters *codecpar =
        player->fmt_ctx->streams[player->video_stream_idx]->codecpar;
    const AVCodec *codec = avcodec_find_decoder(codecpar->codec_id);
    player->video_dec_ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(player->video_dec_ctx, codecpar);
    avcodec_open2(player->video_dec_ctx, codec, NULL);

    // Prepare SWS context for RGB conversion
    width = codecpar->width;
    height = codecpar->height;
    player->sws_ctx =
        sws_getContext(width, height, player->video_dec_ctx->pix_fmt, width,
                       height, AV_PIX_FMT_RGBA, SWS_BILINEAR, NULL, NULL, NULL);

    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, width, height, 1);
    buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(frame_rgb->data, frame_rgb->linesize, buffer,
                         AV_PIX_FMT_RGBA, width, height, 1);
  }

  player->duration = (double)player->fmt_ctx->duration / AV_TIME_BASE;

  // Setup audio decoding if available
  if (player->audio_stream_idx != -1) {
    AVCodecParameters *audio_par =
        player->fmt_ctx->streams[player->audio_stream_idx]->codecpar;
    const AVCodec *audio_codec = avcodec_find_decoder(audio_par->codec_id);
    if (audio_codec != NULL) {
      player->audio_dec_ctx = avcodec_alloc_context3(audio_codec);
      if (player->audio_dec_ctx &&
          avcodec_parameters_to_context(player->audio_dec_ctx, audio_par) >=
              0 &&
          avcodec_open2(player->audio_dec_ctx, audio_codec, NULL) >= 0) {
        player->audio_sample_rate = player->audio_dec_ctx->sample_rate;

#if LIBAVCODEC_VERSION_MAJOR >= 59
        AVChannelLayout in_layout;
        AVChannelLayout out_layout;
        if (player->audio_dec_ctx->ch_layout.nb_channels == 0) {
          av_channel_layout_default(&player->audio_dec_ctx->ch_layout, 2);
        }
        av_channel_layout_copy(&in_layout, &player->audio_dec_ctx->ch_layout);
        av_channel_layout_copy(&out_layout, &player->audio_dec_ctx->ch_layout);
        player->audio_channels = in_layout.nb_channels;
        if (swr_alloc_set_opts2(&player->swr_ctx, &out_layout,
                                AV_SAMPLE_FMT_S16, player->audio_sample_rate,
                                &in_layout, player->audio_dec_ctx->sample_fmt,
                                player->audio_sample_rate, 0, NULL) < 0) {
          log_error("Failed to configure audio resampler\n");
          player->swr_ctx = NULL;
        } else if (swr_init(player->swr_ctx) < 0) {
          log_error("Failed to initialize audio resampler\n");
          swr_free(&player->swr_ctx);
        }
        av_channel_layout_uninit(&in_layout);
        av_channel_layout_uninit(&out_layout);
#else
        player->audio_channels = player->audio_dec_ctx->channels;
        int64_t in_layout = player->audio_dec_ctx->channel_layout;
        if (in_layout == 0 && player->audio_channels > 0) {
          in_layout = av_get_default_channel_layout(player->audio_channels);
        }
        int64_t out_layout = in_layout;
        player->swr_ctx = swr_alloc_set_opts(
            NULL, out_layout, AV_SAMPLE_FMT_S16, player->audio_sample_rate,
            in_layout, player->audio_dec_ctx->sample_fmt,
            player->audio_sample_rate, 0, NULL);
        if (player->swr_ctx && swr_init(player->swr_ctx) < 0) {
          log_error("Failed to initialize audio resampler\n");
          swr_free(&player->swr_ctx);
        }
#endif

        if (player->audio_channels <= 0) {
          player->audio_channels = 2;
        }

        if (player->swr_ctx) {
          if (!player->audio_initialized) {
            if (SDL_InitSubSystem(SDL_INIT_AUDIO) == 0) {
              player->audio_initialized = TRUE;
            } else {
              log_error("SDL_InitSubSystem audio failed: %s\n", SDL_GetError());
            }
          }

          if (player->audio_initialized) {
            SDL_AudioSpec want;
            SDL_zero(want);
            want.freq = player->audio_sample_rate;
            want.format = AUDIO_S16SYS;
            want.channels = (Uint8)player->audio_channels;
            want.samples = 1024;
            want.callback = NULL;

            player->audio_dev = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
            if (player->audio_dev == 0) {
              log_error("SDL_OpenAudioDevice failed: %s\n", SDL_GetError());
            } else {
              if (player->state == PLAYER_STATE_PAUSED) {
                SDL_PauseAudioDevice(player->audio_dev, 1);
              } else {
                SDL_PauseAudioDevice(player->audio_dev, 0);
              }
            }
          }
        }
      } else {
        log_error("Failed to open audio decoder\n");
      }
    }
  }

  while (!player->quit) {
    if (player->state == PLAYER_STATE_PAUSED) {
      sleep_ms(100);
      continue;
    }

    ret = av_read_frame(player->fmt_ctx, pkt);
    if (ret < 0)
      break; // End of stream or error

    if (pkt->stream_index == player->video_stream_idx) {
      ret = avcodec_send_packet(player->video_dec_ctx, pkt);
      if (ret < 0) {
        av_packet_unref(pkt);
        continue;
      }

      while (ret >= 0) {
        ret = avcodec_receive_frame(player->video_dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
          break;
        if (ret < 0)
          break;

        // Convert to RGB
        sws_scale(player->sws_ctx, (uint8_t const *const *)frame->data,
                  frame->linesize, 0, height, frame_rgb->data,
                  frame_rgb->linesize);

        // Notify callback
        if (player->on_frame) {
          player->on_frame(player->on_frame_ctx, frame_rgb->data[0], width,
                           height, AV_PIX_FMT_RGBA);
        }

        // Simple sync (very basic)
        double pts =
            frame->pts *
            av_q2d(
                player->fmt_ctx->streams[player->video_stream_idx]->time_base);
        player->position = pts;

        // Delay to match framerate (approximate)
        // Ideally use audio clock or system clock
        sleep_ms(30);

        av_frame_unref(frame);
      }
    } else if (pkt->stream_index == player->audio_stream_idx &&
               player->audio_dec_ctx) {
      ret = avcodec_send_packet(player->audio_dec_ctx, pkt);
      if (ret < 0) {
        av_packet_unref(pkt);
        continue;
      }

      while (ret >= 0) {
        ret = avcodec_receive_frame(player->audio_dec_ctx, audio_frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
          break;
        if (ret < 0)
          break;

        if (player->swr_ctx && player->audio_dev != 0) {
          int dst_nb_samples = av_rescale_rnd(
              swr_get_delay(player->swr_ctx, player->audio_sample_rate) +
                  audio_frame->nb_samples,
              player->audio_sample_rate, player->audio_sample_rate,
              AV_ROUND_UP);
          int out_channels = player->audio_channels;
          int out_buffer_size = av_samples_get_buffer_size(
              NULL, out_channels, dst_nb_samples, AV_SAMPLE_FMT_S16, 1);
          if (out_buffer_size > 0) {
            uint8_t *audio_buf = (uint8_t *)av_malloc(out_buffer_size);
            if (audio_buf) {
              int converted = swr_convert(
                  player->swr_ctx, &audio_buf, dst_nb_samples,
                  (const uint8_t **)audio_frame->data, audio_frame->nb_samples);
              if (converted > 0) {
                int bytes = av_samples_get_buffer_size(
                    NULL, out_channels, converted, AV_SAMPLE_FMT_S16, 1);
                if (bytes > 0) {
                  SDL_QueueAudio(player->audio_dev, audio_buf, bytes);
                }
              }
              av_free(audio_buf);
            }
          }
        }

        // Update position for audio-only streams
        if (player->video_stream_idx == -1) {
          double pts = audio_frame->pts *
                       av_q2d(player->fmt_ctx->streams[player->audio_stream_idx]
                                  ->time_base);
          player->position = pts;
        }
        av_frame_unref(audio_frame);
      }
    }
    av_packet_unref(pkt);
  }

end:
  if (buffer)
    av_free(buffer);
  av_frame_free(&frame);
  av_frame_free(&frame_rgb);
  av_frame_free(&audio_frame);
  av_packet_free(&pkt);
  if (player->video_dec_ctx)
    avcodec_free_context(&player->video_dec_ctx);
  if (player->audio_dec_ctx)
    avcodec_free_context(&player->audio_dec_ctx);
  if (player->fmt_ctx)
    avformat_close_input(&player->fmt_ctx);
  if (player->sws_ctx)
    sws_freeContext(player->sws_ctx);
  if (player->swr_ctx)
    swr_free(&player->swr_ctx);
  if (player->audio_dev != 0) {
    SDL_ClearQueuedAudio(player->audio_dev);
    SDL_CloseAudioDevice(player->audio_dev);
    player->audio_dev = 0;
  }

  player->running = FALSE;
  player->state = PLAYER_STATE_STOPPED;
  return NULL;
}
