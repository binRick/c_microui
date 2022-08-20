#pragma once
#ifndef MUIBASICC
#define MUIBASICC
#include "../mui-basic/mui-basic.h"
#include "../mui/mui.h"
#define          BUTTONS_PER_ROW    5
#define          BUTTON_PADDING     5
#define          BUTTON_SIZE        125
#define          BUTTON_ICON        0
#define          BUTTON_HEIGHT      25
#define CFG_TITLE                   "Basic App"
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
static struct mui_init_cfg_t CFG = {
  .title    = CFG_TITLE,
  .options  = CFG_OPTIONS,
  .x_offset = CFG_X_OFFSET,.y_offset  = CFG_Y_OFFSET,
  .width    = CFG_WIDTH,   .height    = BASIC_WINDOW_HEIGHT,
};
#define RETAIN_INITIAL_FOCUS    true
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int pid_pre();
int pid_post(int);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
static char        CUR_COLOR_HEX[32], CUR_COLOR_ROW[2048], CUR_COLOR_NAME[32] = "";
static color_rgb_t CUR_COLOR_RGB    = { 0, 0, 0 };
static color_rgb_t CUR_COLOR_RGB_BG = { 0, 0, 0 };
static char        logbuf[64000];
static int         logbuf_updated = 0;
static float       bg[3]          = { WINDOW_BACKGROUND_RED, WINDOW_BACKGROUND_GREEN, WINDOW_BACKGROUND_BLUE };
static float       bg_text[3]     = { WINDOW_BACKGROUND_RED, WINDOW_BACKGROUND_GREEN, WINDOW_BACKGROUND_BLUE };
static float       OUTER_BG[3]    = { 0, 0, 0 };
volatile int       set_focus_qty  = 0;
//////////////////////////////////////////////////////////////////////////

char          windows_qty_title[32];
struct Vector *windows;
const size_t  RELOAD_WINDOWS_LIST_INTERVAL_MS = 2000;
size_t        last_windows_list_reloaded_ts   = 0;
SDL_mutex     *windows_mutex;
SDL_Thread    *poll_windows_thread;
bool          poll_windows_thread_active = false;
window_t      *cur_selected_window       = NULL;
SDL_Texture   *texture;

static void write_log(const char *text) {
  if (logbuf[0]) {
    strcat(logbuf, "\n");
  }
  strcat(logbuf, text);
  logbuf_updated = 1;
}

static int poll_windows_thread_function(void *ARGS){
  SDL_LockMutex(windows_mutex);
  bool active = poll_windows_thread_active;
  SDL_UnlockMutex(windows_mutex);
  while (active) {
    size_t        qty = 0;
    window_t      *w;
    unsigned long dur = 0;
    SDL_LockMutex(windows_mutex);
    {
      unsigned long ts = timestamp();
      active = poll_windows_thread_active;
      if (active == false) {
        break;
      }
      windows                       = get_windows();
      last_windows_list_reloaded_ts = timestamp();
      qty                           = vector_size(windows);
      unsigned long dur = timestamp() - ts;
    }
    SDL_UnlockMutex(windows_mutex);
    for (size_t i = 0; i < qty; i++) {
      SDL_LockMutex(windows_mutex);
      w = (window_t *)vector_get(windows, i);
      SDL_UnlockMutex(windows_mutex);
    }
    SDL_Delay(RELOAD_WINDOWS_LIST_INTERVAL_MS);
  }
  SDL_Log(">poll_windows_thread_function exited");
  return(0);
} /* poll_windows_thread_function */

