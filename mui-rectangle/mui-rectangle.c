#pragma once
#include "../mui-rectangle/mui-rectangle.h"
#include "b64.c/b64.h"

static struct mui_init_cfg_t CFG = {
  .title    = CFG_TITLE,
  .options  = CFG_OPTIONS,
  .x_offset = CFG_X_OFFSET,.y_offset  = CFG_Y_OFFSET,
  .width    = CFG_WIDTH,   .height    = BASIC_WINDOW_HEIGHT,
};
//////////////////////////////////////////////////////////////////////////
static int pid_pre();
static void callback(tmt_msg_t m, TMT *vt, void *EXEC, void *c);
static void printTerminal(TMT *vt, struct tmt_exec_t *exec);
//////////////////////////////////////////////////////////////////////////
struct rectangle_info_t {
  size_t                    rectangle_info_update_interval_ms;
  int                       display_width, todo_width, rectangle_pid;
  bool                      todo_enabled, poller_active;
  char                      *todo_app, *config, *buf, *title;
  unsigned long             last_update_ts;
  size_t                    updates_qty, update_dur_ms, label_width, value_width;
  struct StringFNStrings    config_lines;
  SDL_mutex                 *mutex;
  struct keycode_modifier_t *kcm;
};
static struct rectangle_info_t *rec = &(struct rectangle_info_t){
  .title                             = "Execution Info",
  .rectangle_info_update_interval_ms = 3000,
  .label_width                       = 90,
  .value_width                       = 55,
  .last_update_ts                    = 0,
  .updates_qty                       = 0,
  .update_dur_ms                     = 0,
  .poller_active                     = true,
  .mutex                             = NULL,
};

//////////////////////////////////////////////////////////////////////////
int tmt_exec(struct tmt_exec_t *exec){
  exec->started_ms    = timestamp();
  exec->output_lines  = vector_new();
  exec->output_buffer = stringbuffer_new();
  TMT *vt = tmt_open(exec->rows, exec->cols, callback, (void *)exec, NULL);
  assert(vt != NULL);
  tmt_reset(vt);
//  tmt_write(vt, "\x1b[0;0H", 0);         //Bring cursor to (0,0).
//  tmt_write(vt, "\x1b[2J\x1b[?25h", 0);  //Clear terminal virtual screen, show cursor
  tmt_write(vt, "\x1b[?25h", 0);  //Clear terminal virtual screen, show cursor
  tmt_write(vt, AC_RESETALL, 0);
  tmt_write(vt, exec->input, 0);
  tmt_write(vt, AC_RESETALL "\r\n", 0);
  tmt_close(vt);
  exec->dur_ms = timestamp() - exec->started_ms;
  fprintf(stderr, AC_RESETALL
          AC_REVERSED AC_BRIGHT_YELLOW AC_BOLD "%lu callbacks, %lu lines, %lu chars in %ldms\n" AC_RESETALL,
          exec->callbacks_qty,
          vector_size(exec->output_lines),
          strlen(stringbuffer_to_string(exec->output_buffer)),
          exec->dur_ms
          );
  fprintf(stderr, AC_RESETALL AC_REVERSED AC_BRIGHT_YELLOW AC_BOLD "cursor pos: %dx%d (%s)\n" AC_RESETALL, exec->cursor_pos_x, exec->cursor_pos_y, exec->cursor_state);
  printf("\n=====================================\n");
  printf(AC_RESETALL "%s" AC_RESETALL, stringbuffer_to_string(exec->output_buffer));
  printf("\n=====================================\n");
  return(EXIT_SUCCESS);
}

static void callback(tmt_msg_t m, TMT *vt, void *a, void *EXEC){
  struct tmt_exec_t *exec = (struct tmt_exec_t *)EXEC;

  exec->callbacks_qty++;
  const TMTSCREEN *s = tmt_screen(vt);
  const TMTPOINT  *c = tmt_cursor(vt);

  switch (m) {
  case TMT_MSG_CURSOR:
    exec->cursor_pos_y = (c->r);
    exec->cursor_pos_x = (c->c);
    if (((const char *)a)[0] == 't') {
      exec->cursor_state = "visible";
    } else{
      exec->cursor_state = "hidden";
    }
    break;

  case TMT_MSG_BELL:
    printf(
      AC_RESETALL AC_BRIGHT_RED "Bell Rang!\n" AC_RESETALL
      );
    break;

  case TMT_MSG_UPDATE:
    printTerminal(vt, exec);
    break;

  case TMT_MSG_ANSWER:
    printf("Terminal answered %s\n", (const char *)a);
    break;

  case TMT_MSG_MOVED:
    exec->cursor_pos_y = (c->r) + 1;
    exec->cursor_pos_x = (c->c) + 1;
    break;
  } /* switch */
}   /* callback */

