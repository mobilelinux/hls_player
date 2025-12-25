#ifndef VIDEO_IMAGE_H
#define VIDEO_IMAGE_H

#include "awtk.h"

BEGIN_C_DECLS

#define WIDGET_TYPE_VIDEO_IMAGE "video_image"

widget_t* video_image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
ret_t video_image_set_image(widget_t* widget, bitmap_t* image);

#define VIDEO_IMAGE(widget) ((video_image_t*)(widget))

typedef struct _video_image_t {
  widget_t widget;
  bitmap_t* image;
} video_image_t;

END_C_DECLS

#endif /* VIDEO_IMAGE_H */
