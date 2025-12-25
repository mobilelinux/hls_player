#include "player_view_model.h"
#include "../model/hls_player.h"
#include "tkc/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _player_view_model_t {
  view_model_t view_model;
  hls_player_t *player;

  /* Properties */
  char *url;
  char *state_str;
  bitmap_t *image;
  char position_text[8];
  char duration_text[8];
  double progress;
  bool_t update_pending;
} player_view_model_t;

typedef struct _frame_info_t {
  int w;
  int h;
  uint8_t *data;
  player_view_model_t *vm;
} frame_info_t;

static void format_time(double seconds, char *buffer, size_t size) {
  if (seconds < 0) {
    seconds = 0;
  }
  int total = (int)(seconds + 0.5);
  int minutes = total / 60;
  int secs = total % 60;
  if (size > 0) {
    snprintf(buffer, size, "%02d:%02d", minutes, secs);
  }
}

static void player_view_model_update_progress(player_view_model_t *vm) {
  return_if_fail(vm != NULL && vm->player != NULL);
  double duration = hls_player_get_duration(vm->player);
  double position = hls_player_get_position(vm->player);
  format_time(position, vm->position_text, sizeof(vm->position_text));
  format_time(duration, vm->duration_text, sizeof(vm->duration_text));
  if (duration > 0.0) {
    double percent = (position / duration) * 100.0;
    if (percent < 0)
      percent = 0;
    if (percent > 100)
      percent = 100;
    vm->progress = percent;
  } else {
    vm->progress = 0;
  }
}

static void player_view_model_reset_progress(player_view_model_t *vm) {
  return_if_fail(vm != NULL);
  format_time(0, vm->position_text, sizeof(vm->position_text));
  if (vm->player) {
    format_time(hls_player_get_duration(vm->player), vm->duration_text,
                sizeof(vm->duration_text));
  } else {
    format_time(0, vm->duration_text, sizeof(vm->duration_text));
  }
  vm->progress = 0;
}

static ret_t on_update_ui(const idle_info_t *info) {
  frame_info_t *frame = (frame_info_t *)(info->ctx);
  player_view_model_t *vm = frame->vm;

  if (vm->image == NULL || vm->image->w != frame->w ||
      vm->image->h != frame->h) {
    if (vm->image) {
      bitmap_destroy(vm->image);
    }
    log_debug("on_update_ui: create bitmap: w=%d, h=%d\n", frame->w, frame->h);
    vm->image = bitmap_create_ex(frame->w, frame->h, 0, BITMAP_FMT_RGBA8888);
  }

  if (vm->image) {
    uint8_t *data = (uint8_t *)bitmap_lock_buffer_for_write(vm->image);
    if (data) {
      memcpy(data, frame->data, frame->w * frame->h * 4);
      bitmap_unlock_buffer(vm->image);
    }
  }

  player_view_model_update_progress(vm);
  view_model_notify_props_changed(VIEW_MODEL(vm));

  free(frame->data);
  free(frame);
  vm->update_pending = FALSE;
  return RET_REMOVE;
}

static void on_frame_callback(void *ctx, const void *data, int w, int h,
                              int format) {
  player_view_model_t *vm = (player_view_model_t *)ctx;

  if (vm->update_pending) {
    return;
  }

  uint32_t size = w * h * 4;
  uint8_t *buffer = (uint8_t *)malloc(size);
  if (buffer) {
    memcpy(buffer, data, size);
    frame_info_t *info = (frame_info_t *)malloc(sizeof(frame_info_t));
    info->w = w;
    info->h = h;
    info->data = buffer;
    info->vm = vm;
    vm->update_pending = TRUE;
    idle_queue(on_update_ui, info);
  }
}

