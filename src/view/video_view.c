#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/widget_vtable.h"
#include "video_view.h"
#include "mutable_image/mutable_image.h"

static bitmap_t* video_view_create_image(void* ctx, bitmap_format_t format, bitmap_t* old_image) {
  widget_t* widget = WIDGET(ctx);
  value_t v;

  if (widget_get_prop(widget, "image", &v) == RET_OK) {
    bitmap_t* src = (bitmap_t*)value_pointer(&v);
    if (src == NULL) {
      return old_image;
    }

    if (old_image != NULL && old_image->w == src->w && old_image->h == src->h &&
        old_image->format == src->format) {
      return old_image;
    }

    BITMAP_DESTROY(old_image);
    return bitmap_create_ex(src->w, src->h, 0, (bitmap_format_t)src->format);
  }

  return old_image;
}

static ret_t video_view_prepare_image(void* ctx, bitmap_t* image) {
  widget_t* widget = WIDGET(ctx);
  value_t v;

  return_value_if_fail(image != NULL, RET_BAD_PARAMS);

  if (widget_get_prop(widget, "image", &v) == RET_OK) {
    bitmap_t* src = (bitmap_t*)value_pointer(&v);
    if (src != NULL && src->buffer != NULL && image->buffer != NULL) {
      uint8_t* s = bitmap_lock_buffer_for_read(src);
      uint8_t* d = bitmap_lock_buffer_for_write(image);
      uint32_t size = image->line_length * image->h;

      if (s != NULL && d != NULL) {
        memcpy(d, s, size);
      }

      bitmap_unlock_buffer(src);
      bitmap_unlock_buffer(image);
      return RET_OK;
    }
  }

  return RET_FAIL;
}

static ret_t video_view_on_event(widget_t* widget, event_t* e) {
  if (e->type == EVT_WINDOW_WILL_OPEN) {
    widget_t* mutable_image = widget_lookup(widget, "mutable_image", TRUE);
    if (mutable_image != NULL) {
      mutable_image_set_create_image(mutable_image, video_view_create_image, widget);
      mutable_image_set_prepare_image(mutable_image, video_view_prepare_image, widget);
      mutable_image_invalidate_force(mutable_image);
    }
  }
  return RET_OK;
}

TK_DECL_VTABLE(video_view) = {
    .size = sizeof(video_view_t),
    .type = WIDGET_TYPE_VIDEO_VIEW,
    .get_parent_vt = TK_GET_PARENT_VTABLE(widget),
    .create = video_view_create,
    .on_event = video_view_on_event
};

widget_t* video_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(video_view), x, y, w, h);
}

video_view_t* video_view_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, video_view), NULL);

  return (video_view_t*)widget;
}