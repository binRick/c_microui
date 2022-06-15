#pragma once
#ifndef WINDOW_TITLE
#define WINDOW_TITLE       "MUI Application"
#endif
#ifndef WINDOW_X_OFFSET
#define WINDOW_X_OFFSET    670
#endif
#ifndef WINDOW_Y_OFFSET
#define WINDOW_Y_OFFSET    100
#endif
#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH       1200
#endif
#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT      600
#endif
#include "mui/mui.h"

void r_init(void);
void r_draw_rect(mu_Rect rect, mu_Color color);
void r_draw_text(const char *text, mu_Vec2 pos, mu_Color color);
void r_draw_icon(int id, mu_Rect rect, mu_Color color);
int r_get_text_width(const char *text, int len);
int r_get_text_height(void);
void r_set_clip_rect(mu_Rect rect);
void r_clear(mu_Color color);
void r_present(void);

