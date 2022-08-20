#pragma once
#ifndef MUIICONSH
#define MUIICONSH
#include <SDL2/SDL.h>
/////////////////////////////////////////////////////////////////////////////
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define INCBIN_SILENCE_BITCODE_WARNING
#define INCBIN_STYLE     INCBIN_STYLE_SNAKE
#define INCBIN_PREFIX    icon_
#define ICONS_DIR        "../assets/"
#define ICONS_SUFFIX     "_512x512x32.png"
/////////////////////////////////////////////////////////////////////////////
struct surface_icon_t {
  char          *icon_name;
  unsigned char *icon_data;
  int           icon_size;
  int           width, height, orig_format, depth, pitch, pixel_format, req_format;
  Uint16        *data;
  SDL_Surface   *icon_surface;
};
/////////////////////////////////////////////////////////////////////////////
#include "../mui/mui.h"
#include "incbin/incbin.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
/////////////////////////////////////////////////////////////////////////////
void __mui_icons_constructor();
struct surface_icon_t *get_surface_icon(char *ICON_NAME);
/////////////////////////////////////////////////////////////////////////////
#define ICON_VAR(S)         S
#define ICON_DATA_VAR(S)    "icon_" S "_data"
#define ICON_DATA(S)        ICON_VAR("icon_" ICON_VAR(S) "_data")
#define ICON_SIZE(S)        icon_ ICON_VAR(S) _size
#define _ICON_STR(S)        #S
#define ICON_STR(S)         _ICON_STR(S)
/////////////////////////////////////////////////////////////////////////////
#define INC_ICON(NAME) \
  INCBIN(Uint16, NAME, ICONS_DIR ICON_STR(NAME) ICONS_SUFFIX)
/////////////////////////////////////////////////////////////////////////////
#define LOAD_SURFACE_ICON(WINDOW, ICON_NAME)                            \
  { do {                                                                \
      INCBIN_EXTERN(ICON_NAME);                                         \
      struct surface_icon_t *i = get_surface_icon(ICON_STR(ICON_NAME)); \
      SDL_SetWindowIcon(WINDOW, i->icon_surface);                       \
      SDL_FreeSurface(i->icon_surface);                                 \
    } while (0); }
/////////////////////////////////////////////////////////////////////////////
#endif
