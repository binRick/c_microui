#pragma once
#ifndef MUIICONSH
#define MUIICONSH
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define INCBIN_SILENCE_BITCODE_WARNING
#define INCBIN_STYLE     INCBIN_STYLE_SNAKE
#define INCBIN_PREFIX    icon_
#define ICONS_DIR        "../assets/"
#define ICONS_SUFFIX     "_16x16x32.png"
#include "incbin/incbin.h"
#include "../mui/mui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
void __mui_icons_constructor();

#define _ICON_STR(S)    #S
#define ICON_STR(S)     _ICON_STR(S)

#define INC_ICON(NAME) \
  INCBIN(Uint16, NAME, ICONS_DIR ICON_STR(NAME) ICONS_SUFFIX)

#endif
