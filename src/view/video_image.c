#include "video_image.h"

static ret_t video_image_on_paint_self(widget_t* widget, canvas_t* c) {
  video_image_t* video_image = VIDEO_IMAGE(widget);

  if (video_image->image) {
    rect_t dst = rect_init(0, 0, widget->w, widget->h);
    canvas_draw_image_ex(c, video_image->image, IMAGE_DRAW_SCALE, &dst);
  } else {
    canvas_set_fill_color_str(c, "black");
    canvas_fill_rect(c, 0, 0, widget->w, widget->h);
  }

  return RET_OK;
}

static ret_t video_image_on_destroy(widget_t* widget) {
  /* We don't own the bitmap, ViewModel does */
  return RET_OK;
}

static ret_t video_image_get_prop(widget_t* widget, const char* name, value_t* v) {
  video_image_t* video_image = VIDEO_IMAGE(widget);
  if (tk_str_eq(name, "image")) {
    value_set_pointer(v, video_image->image);
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static ret_t video_image_set_prop(widget_t* widget, const char* name, const value_t* v) {
  video_image_t* video_image = VIDEO_IMAGE(widget);
  if (tk_str_eq(name, "image")) {
    video_image->image = (bitmap_t*)value_pointer(v);
    widget_invalidate(widget, NULL);
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

TK_DECL_VTABLE(video_image) = {
    .size = sizeof(video_image_t),
    .type = WIDGET_TYPE_VIDEO_IMAGE,
    .parent = TK_PARENT_VTABLE(widget),
    .create = video_image_create,
    .on_paint_self = video_image_on_paint_self,
    .get_prop = video_image_get_prop,
    .set_prop = video_image_set_prop,
    .on_destroy = video_image_on_destroy};

widget_t* video_image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(video_image), x, y, w, h);
}

ret_t video_image_set_image(widget_t* widget, bitmap_t* image) {
  video_image_t* video_image = VIDEO_IMAGE(widget);
  return_value_if_fail(video_image != NULL, RET_BAD_PARAMS);

  video_image->image = image;
  widget_invalidate(widget, NULL);

  return RET_OK;
}