static void basic_window(mu_Context *ctx) {
  if (mu_begin_window_ex(ctx, "Basic", mu_rect(0, 0, CFG.width, BASIC_WINDOW_HEIGHT), BASIC_WINDOW_OPTIONS)) {
    mu_Container *win = mu_get_current_container(ctx);
    win->rect.w = mu_max(win->rect.w, CFG.width);
    win->rect.h = mu_max(win->rect.h, CFG.height);
    window_t *w;
    size_t   qty = 0;
    SDL_LockMutex(windows_mutex);
    qty = vector_size(windows);
    SDL_UnlockMutex(windows_mutex);
    if (mu_header_ex(ctx, BASIC_WINDOW_TITLE, BASIC_WINDOW_OPTIONS | MU_OPT_EXPANDED | MU_OPT_AUTOSIZE)) {
      for (size_t i = 0; i < qty; i++) {
        SDL_LockMutex(windows_mutex);
        w = (window_t *)vector_get(windows, i);
        SDL_UnlockMutex(windows_mutex);
        if ((i % BUTTONS_PER_ROW) == 0) {
          mu_layout_row(ctx, BUTTONS_PER_ROW, (int[]) {
            CFG.width / BUTTONS_PER_ROW - BUTTON_PADDING,
            CFG.width / BUTTONS_PER_ROW - BUTTON_PADDING,
            CFG.width / BUTTONS_PER_ROW - BUTTON_PADDING,
            CFG.width / BUTTONS_PER_ROW - BUTTON_PADDING,
            CFG.width / BUTTONS_PER_ROW - BUTTON_PADDING,
          }, BUTTON_HEIGHT);
        }
        char *button_name;
        asprintf(&button_name, "%s-%d", w->app_name, w->window_id);
        if (mu_button_ex(ctx, button_name, BUTTON_ICON, MU_OPT_ALIGNCENTER)) {
          SDL_Log("clicked app name: %s | window id: %d", w->app_name, w->window_id);
          SDL_LockMutex(windows_mutex);
          cur_selected_window = w;
          SDL_UnlockMutex(windows_mutex);
          SDL_Log("cur window id: %d", cur_selected_window->window_id);
        }
        free(button_name);
      }
    }
    mu_end_window(ctx);
  }
} /* windows_window */

static int uint8_slider(mu_Context *ctx, unsigned char *value, int low, int high) {
  static float tmp;

  mu_push_id(ctx, &value, sizeof(value));
  tmp = *value;
  int res = mu_slider_ex(ctx, &tmp, low, high, 0, "%.0f", MU_OPT_ALIGNCENTER);

  *value = tmp;
  mu_pop_id(ctx);
  return(res);
}

static void style_window(mu_Context *ctx) {
  static struct { const char *label; int idx; } windows[] = {
    { "text:",        MU_COLOR_TEXT        },
    { "border:",      MU_COLOR_BORDER      },
    { "windowbg:",    MU_COLOR_WINDOWBG    },
    { "titlebg:",     MU_COLOR_TITLEBG     },
    { "titletext:",   MU_COLOR_TITLETEXT   },
    { "panelbg:",     MU_COLOR_PANELBG     },
    { "button:",      MU_COLOR_BUTTON      },
    { "buttonhover:", MU_COLOR_BUTTONHOVER },
    { "buttonfocus:", MU_COLOR_BUTTONFOCUS },
    { "base:",        MU_COLOR_BASE        },
    { "basehover:",   MU_COLOR_BASEHOVER   },
    { "basefocus:",   MU_COLOR_BASEFOCUS   },
    { "scrollbase:",  MU_COLOR_SCROLLBASE  },
    { "scrollthumb:", MU_COLOR_SCROLLTHUMB },
    { NULL }
  };
}

static void process_frame(mu_Context *ctx) {
  mu_begin(ctx);
  basic_window(ctx);
  mu_end(ctx);
}

static const char button_map[256] = {
  [SDL_BUTTON_LEFT & 0xff]   = MU_MOUSE_LEFT,
  [SDL_BUTTON_RIGHT & 0xff]  = MU_MOUSE_RIGHT,
  [SDL_BUTTON_MIDDLE & 0xff] = MU_MOUSE_MIDDLE,
};

static const char key_map[256] = {
  [SDLK_LSHIFT & 0xff]    = MU_KEY_SHIFT,
  [SDLK_RSHIFT & 0xff]    = MU_KEY_SHIFT,
  [SDLK_LCTRL & 0xff]     = MU_KEY_CTRL,
  [SDLK_RCTRL & 0xff]     = MU_KEY_CTRL,
  [SDLK_LALT & 0xff]      = MU_KEY_ALT,
  [SDLK_RALT & 0xff]      = MU_KEY_ALT,
  [SDLK_RETURN & 0xff]    = MU_KEY_RETURN,
  [SDLK_BACKSPACE & 0xff] = MU_KEY_BACKSPACE,
};

int text_width(mu_Font font, const char *text, int len) {
  if (len == -1) {
    len = strlen(text);
  }
  return(r_get_text_width(text, len));
}

int text_height(mu_Font font) {
  return(r_get_text_height());
}

