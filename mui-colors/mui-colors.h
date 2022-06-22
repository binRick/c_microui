#include <assert.h>
#include <fnmatch.h>
#include <libproc.h>
#include <mach/mach_time.h>
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
#include "mui-render-options.h"
#include "mui-render.h"
#include "mui.h"
#include "submodules/meson_deps/submodules/c_colors/db/db.h"
#include "submodules/meson_deps/submodules/c_darwin/active-app/active-app.h"
#include "submodules/meson_deps/submodules/c_string_buffer/include/stringbuffer.h"
#include "submodules/meson_deps/submodules/c_stringfn/include/stringfn.h"
#include "submodules/meson_deps/submodules/djbhash/src/djbhash.h"
#include "submodules/meson_deps/submodules/parson/parson.h"
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#define WINDOW_WIDTH    650

int mui_colors();
