#pragma once
/////////////////////////////////////////////////////////////////////////
#include "../mui-render/mui-icons.h"
#include "../mui/mui.h"
/////////////////////////////////////////////////////////////////////////
INC_ICON(Terminal);
/////////////////////////////////////////////////////////////////////////
struct surface_icon_t *get_surface_icon(char *ICON_NAME){
  struct surface_icon_t *i = malloc(sizeof(struct surface_icon_t));

  i->icon_name = ICON_NAME;
  printf("icon name:  '%s'\n", ICON_NAME);
  i->icon_data  = icon_Terminal_data;
  i->icon_size  = icon_Terminal_size;
  i->req_format = STBI_rgb_alpha;
  i->data       = stbi_load_from_memory(i->icon_data, i->icon_size, &i->width, &i->height, &i->orig_format, i->req_format);
  if (i->data == NULL) {
    fprintf(stderr, "Loading icon %s failed", i->icon_name);
    exit(1);
  }
  if (i->req_format == STBI_rgb) {
    i->depth        = 24;
    i->pitch        = 3 * i->width; // 3 bytes per pixel * pixels per row
    i->pixel_format = SDL_PIXELFORMAT_RGB24;
  } else {                          // STBI_rgb_alpha (RGBA)
    i->depth        = 32;
    i->pitch        = 4 * i->width;
    i->pixel_format = SDL_PIXELFORMAT_RGBA32;
  }
  SDL_Log("Loaded %db Icon %s> depth:%d,pitch:%d,pixel_format:%d",
          i->icon_size, i->icon_name,
          i->depth, i->pitch, i->pixel_format
          );
  i->icon_surface = SDL_CreateRGBSurfaceWithFormatFrom((void *)i->data, i->width, i->height, i->depth, i->pitch, i->pixel_format);
  return(i);
}

void  __mui_icons_constructor(){
  printf("Loading Icons...\n");
  printf("Terminal size: %db\n", icon_Terminal_size);

  int           req_format = STBI_rgb_alpha;
  int           width, height, orig_format;
  unsigned char *data = stbi_load_from_memory(icon_Terminal_data, icon_Terminal_size, &width, &height, &orig_format, req_format);
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
