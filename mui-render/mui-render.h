#pragma once
#include <stdbool.h>
#include "../mui/mui.h"
#include "mui-render-options.h"
struct mui_init_cfg_t {
  int x_offset, y_offset;
  int width, height;
  int options;
  char *title;
  bool retain_initial_focus;
};
void r_init(struct mui_init_cfg_t CFG);
void r_draw_rect(mu_Rect rect, mu_Color color);
void r_draw_text(const char *text, mu_Vec2 pos, mu_Color color);
void r_draw_icon(int id, mu_Rect rect, mu_Color color);
int r_get_text_width(const char *text, int len);
int r_get_text_height(void);
void r_set_clip_rect(mu_Rect rect);
void r_clear(mu_Color color);
void r_transparent();
void r_present(void);