static void printTerminal(TMT *vt, struct tmt_exec_t *exec){
  const TMTSCREEN     *s                = tmt_screen(vt);
  const TMTPOINT      *c                = tmt_cursor(vt);
  unsigned int        qty_cells_printed = 0;
  struct Vector       *lines            = vector_new();
  struct StringBuffer *terminal_buffer  = stringbuffer_new();

  for (size_t r = 0; r < s->nline; r++) {
    if (!s->lines[r]->dirty) {
      vector_push(lines, (char *)vector_get(exec->output_lines, r));
      stringbuffer_append_string(terminal_buffer, (char *)vector_get(exec->output_lines, r));
      stringbuffer_append_string(terminal_buffer, "\r\n");
      continue;
    }
    struct StringBuffer *row_sb = stringbuffer_new();
    for (size_t c = 0; c < s->ncol; c++) {
      stringbuffer_append_string(row_sb, AC_RESETALL);
      if (s->lines[r]->chars[c].a.fg > -1) {
        stringbuffer_append_string(row_sb, "\x1b[");
        stringbuffer_append_int(row_sb, ((int)(s->lines[r]->chars[c].a.fg) + 29));
        stringbuffer_append_string(row_sb, "m");
      }
      if (s->lines[r]->chars[c].a.bg > -1) {
        stringbuffer_append_string(row_sb, "\x1b[");
        stringbuffer_append_int(row_sb, ((int)(s->lines[r]->chars[c].a.bg) + 39));
        stringbuffer_append_string(row_sb, "m");
      }
      if (s->lines[r]->chars[c].a.reverse) {
        stringbuffer_append_string(row_sb, AC_INVERSE);
      }
      if (s->lines[r]->chars[c].a.dim) {
        stringbuffer_append_string(row_sb, AC_FAINT);
      }
      if (s->lines[r]->chars[c].a.underline) {
        stringbuffer_append_string(row_sb, AC_UNDERLINE);
      }
      if (s->lines[r]->chars[c].a.bold) {
        stringbuffer_append_string(row_sb, AC_BOLD);
      }
      stringbuffer_append(row_sb, s->lines[r]->chars[c].c);
      qty_cells_printed++;
    }
    vector_push(lines, (char *)stringbuffer_to_string(row_sb));
    stringbuffer_append_string(row_sb, "\r\n");
    stringbuffer_append_string(terminal_buffer, stringbuffer_to_string(row_sb));
    stringbuffer_release(row_sb);
  }
  printf(
    AC_RESETALL AC_BRIGHT_GREEN AC_REVERSED "%d cells printed, %lu lines" AC_RESETALL
    AC_RESETALL "\n" AC_RESETALL,
    qty_cells_printed,
    vector_size(lines)
    );
  vector_release(exec->output_lines);
  stringbuffer_release(exec->output_buffer);

  exec->output_lines  = lines;
  exec->output_buffer = terminal_buffer;
  tmt_clean(vt);
} /* printTerminal */
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
SDL_Thread    *poll_rec_thread;
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

void update_rectangle_info(bool FORCE_UPDATE){
  if ((FORCE_UPDATE == true) || rec->last_update_ts == 0 || ((timestamp() - rec->last_update_ts) > rec->rectangle_info_update_interval_ms)) {
    size_t started = timestamp();
    rec->updates_qty++;
    rec->display_width = get_display_width();
    rec->todo_width    = rectangle_get_todo_width();
    rec->rectangle_pid = rectangle_get_pid();
    rec->todo_app      = rectangle_get_todo_app();
    rec->todo_enabled  = rectangle_get_todo_mode_enabled();
    if ((FORCE_UPDATE) || rec->last_update_ts == 0 || ((timestamp() - rec->last_update_ts) > rec->rectangle_info_update_interval_ms * 3)) {
      rec->kcm          = rectangle_get_todo_keys();
      rec->config       = rectangle_get_config();
      rec->config_lines = stringfn_split_lines_and_trim(rec->config);
    }
    char *input = ""
                  "" AC_RESETALL ""
                  AC_BLACK_WHITE "========================================" AC_RESETALL "\r\n"
                  AC_BOLD AC_YELLOW_BLACK " yellow " AC_RESETALL
                  AC_UNDERLINE AC_RED_WHITE " red " AC_RESETALL
                  AC_ITALIC AC_GREEN_RED " green " AC_RESETALL
                  AC_INVERSE AC_CYAN_RED " cyan " AC_RESETALL
                  AC_FAINT AC_MAGENTA_YELLOW " magenta " AC_RESETALL
                  "\r\n" AC_WHITE_BLACK "----------------------------------------" AC_RESETALL "\r\n"
                  "";

    tmt_exec(&(struct tmt_exec_t){
      .input = input,
      .rows  = 5,
      .cols  = 80,
    });
    rec->last_update_ts = timestamp();
    rec->update_dur_ms  = (size_t)(rec->last_update_ts - started);
  }
}

int update_rectangle_info_thread(void *PARAM){
  SDL_LockMutex(rec->mutex);
  bool active = rec->poller_active;
  SDL_UnlockMutex(rec->mutex);
  while (active == true) {
    SDL_LockMutex(rec->mutex);
    update_rectangle_info(false);
    size_t dur = rec->rectangle_info_update_interval_ms - rec->update_dur_ms;
    SDL_UnlockMutex(rec->mutex);
    usleep(1000 * dur);
    SDL_LockMutex(rec->mutex);
    active = rec->poller_active;
    SDL_UnlockMutex(rec->mutex);
  }
  return(0);
}

