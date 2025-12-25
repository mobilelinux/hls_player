#ifndef TK_VIDEO_VIEW_H
#define TK_VIDEO_VIEW_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class video_view_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 视频视图控件。
 *
 * 负责管理视频显示的视图，包括初始化 mutable_image 子控件。
 */
typedef struct _video_view_t {
  widget_t widget;
} video_view_t;

/**
 * @method video_view_create
 * 创建 video_view 对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x 坐标
 * @param {xy_t} y y 坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* video_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method video_view_cast
 * 转换为 video_view 对象(供脚本使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget video_view 对象。
 *
 * @return {video_view_t*} video_view 对象。
 */
video_view_t* video_view_cast(widget_t* widget);

#define WIDGET_TYPE_VIDEO_VIEW "video_view"

#define VIDEO_VIEW(widget) ((video_view_t*)(video_view_cast(WIDGET(widget))))

END_C_DECLS

#endif /*TK_VIDEO_VIEW_H*/
