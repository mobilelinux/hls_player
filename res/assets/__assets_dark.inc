#include "awtk.h"
#include "base/assets_manager.h"
#if !defined(WITH_FS_RES) || defined(AWTK_WEB)
extern TK_CONST_DATA_ALIGN(const unsigned char style_main_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_keyboard_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_window_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char style_default_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_main_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char ui_player_common_default[]);
#ifndef AWTK_WEB
#ifdef WITH_STB_IMAGE
extern TK_CONST_DATA_ALIGN(const unsigned char image_checked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_sound_plus_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shift_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_earth_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_dialog_title_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_hand_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_empty_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_en_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_radio_checked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_zh_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shifton_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_check_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_muted_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_unmuted_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_sound_minus_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_text_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_unchecked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_switch_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_radio_unchecked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_mode_2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_mode_1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_mode_0_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_pause_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_backspace_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_p_default[]);
#else /*WITH_STB_IMAGE*/
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_sound_plus_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_muted_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_earth_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shift_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_en_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_sound_minus_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_switch_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_checked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_zh_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_right_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_mode_2_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_empty_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_radio_unchecked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_shifton_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_up_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_unmuted_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_backspace_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_dialog_title_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_hand_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_mode_0_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_p_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_play_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_pause_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_mode_1_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_check_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_o_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_radio_checked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_unchecked_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_down_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_cursor_text_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_arrow_left_n_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char image_edit_clear_p_default[]);
#endif /*WITH_STB_IMAGE*/
#ifdef WITH_TRUETYPE_FONT
extern TK_CONST_DATA_ALIGN(const unsigned char font_trado_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_ap_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_full_default[]);
extern TK_CONST_DATA_ALIGN(const unsigned char font_default_default[]);
#else /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif /*AWTK_WEB*/
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#endif /*!defined(WITH_FS_RES) || defined(AWTK_WEB)*/

ret_t assets_init_dark(void) {
  assets_manager_t* am = assets_manager();
  assets_manager_set_theme(am, "dark");

#if defined(WITH_FS_RES) && !defined(AWTK_WEB)
  assets_manager_preload(am, ASSET_TYPE_STYLE, "default");
#else /*defined(WITH_FS_RES) && !defined(AWTK_WEB)*/
  assets_manager_add(am, style_main_default);
  assets_manager_add(am, style_keyboard_default);
  assets_manager_add(am, style_window_default);
  assets_manager_add(am, style_default_default);
  assets_manager_add(am, ui_main_default);
  assets_manager_add(am, ui_player_common_default);
#ifndef AWTK_WEB
  assets_manager_add(am, image_checked_default);
  assets_manager_add(am, image_sound_plus_default);
  assets_manager_add(am, image_shift_default);
  assets_manager_add(am, image_earth_default);
  assets_manager_add(am, image_dialog_title_default);
  assets_manager_add(am, image_arrow_up_n_default);
  assets_manager_add(am, image_cursor_hand_default);
  assets_manager_add(am, image_arrow_right_p_default);
  assets_manager_add(am, image_empty_default);
  assets_manager_add(am, image_edit_clear_p_default);
  assets_manager_add(am, image_arrow_up_o_default);
  assets_manager_add(am, image_arrow_left_o_default);
  assets_manager_add(am, image_en_default);
  assets_manager_add(am, image_arrow_left_n_default);
  assets_manager_add(am, image_radio_checked_default);
  assets_manager_add(am, image_zh_default);
  assets_manager_add(am, image_shifton_default);
  assets_manager_add(am, image_check_default);
  assets_manager_add(am, image_cursor_default);
  assets_manager_add(am, image_arrow_down_n_default);
  assets_manager_add(am, image_muted_default);
  assets_manager_add(am, image_arrow_down_o_default);
  assets_manager_add(am, image_play_default);
  assets_manager_add(am, image_unmuted_default);
  assets_manager_add(am, image_sound_minus_default);
  assets_manager_add(am, image_cursor_text_default);
  assets_manager_add(am, image_unchecked_default);
  assets_manager_add(am, image_arrow_down_p_default);
  assets_manager_add(am, image_switch_default);
  assets_manager_add(am, image_radio_unchecked_default);
  assets_manager_add(am, image_mode_2_default);
  assets_manager_add(am, image_mode_1_default);
  assets_manager_add(am, image_mode_0_default);
  assets_manager_add(am, image_arrow_up_p_default);
  assets_manager_add(am, image_edit_clear_o_default);
  assets_manager_add(am, image_arrow_right_n_default);
  assets_manager_add(am, image_arrow_right_o_default);
  assets_manager_add(am, image_edit_clear_n_default);
  assets_manager_add(am, image_pause_default);
  assets_manager_add(am, image_backspace_default);
  assets_manager_add(am, image_arrow_left_p_default);
#ifdef WITH_TRUETYPE_FONT
  assets_manager_add(am, font_trado_default);
  assets_manager_add(am, font_ap_default);
  assets_manager_add(am, font_default_full_default);
  assets_manager_add(am, font_default_default);
#else /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif /*AWTK_WEB*/
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#endif /*defined(WITH_FS_RES) && !defined(AWTK_WEB)*/

  tk_init_assets();
  return RET_OK;
}
