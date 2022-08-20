#pragma once
#include "../mui/mui.h"
#include "active-app/active-app.h"
#include "bytes/bytes.h"
#include "c_stringfn/include/stringfn.h"
#include "libtmt/tmt.h"
#include "ms/ms.h"
#include "rectangle/rectangle.h"
#include <assert.h>
#include <assert.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#include <fnmatch.h>
#include <libproc.h>
#include <mach/mach_time.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/proc_info.h>
#include <sys/sysctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define COLOR_NAMES_DB_PATH    "etc/colornames.bestof.sqlite"
#include "../mui-render/mui-render-options.h"
#include "../mui-render/mui-render.h"
#include "../mui/mui.h"
#include "c_deps/submodules/c_colors/db/db.h"
#include "c_deps/submodules/c_darwin/active-app/active-app.h"
#include "c_deps/submodules/c_string_buffer/include/stringbuffer.h"
#include "c_deps/submodules/c_stringfn/include/stringfn.h"
#include "c_deps/submodules/parson/parson.h"
#include "osx-keys/osx-keys.h"
#include "SDL2/SDL_image.h"
#include "timestamp/timestamp.h"
#include "window-utils/window-utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_thread.h>
#ifdef WINDOW_WIDTH
#undef WINDOW_WIDTH
#endif

#define WINDOW_WIDTH                650
#define          BUTTONS_PER_ROW    5
#define          BUTTON_PADDING     5
#define          BUTTON_SIZE        125
#define          BUTTON_ICON        0
#define          BUTTON_HEIGHT      25
#define CFG_TITLE                   "Rectangle Manager"
#define CFG_X_OFFSET                50
#define CFG_Y_OFFSET                50
#define CFG_WIDTH                   (BUTTON_SIZE * BUTTONS_PER_ROW)
#define CFG_OPTIONS         \
  SDL_WINDOW_ALLOW_HIGHDPI  \
  | SDL_WINDOW_OPENGL       \
  | SDL_WINDOW_POPUP_MENU   \
  | SDL_WINDOW_BORDERLESS   \
  | SDL_WINDOW_SKIP_TASKBAR \
  | SDL_WINDOW_ALWAYS_ON_TOP
#define BASIC_WINDOW_TITLE          "Windows"
#define BASIC_WINDOW_HEIGHT         300
#define BASIC_WINDOW_OPTIONS        MU_OPT_NODRAG | MU_OPT_NOCLOSE
#define RETAIN_INITIAL_FOCUS        true
struct tmt_exec_t {
  char                   *input;
  size_t                 rows, cols, callbacks_qty;
  int                    cursor_pos_x, cursor_pos_y;
  char                   *cursor_state;
  struct StringFNStrings *output_lines;
  struct StringBuffer    *output_buffer;
  long unsigned          started_ms, dur_ms;
};
typedef struct {
  int red, green, blue;
} color_rgb_t;
int mui_rectangle();
int tmt_exec(struct tmt_exec_t *);
