#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#ifndef MUIRENDERC
#define MUIRENDERC
#define TERMINAL_FONT_SIZE    30
#include "../mui-rectangle/mui-rectangle.h"
#include "../mui/mui.h"
TTF_Font            *font;
SDL_Surface         *text = NULL;
static GLfloat      tex_buf[BUFFER_SIZE * 8];
static GLfloat      vert_buf[BUFFER_SIZE * 8];
static GLubyte      color_buf[BUFFER_SIZE * 16];
static GLuint       index_buf[BUFFER_SIZE * 6];
static int          buf_idx;
static SDL_Window   *window = NULL, *window2 = NULL, *window_wrapper = NULL;
static SDL_Renderer *renderer  = NULL;
SDL_Renderer        *renderer2 = NULL;
static SDL_Texture  *texture   = NULL;
#define GL_RENDERER    true

void render_terminal(struct mui_init_cfg_t CFG){
  __mui_icons_constructor();
  int       rendermethod = TextRenderShaded;
  int       renderstyle  = TTF_STYLE_NORMAL;
  int       rendertype   = RENDER_LATIN1;
  int       outline      = 0;
  int       hinting      = TTF_HINTING_NORMAL;
  int       kerning      = 1;
  char      *font_file   = "../" FONT_FILE;
  char      *string      = "ok123";
  SDL_Color white        = { 0xFF, 0xFF, 0xFF, 0 };
  SDL_Color black        = { 0x00, 0x00, 0x00, 0 };
  SDL_Color *forecol;
  SDL_Color *backcol;
  int       ptsize = TERMINAL_FONT_SIZE;

  font = TTF_OpenFont(font_file, ptsize);
  if (font == NULL) {
    SDL_Log("Couldn't load %d pt font from %s: %s\n", ptsize, font_file, SDL_GetError());
    exit(2);
  }
  ///////////////////////////////////////////
  TTF_SetFontKerning(font, kerning);
  TTF_SetFontSDF(font, SDL_TRUE);
  TTF_SetFontHinting(font, TTF_HINTING_MONO);
  TTF_SetFontWrappedAlign(font, TTF_WRAPPED_ALIGN_LEFT);
  TTF_SetFontStyle(font, TTF_STYLE_BOLD);
  TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
  TTF_SetFontStyle(font, TTF_STYLE_UNDERLINE);
  TTF_SetFontStyle(font, TTF_STYLE_UNDERLINE | TTF_STYLE_UNDERLINE | TTF_STYLE_ITALIC | TTF_STYLE_BOLD);
  TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
  TTF_SetFontOutline(font, outline);
  ///////////////////////////////////////////
  int                     is_fixed_width = TTF_FontFaceIsFixedWidth(font);
  char                    *font_style = TTF_FontFaceStyleName(font);
  char                    *font_family = TTF_FontFaceFamilyName(font);
  long                    font_faces_qty = TTF_FontFaces(font);
  int                     font_ascent = TTF_FontAscent(font);
  int                     x_w, x_h, r2_w, r2_h;
  TTF_SizeText(font, "O", &x_w, &x_h);
  struct SDL_RendererInfo *renderer2_info = malloc(sizeof(struct SDL_RendererInfo));
  SDL_GetRendererInfo(renderer2, renderer2_info);
  SDL_GetRendererOutputSize(renderer2, &r2_w, &r2_h);
  term_cols = r2_w / x_w;
  term_rows = r2_h / x_h;
  SDL_Log("font '%s'|'%s' has %lu faces, ascent: %d, is fixed width: %d, x width: %d, x height: %d\n\tterm width: %x, term height: %d\n\tterminal rows: %d, terminal cols: %d",
          font_family,
          font_style,
          font_faces_qty,
          font_ascent,
          is_fixed_width, x_w, x_h,
          r2_w, r2_h,
          term_rows,
          term_cols
          );
  _Scene scene;

/*
 * text                = TTF_RenderText_Shaded_Wrapped(font, string, *forecol, *backcol, 0);
 * scene.captionRect.x = 4;
 * scene.captionRect.y = 4;
 * scene.captionRect.w = text->w;
 * scene.captionRect.h = text->h;
 * scene.caption       = SDL_CreateTextureFromSurface(renderer2, text);
 * _Scene *s = &scene;
 * SDL_FreeSurface(text);
 * text = TTF_RenderText_Shaded_Wrapped(font, CFG.terminal_content, *forecol, *backcol, 0);
 */

  if (text == NULL) {
    SDL_Log("Couldn't render text: %s\n", SDL_GetError());
    TTF_CloseFont(font);
    exit(2);
  }

  SDL_Texture *Loading_Surf = SDL_LoadBMP("/tmp/hello.bmp");
//  SDL_Texture *Background_Tx = SDL_CreateTextureFromSurface(renderer2, Loading_Surf);

  SDL_FreeSurface(Loading_Surf);

  scene.messageRect.x = 0;
  scene.messageRect.y = 1;
  scene.messageRect.w = text->w;
  scene.messageRect.h = text->h;
  scene.message       = SDL_CreateTextureFromSurface(renderer2, text);
  SDL_Log("Font height: %d, text height: %d, text width: %d, font ptsize: %d, \n",
          TTF_FontHeight(font), text->h, text->w, ptsize);
  SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
  SDL_RenderClear(renderer2);
  //    SDL_RenderCopy(renderer2, Background_Tx, NULL, NULL);
//  SDL_RenderCopy(renderer2, s->caption, NULL, &(s->captionRect));
// SDL_RenderCopy(renderer2, s->message, NULL, &(s->messageRect));
//  SDL_RenderPresent(renderer2);
} /* render_terminal */

