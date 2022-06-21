#include "mui-atlas.h"
#include "mui-render.h"
#include <assert.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define IMG_PATH           "/tmp/a.png"
#define CREATE_RENDERER    true
#define SDL_WINDOW_OPTIONS \
  SDL_WINDOW_ALLOW_HIGHDPI
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
#endif
#ifdef WINDOW_ALWAYS_ON_TOP
#undef __SDL_WINDOW_ALWAYS_ON_TOP
#define __SDL_WINDOW_ALWAYS_ON_TOP    | SDL_WINDOW_ALWAYS_ON_TOP
#endif
//#  | SDL_WINDOW_ALWAYS_ON_TOP
//#  | SDL_WINDOW_BORDERLESS

#define BUFFER_SIZE    16384

static GLfloat tex_buf[BUFFER_SIZE * 8];
static GLfloat      vert_buf[BUFFER_SIZE * 8];
static GLubyte      color_buf[BUFFER_SIZE * 16];
static GLuint       index_buf[BUFFER_SIZE * 6];

static int          width  = WINDOW_WIDTH;
static int          height = WINDOW_HEIGHT;
static int          buf_idx;

static SDL_Window   *window;
static SDL_Renderer *renderer;
static SDL_Texture  *texture;
static int          w, h;


static void screenshot(SDL_Renderer *renderer, const char *filename) {
  int width  = 0;
  int height = 0;

  SDL_GetRendererOutputSize(renderer, &width, &height);

  SDL_Surface *screenshot = SDL_CreateRGBSurface(
    0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

  SDL_RenderReadPixels(
    renderer, NULL, SDL_PIXELFORMAT_ARGB8888, screenshot->pixels,
    screenshot->pitch);
  SDL_SaveBMP(screenshot, filename);
  SDL_FreeSurface(screenshot);
}


void r_init(void) {
  window = SDL_CreateWindow(
    WINDOW_TITLE,
    WINDOW_X_OFFSET, WINDOW_Y_OFFSET,
    width, height,
    SDL_WINDOW_OPTIONS
    __SDL_WINDOW_ALWAYS_ON_TOP \
    __SDL_WINDOW_BORDERLESS    \
    __SDL_WINDOW_HIDDEN        \
    );
//  SDL_RaiseWindow(window);
// SDL_HideWindow(window);
//  SDL_ShowWindow(window);

  SDL_GL_CreateContext(window);
  SDL_SetWindowIcon(window, SDL_LoadBMP("../window_icon.bmp"));


  /* init gl */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  /* init texture */
  GLuint id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, ATLAS_WIDTH, ATLAS_HEIGHT, 0,
               GL_ALPHA, GL_UNSIGNED_BYTE, atlas_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  assert(glGetError() == 0);
  if (CREATE_RENDERER) {
    renderer = SDL_GetRenderer(window);
    if (renderer == NULL) {
      fprintf(stderr, "unable to create renderer: %s\n", SDL_GetError());
    }else{
      fprintf(stderr, "created renderer.................\n");
      texture = IMG_LoadTexture(renderer, IMG_PATH);
      if (!texture) {
        SDL_Log("Couldn't load %s: %s\n", IMG_PATH, SDL_GetError());
      }else{
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        printf("w:%d|h:%d\n", w, h);
      }
    }
  }
} /* r_init */


static void flush(void) {
  if (buf_idx == 0) {
    return;
  }

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0f, width, height, 0.0f, -1.0f, +1.0f);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glTexCoordPointer(2, GL_FLOAT, 0, tex_buf);
  glVertexPointer(2, GL_FLOAT, 0, vert_buf);
  glColorPointer(4, GL_UNSIGNED_BYTE, 0, color_buf);
  glDrawElements(GL_TRIANGLES, buf_idx * 6, GL_UNSIGNED_INT, index_buf);

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

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
  glScissor(rect.x, height - (rect.y + rect.h), rect.w, rect.h);
}


void r_clear(mu_Color clr) {
  flush();
  glClearColor(clr.r / 255., clr.g / 255., clr.b / 255., clr.a / 255.);
  glClear(GL_COLOR_BUFFER_BIT);
}


void r_present(void) {
  flush();
  SDL_GL_SwapWindow(window);
}