static ret_t player_view_model_set_prop(tk_object_t *obj, const char *name,
                                        const value_t *v) {
  view_model_t *view_model = VIEW_MODEL(obj);
  player_view_model_t *vm = (player_view_model_t *)view_model;

  if (tk_str_eq(name, "url")) {
    if (vm->url)
      free(vm->url);
    vm->url = tk_strdup(value_str(v));
    if (vm->player) {
      hls_player_set_url(vm->player, vm->url);
    }
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t player_view_model_get_prop(tk_object_t *obj, const char *name,
                                        value_t *v) {
  view_model_t *view_model = VIEW_MODEL(obj);
  player_view_model_t *vm = (player_view_model_t *)view_model;

  if (tk_str_eq(name, "url")) {
    value_set_str(v, vm->url);
    return RET_OK;
  } else if (tk_str_eq(name, "state")) {
    value_set_str(v, vm->state_str);
    return RET_OK;
  } else if (tk_str_eq(name, "image")) {
    value_set_pointer(v, vm->image);
    return RET_OK;
  } else if (tk_str_eq(name, "position_text")) {
    value_set_str(v, vm->position_text);
    return RET_OK;
  } else if (tk_str_eq(name, "duration_text")) {
    value_set_str(v, vm->duration_text);
    return RET_OK;
  } else if (tk_str_eq(name, "progress")) {
    value_set_double(v, vm->progress);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static bool_t player_view_model_can_exec(tk_object_t *obj, const char *name,
                                         const char *args) {
  return TRUE;
}

static ret_t player_view_model_exec(tk_object_t *obj, const char *name,
                                    const char *args) {
  view_model_t *view_model = VIEW_MODEL(obj);
  player_view_model_t *vm = (player_view_model_t *)view_model;

  if (tk_str_eq(name, "play")) {
    log_debug("exec play url: %s\n", vm->url);
    if (vm->player && vm->url) {
      hls_player_set_url(vm->player, vm->url);
      hls_player_play(vm->player);

      if (vm->state_str)
        free(vm->state_str);
      vm->state_str = tk_strdup("Playing");
      player_view_model_update_progress(vm);
      view_model_notify_props_changed(view_model);
    }
    return RET_OK;
  } else if (tk_str_eq(name, "pause")) {
    if (vm->player) {
      hls_player_pause(vm->player);

      if (vm->state_str)
        free(vm->state_str);
      vm->state_str = tk_strdup("Paused");
      player_view_model_update_progress(vm);
      view_model_notify_props_changed(view_model);
    }
    return RET_OK;
  } else if (tk_str_eq(name, "stop")) {
    if (vm->player) {
      hls_player_stop(vm->player);

      if (vm->state_str)
        free(vm->state_str);
      vm->state_str = tk_strdup("Stopped");
      player_view_model_reset_progress(vm);
      view_model_notify_props_changed(view_model);
    }
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t player_view_model_on_destroy(tk_object_t *obj) {
  view_model_t *view_model = VIEW_MODEL(obj);
  player_view_model_t *vm = (player_view_model_t *)view_model;

  if (vm->player) {
    hls_player_destroy(vm->player);
    vm->player = NULL;
  }
  if (vm->url) {
    free(vm->url);
    vm->url = NULL;
  }
  if (vm->state_str) {
    free(vm->state_str);
    vm->state_str = NULL;
  }
  if (vm->image) {
    bitmap_destroy(vm->image);
    vm->image = NULL;
  }

  return RET_OK;
}

static const object_vtable_t s_player_view_model_vtable = {
    .type = "player_view_model",
    .desc = "player_view_model",
    .size = sizeof(player_view_model_t),
    .is_collection = FALSE,
    .on_destroy = player_view_model_on_destroy,
    .get_prop = player_view_model_get_prop,
    .set_prop = player_view_model_set_prop,
    .can_exec = player_view_model_can_exec,
    .exec = player_view_model_exec};

view_model_t *player_view_model_create(navigator_request_t *req) {
  tk_object_t *obj = tk_object_create(&s_player_view_model_vtable);
  view_model_t *view_model = view_model_init(VIEW_MODEL(obj));
  player_view_model_t *vm = (player_view_model_t *)view_model;

  vm->player = hls_player_create();
  hls_player_set_on_frame(vm->player, on_frame_callback, vm);
  vm->url = tk_strdup(
      "http://devimages.apple.com/iphone/samples/bipbop/bipbopall.m3u8");
  hls_player_set_url(vm->player, vm->url);
  vm->state_str = tk_strdup("Stopped");
  player_view_model_reset_progress(vm);

  return view_model;
}