void r_init(struct mui_init_cfg_t CFG){
  SDL_Log("OPTIONS:%d\n", CFG.options);
  SDL_Log("Offset:%dx%d\n", CFG.x_offset, CFG.y_offset);
  SDL_Log("size:%dx%d\n", CFG.width, CFG.height);
  if (TTF_Init() < 0) {
    SDL_Log("Couldn't initialize TTF: %s\n", SDL_GetError());
    SDL_Quit();
    return;
  }
  int flags = 0;
  flags |= SDL_RENDERER_ACCELERATED;
  flags |= SDL_RENDERER_PRESENTVSYNC;
  SDL_Log("Creating Windows");
  //CFG.options = SDL_WINDOW_OPENGL;
  window = SDL_CreateWindow(
    CFG.title,
    CFG.x_offset, CFG.y_offset,
    CFG.width, CFG.height,
    CFG.options
    );
  window2 = SDL_CreateWindow(
    CFG.title,
    CFG.x_offset, CFG.y_offset + CFG.height,
    CFG.width, CFG.height,
    CFG.options
    );
  SDL_Log("Created Windows");
  int  window_id      = SDL_GetWindowID(window);
  int  window_id2     = SDL_GetWindowID(window2);
  char *window_title  = SDL_GetWindowTitle(window);
  char *window_title2 = SDL_GetWindowTitle(window2);
  SDL_Log("Created Window ID #%d with title '%s'", window_id, window_title);
  SDL_Log("Created Window2 ID #%d with title '%s'", window_id2, window_title2);

  SDL_SetWindowGrab(window, SDL_FALSE);
  SDL_SetWindowGrab(window2, SDL_FALSE);

  LOAD_SURFACE_ICON(window, Terminal);

  int flags2 = 0;
//  flags   |= SDL_RENDERER_ACCELERATED;
  //flags   |= SDL_RENDERER_PRESENTVSYNC;
  //SDL_RENDERER_ACCELERATED;
//  SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
  if (GL_RENDERER == true) {
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
  }

  renderer = SDL_CreateRenderer(window, -1, flags);
  if (renderer == NULL) {
    SDL_Log("Error creating SDL renderer: %s", SDL_GetError());
    return;
  } else {
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    SDL_Log("Current SDL Renderer: %s", info.name);
  }
  renderer2 = SDL_CreateRenderer(window2, -1, flags);
  if (renderer2 == NULL) {
    SDL_Log("Error creating SDL renderer2: %s", SDL_GetError());
    return;
  } else {
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer2, &info);
    SDL_Log("Current SDL Renderer2: %s", info.name);
  }
  SDL_Log("Created renderers");

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, ATLAS_WIDTH, ATLAS_HEIGHT);
  if (texture == NULL) {
    SDL_Log("Error creating texture: %s", SDL_GetError());
    return;
  }
  /* atlas_texture only contains GL_ALPHA channel, so create an intermediate ABGR8888 for SDL2 */
  {
    Uint8 *ptr = SDL_malloc(4 * ATLAS_WIDTH * ATLAS_HEIGHT);
    for (int x = 0; x < ATLAS_WIDTH * ATLAS_HEIGHT; x++) {
      ptr[4 * x]     = 255;
      ptr[4 * x + 1] = 255;
      ptr[4 * x + 2] = 255;
      ptr[4 * x + 3] = atlas_texture[x];
    }
    SDL_UpdateTexture(texture, NULL, ptr, 4 * ATLAS_WIDTH);
    SDL_free(ptr);
  }
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
//  render_terminal(CFG);
} /* r_init */