static void rectangle_state_window(mu_Context *ctx) {
  if (mu_begin_window_ex(ctx, "Rectangle State", mu_rect(0, 0, CFG.width, BASIC_WINDOW_HEIGHT), BASIC_WINDOW_OPTIONS)) {
    SDL_LockMutex(rec->mutex);
    {
      asprintf(&rec->buf, "%s", rec->title);
      if (mu_header_ex(ctx, rec->buf, MU_OPT_NODRAG | MU_OPT_EXPANDED)) {
        mu_layout_row(ctx,
                      8,
                      (int[]) {
          rec->label_width, rec->value_width,
          rec->label_width, rec->value_width,
          rec->label_width, rec->value_width,
          rec->label_width, rec->value_width,
        },
                      0);

        mu_label(ctx, "PID:");
        asprintf(&rec->buf, "%d", rec->rectangle_pid); mu_label(ctx, rec->buf); free(rec->buf);

        mu_label(ctx, "Todo Enabled:");
        asprintf(&rec->buf, "%s", (rec->todo_enabled == true) ? "Yes" : "No"); mu_label(ctx, rec->buf); free(rec->buf);

        mu_label(ctx, "Todo App:");
        asprintf(&rec->buf, "%s", rec->todo_app); mu_label(ctx, rec->buf); free(rec->buf);

        mu_label(ctx, "Todo Width:");
        asprintf(&rec->buf, "%dpx", rec->todo_width); mu_label(ctx, rec->buf); free(rec->buf);

        mu_label(ctx, "Update Duration:");
        asprintf(&rec->buf, "%s", milliseconds_to_string(rec->update_dur_ms)); mu_label(ctx, rec->buf); free(rec->buf);

        mu_label(ctx, "Display Width:");
        asprintf(&rec->buf, "%dpx", rec->display_width); mu_label(ctx, rec->buf); free(rec->buf);

        mu_label(ctx, "Config Size:");
        asprintf(&rec->buf, "%s", bytes_to_string(strlen(rec->config))); mu_label(ctx, rec->buf); free(rec->buf);

        mu_label(ctx, "Config Lines:");
        asprintf(&rec->buf, "%d", rec->config_lines.count); mu_label(ctx, rec->buf); free(rec->buf);

        mu_label(ctx, "Update Interval:");
        asprintf(&rec->buf, "%s", milliseconds_to_string(rec->rectangle_info_update_interval_ms)); mu_label(ctx, rec->buf); free(rec->buf);

        mu_label(ctx, "Todo Keys:");
        asprintf(&rec->buf, "%s", rec->kcm->keys); mu_label(ctx, rec->buf); free(rec->buf);
      }
    }
    SDL_UnlockMutex(rec->mutex);

    if (mu_header_ex(ctx, "Controller", MU_OPT_NODRAG | MU_OPT_EXPANDED)) {
      mu_layout_row(ctx, 3, (int[]) { 100, 100, -1 }, 0);
      mu_label(ctx, "Test buttons 1:");
      if (mu_button(ctx, "Popup")) {
        mu_open_popup(ctx, "Popup");
      }
      if (mu_begin_popup(ctx, "Popup")) {
        if (mu_button_ex(ctx, "Hello", 0, MU_OPT_ALIGNRIGHT)) {
          SDL_Log("popup 1");
        }
        if (mu_button_ex(ctx, "World", 0, MU_OPT_ALIGNRIGHT)) {
          SDL_Log("popup 2");
        }
        mu_end_popup(ctx);
      }
      if (mu_button(ctx, "Button 1")) {
        SDL_Log("Pressed button 1");
      }
    }
    mu_end_window(ctx);
  }
} /* rectangle_state_window */

static void rectangle_windows_window(mu_Context *ctx) {
  if (mu_begin_window_ex(ctx, "Windows", mu_rect(0, 0, CFG.width, BASIC_WINDOW_HEIGHT), BASIC_WINDOW_OPTIONS)) {
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
  rectangle_state_window(ctx);
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

static int  orig_focused_pid     = -1;
static bool orig_focused_pid_set = false;

int mui_rectangle(){
  windows_mutex = SDL_CreateMutex();
  int threadReturnValue = -1;
  if (orig_focused_pid == -1) {
    orig_focused_pid = pid_pre();
  }
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

  if (rec->mutex == NULL) {
    rec->mutex = SDL_CreateMutex();
  }
  poll_rec_thread = SDL_CreateThread(update_rectangle_info_thread, "PollRectangle", (void *)NULL);
  if (NULL == poll_rec_thread) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateThread update_rectangle_info_thread failed: %s\n", SDL_GetError());
  } else {
    SDL_Log("Thread poll rec info created");
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
    if (orig_focused_pid_set == false) {
      orig_focused_pid_set = true;
      set_focused_pid(orig_focused_pid);
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
  is_authorized_for_accessibility();
  int focused_pid = get_focused_pid();
  SDL_Log("found focused pid to be %d", focused_pid);
  return(focused_pid);
}
