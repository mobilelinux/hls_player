#ifndef HLS_PLAYER_H
#define HLS_PLAYER_H

#include "awtk.h"

BEGIN_C_DECLS

typedef enum _player_state_t {
  PLAYER_STATE_STOPPED = 0,
  PLAYER_STATE_PLAYING,
  PLAYER_STATE_PAUSED,
  PLAYER_STATE_BUFFERING
} player_state_t;

typedef struct _hls_player_t hls_player_t;

hls_player_t* hls_player_create(void);
ret_t hls_player_set_url(hls_player_t* player, const char* url);
ret_t hls_player_play(hls_player_t* player);
ret_t hls_player_pause(hls_player_t* player);
ret_t hls_player_stop(hls_player_t* player);
ret_t hls_player_destroy(hls_player_t* player);

player_state_t hls_player_get_state(hls_player_t* player);
double hls_player_get_position(hls_player_t* player);
double hls_player_get_duration(hls_player_t* player);

/* Callback for video frame update */
typedef void (*hls_player_on_frame_t)(void* ctx, const void* data, int width, int height, int format);
void hls_player_set_on_frame(hls_player_t* player, hls_player_on_frame_t on_frame, void* ctx);

END_C_DECLS

#endif /* HLS_PLAYER_H */
