#include "awtk.h"
#include "mvvm/mvvm.h"

#include "view_model/player_view_model.h"
#include "view/video_image.h"
#include "view/video_view.h"
#include "view_model/player_view_model.h"

ret_t application_init(void) {
  mvvm_init();
  
  widget_factory_register(widget_factory(), WIDGET_TYPE_VIDEO_IMAGE, video_image_create);
  widget_factory_register(widget_factory(), WIDGET_TYPE_VIDEO_VIEW, video_view_create);
  view_model_factory_register("player", player_view_model_create);

  return navigator_to("main");
}

ret_t application_exit(void) {
  log_debug("application_exit\n");
  return RET_OK;
}
