#pragma once
#include "../mui/mui.h"

INC_ICON(Terminal);

void  __mui_icons_constructor(){
  printf("Loading Icons...\n");
  printf("Terminal size: %db\n", icon_Terminal_size);

  int           req_format = STBI_rgb_alpha;
  int           width, height, orig_format;
  unsigned char *data = stbi_load("./assets/Terminal_16x16x32.png", &width, &height, &orig_format, req_format);
  if (data == NULL) {
    SDL_Log("Loading image failed");
    exit(1);
  }

  int    depth, pitch;
  Uint32 pixel_format;
  if (req_format == STBI_rgb) {
    depth        = 24;
    pitch        = 3 * width; // 3 bytes per pixel * pixels per row
    pixel_format = SDL_PIXELFORMAT_RGB24;
  } else {                    // STBI_rgb_alpha (RGBA)
    depth        = 32;
    pitch        = 4 * width;
    pixel_format = SDL_PIXELFORMAT_RGBA32;
  }

  SDL_Log("depth:%d,pitch:%d,pixel_format:%d", depth, pitch, pixel_format);
}

