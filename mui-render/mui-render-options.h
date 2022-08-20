#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_ttf.h>
#ifdef WINDOW_HIDDEN
#define __WINDOW_HIDDEN
#endif
#ifndef WINDOW_TITLE
#define WINDOW_TITLE               "MUI Application"
#endif
#ifndef WINDOW_X_OFFSET
#define WINDOW_X_OFFSET            670
#else
#undef WINDOW_X_OFFSET
#define WINDOW_X_OFFSET            WINDOW_X_OFFSET
#endif
#ifndef WINDOW_Y_OFFSET
#define WINDOW_Y_OFFSET            100
#else
#undef WINDOW_Y_OFFSET
#define WINDOW_Y_OFFSET            WINDOW_Y_OFFSET
#endif
#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH               800
#endif
#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT              600
#endif
#ifndef WINDOW_BACKGROUND_RED
#define WINDOW_BACKGROUND_RED      20
#endif
#ifndef WINDOW_BACKGROUND_GREEN
#define WINDOW_BACKGROUND_GREEN    95
#endif
#ifndef WINDOW_BACKGROUND_BLUE
#define WINDOW_BACKGROUND_BLUE     100
#endif