int pid_post(int pid){
  if (RETAIN_INITIAL_FOCUS) {
    SDL_Log("setting focused process to pid %d", pid);
    bool ok = set_focused_pid(pid);
    printf("set ok:%d\n", ok);
    return(ok);
  }
  return(0);
}

int mui_basic(){
  windows_mutex = SDL_CreateMutex();
  int threadReturnValue = -1;
//  int focused_pid       = pid_pre();
  SDL_Init(SDL_INIT_EVERYTHING);
  r_init(CFG);

  mu_Context *ctx = malloc(sizeof(mu_Context));

  mu_init(ctx);
  ctx->text_width  = text_width;
  ctx->text_height = text_height;

  SDL_LockMutex(windows_mutex);
  windows                    = get_windows();
  poll_windows_thread_active = true;
  SDL_UnlockMutex(windows_mutex);
  poll_windows_thread = SDL_CreateThread(poll_windows_thread_function, "PollWindows", (void *)NULL);
  if (NULL == poll_windows_thread) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateThread failed: %s\n", SDL_GetError());
  } else {
    SDL_Log("Thread poll windows created");
  }

  /* main loop */
  bool quit = false;
  while (quit == false) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (quit == true) {
        break;
      }
      //     SDL_Log("event: %d",e.type);
      switch (e.type) {
      case SDL_QUIT: exit(EXIT_SUCCESS); break;
      case SDL_WINDOWEVENT_SHOWN: SDL_Log("SDL_WINDOWEVENT_SHOWN"); break;
      case SDL_WINDOWEVENT_LEAVE: SDL_Log("SDL_WINDOWEVENT_LEAVE"); break;
      case SDL_WINDOWEVENT_ENTER: SDL_Log("SDL_WINDOWEVENT_ENTER"); break;
      case SDL_MOUSEMOTION: mu_input_mousemove(ctx, e.motion.x, e.motion.y); break;
      case SDL_MOUSEWHEEL: mu_input_scroll(ctx, 0, e.wheel.y * -30); break;
      case SDL_TEXTINPUT: mu_input_text(ctx, e.text.text); break;

      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: {
        int b = button_map[e.button.button & 0xff];
        if (b && e.type == SDL_MOUSEBUTTONDOWN) {
          mu_input_mousedown(ctx, e.button.x, e.button.y, b);
        }
        if (b && e.type == SDL_MOUSEBUTTONUP) {
          mu_input_mouseup(ctx, e.button.x, e.button.y, b);
        }
        break;
      }

      case SDL_KEYDOWN:
        fprintf(stderr, "keydown:%d..\n", e.key.keysym.sym);
        if (e.key.keysym.sym == SDLK_q) {
          fprintf(stderr, "quitting..\n");
          quit = true;
          break;
        }
        if (e.key.keysym.sym == SDLK_s) {
          fprintf(stderr, "screenshot..\n");
          //screenshot(renderer, "screenshot.bmp");
        }
      case SDL_KEYUP: {
        int c = key_map[e.key.keysym.sym & 0xff];
        if (c && e.type == SDL_KEYDOWN) {
          mu_input_keydown(ctx, c);
        }
        if (c && e.type == SDL_KEYUP) {
          mu_input_keyup(ctx, c);
        }
        break;
      }
      } /* switch */
    }
    process_frame(ctx);
    r_clear(mu_color(OUTER_BG[0], OUTER_BG[1], OUTER_BG[2], 255));
    mu_Command *cmd = NULL;
    while (mu_next_command(ctx, &cmd)) {
      switch (cmd->type) {
      case MU_COMMAND_TEXT:
        r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color);
        break;
      case MU_COMMAND_RECT:
        r_draw_rect(cmd->rect.rect, cmd->rect.color);
        break;
      case MU_COMMAND_ICON:
        r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color);
        break;
      case MU_COMMAND_CLIP:
        r_set_clip_rect(cmd->clip.rect);
        break;
      }
    }
    r_present();
  }
  SDL_LockMutex(windows_mutex);
  poll_windows_thread_active = false;
  SDL_UnlockMutex(windows_mutex);

  SDL_WaitThread(poll_windows_thread, &threadReturnValue);
  SDL_Log("Thread returned value: %d\n", threadReturnValue);
  SDL_DestroyMutex(windows_mutex);
  return(0);
} /* main */

int pid_pre(){
  int focused_pid = get_focused_pid();

  SDL_Log("found focused pid to be %d", focused_pid);
  return(focused_pid);
}

#endif
