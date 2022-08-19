#include <assert.h>
#include <fnmatch.h>
#include <libproc.h>
#include <mach/mach_time.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/proc_info.h>
#include <sys/sysctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#define COLOR_NAMES_DB_PATH    "etc/colornames.bestof.sqlite"
#include "mui-render-options.h"
#include "mui-render.h"
#include "mui.h"
#include "c_deps/submodules/c_colors/db/db.h"
#include "c_deps/submodules/c_darwin/active-app/active-app.h"
#include "c_deps/submodules/c_string_buffer/include/stringbuffer.h"
#include "c_deps/submodules/c_stringfn/include/stringfn.h"
#include "c_deps/submodules/djbhash/src/djbhash.h"
#include "c_deps/submodules/parson/parson.h"
#include "timestamp/timestamp.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_thread.h>
#include "SDL2/SDL_image.h"
#include <stdbool.h>
#include "window-utils/window-utils.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#ifdef WINDOW_WIDTH
#undef WINDOW_WIDTH
#endif

#define WINDOW_WIDTH    650

int mui_windows();
