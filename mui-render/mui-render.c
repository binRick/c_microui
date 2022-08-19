#pragma once
#include "mui-atlas/mui-atlas.h"
#include "mui-render/mui-render.h"
#include <assert.h>
#include <stdbool.h>
#include <SDL.h>


typedef float    GLfloat;
typedef Uint8    GLubyte;
typedef Uint32   GLuint;


#define SDL_WINDOW_OPTIONS SDL_WINDOW_ALLOW_HIGHDPI
#define IMG_PATH           "/tmp/a.png"
#define CREATE_RENDERER    true
#define __SDL_WINDOW_HIDDEN
#define __SDL_WINDOW_ALWAYS_ON_TOP
#define __SDL_WINDOW_BORDERLESS 
#ifdef WINDOW_HIDDEN
#undef __SDL_WINDOW_HIDDEN
#define __SDL_WINDOW_HIDDEN    | SDL_WINDOW_HIDDEN
#endif
#ifdef WINDOW_BORDERLESS
#undef __SDL_WINDOW_BORDERLESS
#define __SDL_WINDOW_BORDERLESS    | SDL_WINDOW_BORDERLESS
#define SDL_WINDOW_OPTIONS SDL_WINDOW_OPTIONS|SDL_WINDOW_BORDERLESS
#endif
#ifdef WINDOW_ALWAYS_ON_TOP
#undef __SDL_WINDOW_ALWAYS_ON_TOP
#define __SDL_WINDOW_ALWAYS_ON_TOP    | SDL_WINDOW_ALWAYS_ON_TOP
#define SDL_WINDOW_OPTIONS SDL_WINDOW_OPTIONS|SDL_WINDOW_ALWAYS_ON_TOP
#endif
#define BUFFER_SIZE                   16384

static GLfloat tex_buf[BUFFER_SIZE * 8];
static GLfloat      vert_buf[BUFFER_SIZE * 8];
static GLubyte      color_buf[BUFFER_SIZE * 16];
static GLuint       index_buf[BUFFER_SIZE * 6];

static int          width  = 800;
static int          height = 600;
static int          buf_idx;

static SDL_Window   *window   = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture  *texture  = NULL;


void r_init(struct mui_init_cfg_t CFG){
  SDL_Log("SDL_WINDOW_OPTIONS:%d\n",SDL_WINDOW_OPTIONS);
  SDL_Log("OPTIONS:%d\n",CFG.options);
  SDL_Log("Offset:%dx%d\n",CFG.x_offset,CFG.y_offset);
  SDL_Log("size:%dx%d\n",CFG.width,CFG.height);
  window = SDL_CreateWindow(
    CFG.title,
    CFG.x_offset, CFG.y_offset,
    CFG.width, CFG.height,
    CFG.options
    );
  int window_id = SDL_GetWindowID(window);
  char *window_title = SDL_GetWindowTitle(window);
  SDL_SetWindowGrab(window,SDL_FALSE);
  SDL_Log("Created Window ID #%d with title '%s'",window_id,window_title);

  /* force a renderer for testing */
  SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

  int flags = 0;
  flags |= SDL_RENDERER_ACCELERATED;
  renderer = SDL_CreateRenderer(window, -1, flags);
  if (renderer == NULL) {
    SDL_Log("Error creating SDL renderer: %s", SDL_GetError());
    return;
  } else {
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    SDL_Log("Current SDL Renderer: %s", info.name);
  }

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