static void flush(void) {
  if (buf_idx == 0) {
    return;
  }

  int   xy_stride = 2 * sizeof(float);
  float *xy       = vert_buf;

  int   uv_stride = 2 * sizeof(float);
  float *uv       = tex_buf;

  int   col_stride = 4 * sizeof(Uint8);
  int   *color     = (int *)color_buf;

  SDL_RenderGeometryRaw(renderer, texture,
                        xy, xy_stride, color,
                        col_stride,
                        uv, uv_stride,
                        buf_idx * 4,
                        index_buf, buf_idx * 6, sizeof(int));

  buf_idx = 0;
}

static void push_quad(mu_Rect dst, mu_Rect src, mu_Color color) {
  if (buf_idx == BUFFER_SIZE) {
    flush();
  }

  int texvert_idx = buf_idx * 8;
  int color_idx   = buf_idx * 16;
  int element_idx = buf_idx * 4;
  int index_idx   = buf_idx * 6;
  buf_idx++;

  /* update texture buffer */
  float x = src.x / (float)ATLAS_WIDTH;
  float y = src.y / (float)ATLAS_HEIGHT;
  float w = src.w / (float)ATLAS_WIDTH;
  float h = src.h / (float)ATLAS_HEIGHT;
  tex_buf[texvert_idx + 0] = x;
  tex_buf[texvert_idx + 1] = y;
  tex_buf[texvert_idx + 2] = x + w;
  tex_buf[texvert_idx + 3] = y;
  tex_buf[texvert_idx + 4] = x;
  tex_buf[texvert_idx + 5] = y + h;
  tex_buf[texvert_idx + 6] = x + w;
  tex_buf[texvert_idx + 7] = y + h;

  /* update vertex buffer */
  vert_buf[texvert_idx + 0] = dst.x;
  vert_buf[texvert_idx + 1] = dst.y;
  vert_buf[texvert_idx + 2] = dst.x + dst.w;
  vert_buf[texvert_idx + 3] = dst.y;
  vert_buf[texvert_idx + 4] = dst.x;
  vert_buf[texvert_idx + 5] = dst.y + dst.h;
  vert_buf[texvert_idx + 6] = dst.x + dst.w;
  vert_buf[texvert_idx + 7] = dst.y + dst.h;

  /* update color buffer */
  memcpy(color_buf + color_idx + 0, &color, 4);
  memcpy(color_buf + color_idx + 4, &color, 4);
  memcpy(color_buf + color_idx + 8, &color, 4);
  memcpy(color_buf + color_idx + 12, &color, 4);

  /* update index buffer */
  index_buf[index_idx + 0] = element_idx + 0;
  index_buf[index_idx + 1] = element_idx + 1;
  index_buf[index_idx + 2] = element_idx + 2;
  index_buf[index_idx + 3] = element_idx + 2;
  index_buf[index_idx + 4] = element_idx + 3;
  index_buf[index_idx + 5] = element_idx + 1;
} /* push_quad */

void r_draw_rect(mu_Rect rect, mu_Color color) {
  push_quad(rect, atlas[ATLAS_WHITE], color);
}

void r_draw_text(const char *text, mu_Vec2 pos, mu_Color color) {
  mu_Rect dst = { pos.x, pos.y, 0, 0 };

  for (const char *p = text; *p; p++) {
    if ((*p & 0xc0) == 0x80) {
      continue;
    }
    int     chr = mu_min((unsigned char)*p, 127);
    mu_Rect src = atlas[ATLAS_FONT + chr];
    dst.w = src.w;
    dst.h = src.h;
    push_quad(dst, src, color);
    dst.x += dst.w;
  }
}

void r_draw_icon(int id, mu_Rect rect, mu_Color color) {
  mu_Rect src = atlas[id];
  int     x   = rect.x + (rect.w - src.w) / 2;
  int     y   = rect.y + (rect.h - src.h) / 2;

  push_quad(mu_rect(x, y, src.w, src.h), src, color);
}

int r_get_text_width(const char *text, int len) {
  int res = 0;

  for (const char *p = text; *p && len--; p++) {
    if ((*p & 0xc0) == 0x80) {
      continue;
    }
    int chr = mu_min((unsigned char)*p, 127);
    res += atlas[ATLAS_FONT + chr].w;
  }
  return(res);
}

int r_get_text_height(void) {
  return(18);
}

void r_set_clip_rect(mu_Rect rect) {
  flush();
  SDL_Rect r;
  r.x = rect.x;
  r.y = rect.y;
  r.w = rect.w;
  r.h = rect.h;
  SDL_RenderSetClipRect(renderer, &r);
}

void r_transparent(){
  flush();
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderClear(renderer);
}

void r_clear(mu_Color clr) {
  flush();
  SDL_SetRenderDrawColor(renderer, clr.r, clr.g, clr.b, clr.a);
  SDL_RenderClear(renderer);
}

void r_present(void) {
  flush();
  SDL_RenderPresent(renderer);
}

#endif
