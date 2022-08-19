#include "mui-colors.h"
#define MAX_COLORS      1000
#define DEBUG_COLORS    false
//////////////////////////////////////////////////////////////////////////
typedef struct {
  int red, green, blue;
} color_rgb_t;
//////////////////////////////////////////////////////////////////////////
color_rgb_t get_color_name_rgb(const char *COLOR_NAME);
int pid_pre();
int pid_post(int);
int load_colors_hash(ColorsDB *DB);
void iterate_colors_hash();
void iterate_color_name_strings();
void iterate_color_hex_strings();
int load_color_names();
char * get_color_hex_name(const char *COLOR_HEX);
char * get_color_name_hex(const char *COLOR_NAME);
static char * get_color_name_row(const char *COLOR_NAME);
static void update_cur_color(const char *COLOR_NAME);
static color_rgb_t get_color_name_rgb_background(const char *COLOR_NAME);
static void *get_color_name_row_property(const char *COLOR_NAME, const char *ROW_PROPERTY);

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
static char            CUR_COLOR_HEX[32], CUR_COLOR_ROW[2048], CUR_COLOR_NAME[32] = "";
static color_rgb_t     CUR_COLOR_RGB    = { 0, 0, 0 };
static color_rgb_t     CUR_COLOR_RGB_BG = { 0, 0, 0 };
static char            logbuf[64000];
static int             logbuf_updated = 0;
static float           bg[3]          = { WINDOW_BACKGROUND_RED, WINDOW_BACKGROUND_GREEN, WINDOW_BACKGROUND_BLUE };
static float           bg_text[3]     = { WINDOW_BACKGROUND_RED, WINDOW_BACKGROUND_GREEN, WINDOW_BACKGROUND_BLUE };
static float           OUTER_BG[3]    = { WINDOW_BACKGROUND_RED, WINDOW_BACKGROUND_GREEN, WINDOW_BACKGROUND_BLUE };
volatile int           set_focus_qty  = 0;
static size_t          colors_per_row = 3;
//////////////////////////////////////////////////////////////////////////
ColorsDB               *DB;
struct djbhash         COLORS_HASH = { 0 }, COLOR_NAME_HASH = { 0 }, COLOR_HEX_HASH = { 0 };
struct StringFNStrings COLOR_NAME_STRINGS, COLOR_HEX_STRINGS;
//////////////////////////////////////////////////////////////////////////

static void write_log(const char *text) {
  if (logbuf[0]) {
    strcat(logbuf, "\n");
  }
  strcat(logbuf, text);
  logbuf_updated = 1;
}

static void test_window(mu_Context *ctx) {
  if (mu_begin_window_ex(ctx, "Color Info", mu_rect(10, 190, WINDOW_WIDTH, 200), MU_OPT_NODRAG)) {
    mu_Container *win = mu_get_current_container(ctx);
    win->rect.w = mu_max(win->rect.w, 240);
    win->rect.h = mu_max(win->rect.h, 230);

    if (mu_header_ex(ctx, CUR_COLOR_NAME, MU_OPT_EXPANDED)) {
      mu_Container *win = mu_get_current_container(ctx);
      char         buf[64];
      mu_layout_row(ctx, 4, (int[]) { 110, 160, 110, -1 }, 0);

      mu_label(ctx, "Name:");
      sprintf(buf, "%s", CUR_COLOR_NAME); mu_label(ctx, buf);

      mu_label(ctx, "Hex:");
      sprintf(buf, "%s", (char *)get_color_name_row_property(CUR_COLOR_NAME, "hex")); mu_label(ctx, buf);

      mu_label(ctx, "Ansi Code:");
      sprintf(buf, "%lu", (size_t)get_color_name_row_property(CUR_COLOR_NAME, "ansicode")); mu_label(ctx, buf);

      mu_label(ctx, "RGB Total:");
      sprintf(buf, "%lu",
              (size_t)get_color_name_row_property(CUR_COLOR_NAME, "rgb.red")
              + (size_t)get_color_name_row_property(CUR_COLOR_NAME, "rgb.green")
              + (size_t)get_color_name_row_property(CUR_COLOR_NAME, "rgb.blue")
              ); mu_label(ctx, buf);

      mu_label(ctx, "RGB Background:");
      sprintf(buf, "%d/%d/%d",
              CUR_COLOR_RGB_BG.red,
              CUR_COLOR_RGB_BG.green,
              CUR_COLOR_RGB_BG.blue
              ); mu_label(ctx, buf);
    }

    if (mu_header_ex(ctx, "RGB", MU_OPT_EXPANDED)) {
      mu_layout_row(ctx, 2, (int[]) { -78, -1 }, 74);
      mu_layout_begin_column(ctx);
      mu_layout_row(ctx, 2, (int[]) { 46, -1 }, 0);
      mu_label(ctx, "Red:");   mu_slider(ctx, &bg[0], 0, 255);
      mu_label(ctx, "Green:"); mu_slider(ctx, &bg[1], 0, 255);
      mu_label(ctx, "Blue:");  mu_slider(ctx, &bg[2], 0, 255);
      mu_layout_end_column(ctx);
      mu_Rect r = mu_layout_next(ctx);
      mu_draw_rect(ctx, r, mu_color(
                     bg[0],
                     bg[1],
                     bg[2],
                     255
                     )
                   );
      char buf[32];
      sprintf(buf, "#%02X%02X%02X", (int)bg_text[0], (int)bg_text[1], (int)bg_text[2]);
      mu_draw_control_text(ctx, buf, r, MU_COLOR_TEXT, MU_OPT_ALIGNCENTER);
    }

    /* tree */

    mu_end_window(ctx);
  }
} /* test_window */

static void colors_window(mu_Context *ctx) {
  if (mu_begin_window_ex(ctx, "Colors", mu_rect(10, 10, WINDOW_WIDTH, 175), MU_OPT_NODRAG)) {
    mu_Container *win = mu_get_current_container(ctx);
    win->rect.w = mu_max(win->rect.w, 240);
    win->rect.h = mu_max(win->rect.h, 100);
    size_t best_qty = 3000, recent_qty = 25, all_qty = 10000;
    if (mu_header_ex(ctx, "Colors", MU_OPT_EXPANDED)) {
      for (size_t i = 0; i < COLOR_NAME_STRINGS.count; i++) {
        if ((i % colors_per_row) == 0) {
          mu_layout_row(ctx, colors_per_row, (int[]) {
            WINDOW_WIDTH / colors_per_row - colors_per_row - 3,
            WINDOW_WIDTH / colors_per_row - colors_per_row - 3,
            WINDOW_WIDTH / colors_per_row - colors_per_row - 3,
          }, 0);
        }
        char color_name[strlen(COLOR_NAME_STRINGS.strings[i]) + 1];
        char color_msg[strlen(color_name) + 128];
        sprintf(color_name, "%s", COLOR_NAME_STRINGS.strings[i]);
        sprintf(color_msg, "Loaded Hex '%s'", get_color_name_hex(color_name));
        if (mu_button(ctx, color_name)) {
          update_cur_color(color_name);
          write_log(color_msg);
        }
      }
    }
    mu_end_window(ctx);
  }
} /* colors_window */

static color_rgb_t get_color_name_rgb_background(const char *COLOR_NAME){
  color_rgb_t bg_color = {
    255,
    255,
    255,
  };
  color_rgb_t rgb = get_color_name_rgb(COLOR_NAME);

  if (rgb.green > 160) {
    bg_color.red   = 0;
    bg_color.green = 0;
    bg_color.blue  = 0;
  }
  return(bg_color);
}

static void update_cur_color(const char *COLOR_NAME){
  sprintf(CUR_COLOR_NAME, "%s", COLOR_NAME);
  sprintf(CUR_COLOR_HEX, "%s", get_color_name_hex(COLOR_NAME));
  sprintf(CUR_COLOR_ROW, "%s", get_color_name_row(COLOR_NAME));
  CUR_COLOR_RGB    = get_color_name_rgb(COLOR_NAME);
  CUR_COLOR_RGB_BG = get_color_name_rgb_background(COLOR_NAME);
  bg[0]            = CUR_COLOR_RGB.red;
  bg[1]            = CUR_COLOR_RGB.green;
  bg[2]            = CUR_COLOR_RGB.blue;
  fprintf(stderr, "updated color to %s- |%s|%d/%d/%d|%s|\n",
          CUR_COLOR_NAME,
          CUR_COLOR_HEX,
          CUR_COLOR_RGB.red, CUR_COLOR_RGB.green, CUR_COLOR_RGB.blue,
          CUR_COLOR_ROW
          );
}

static void log_window(mu_Context *ctx) {
  if (mu_begin_window_ex(ctx, "Log", mu_rect(10, 425, WINDOW_WIDTH, 150), MU_OPT_NODRAG)) {
    /* output text panel */
    mu_layout_row(ctx, 1, (int[]) { -1 }, -25);
    mu_begin_panel(ctx, "Log Output");
    mu_Container *panel = mu_get_current_container(ctx);
    mu_layout_row(ctx, 1, (int[]) { -1 }, -1);
    mu_text(ctx, logbuf);
    mu_end_panel(ctx);
    if (logbuf_updated) {
      panel->scroll.y = panel->content_size.y;
      logbuf_updated  = 0;
    }

    /* input textbox + submit button */
    static char buf[128];
    int         submitted = 0;
    mu_layout_row(ctx, 2, (int[]) { -70, -1 }, 0);
    if (mu_textbox(ctx, buf, sizeof(buf)) & MU_RES_SUBMIT) {
      mu_set_focus(ctx, ctx->last_id);
      submitted = 1;
    }
    if (mu_button(ctx, "Submit")) {
      submitted = 1;
    }
    if (submitted) {
      write_log(buf);
      buf[0] = '\0';
    }

    mu_end_window(ctx);
  }
}

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
  static struct { const char *label; int idx; } colors[] = {
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

  if (mu_begin_window_ex(ctx, "Style Editor", mu_rect(320, 10, 300, 405), MU_OPT_NODRAG)) {
    int sw = mu_get_current_container(ctx)->body.w * 0.14;
    mu_layout_row(ctx, 6, (int[]) { 80, sw, sw, sw, sw, -1 }, 0);
    for (int i = 0; colors[i].label; i++) {
      mu_label(ctx, colors[i].label);
      uint8_slider(ctx, &ctx->style->colors[i].r, 0, 255);
      uint8_slider(ctx, &ctx->style->colors[i].g, 0, 255);
      uint8_slider(ctx, &ctx->style->colors[i].b, 0, 255);
      uint8_slider(ctx, &ctx->style->colors[i].a, 0, 255);
      mu_draw_rect(ctx, mu_layout_next(ctx), ctx->style->colors[i]);
    }
    mu_end_window(ctx);
  }
}

static void process_frame(mu_Context *ctx) {
  mu_begin(ctx);
  colors_window(ctx);
  log_window(ctx);
  test_window(ctx);
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

static int text_width(mu_Font font, const char *text, int len) {
  if (len == -1) {
    len = strlen(text);
  }
  return(r_get_text_width(text, len));
}

static int text_height(mu_Font font) {
  return(r_get_text_height());
}

int pid_post(int pid){
  printf("setting focused process to pid %d.....\n", pid);
  bool ok = set_focused_pid(pid);
  printf("set ok:%d\n", ok);
  return(ok);
}

int mui_colors(){
  int focused_pid = pid_pre();

  /* init SDL and renderer */
  SDL_Init(SDL_INIT_EVERYTHING);
  r_init(0, 0, 0);

  /* init microui */
  mu_Context *ctx = malloc(sizeof(mu_Context));

  mu_init(ctx);
  ctx->text_width  = text_width;
  ctx->text_height = text_height;

  /* main loop */
  for ( ;;) {
    /* handle SDL events */
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT: exit(EXIT_SUCCESS); break;
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
      }
    }
    if (set_focus_qty < 1) {
      pid_post(focused_pid);
      set_focus_qty++;
    }

    /* process frame */
    process_frame(ctx);

    /* render */
    r_clear(mu_color(OUTER_BG[0], OUTER_BG[1], OUTER_BG[2], 255));
    mu_Command *cmd = NULL;
    while (mu_next_command(ctx, &cmd)) {
      switch (cmd->type) {
      case MU_COMMAND_TEXT: r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color); break;
      case MU_COMMAND_RECT: r_draw_rect(cmd->rect.rect, cmd->rect.color); break;
      case MU_COMMAND_ICON: r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color); break;
      case MU_COMMAND_CLIP: r_set_clip_rect(cmd->clip.rect); break;
      }
    }
    r_present();
  }

  return(0);
} /* main */

int pid_pre(){
  int focused_pid = get_focused_pid();

  fprintf(stderr, "found focused pid to be %d.....\n", focused_pid);
  DB = malloc(sizeof(ColorsDB));
  char *Path = malloc(strlen(COLOR_NAMES_DB_PATH));

  sprintf(Path, "%s", COLOR_NAMES_DB_PATH);
  DB->Path = strdup(Path);
  free(Path);

  if (init_colors_db(DB) == 0) {
    int colors_qty = load_colors_hash(DB);
    printf("loaded %d colors to hash\n", colors_qty);
    load_color_names();

    printf("loaded %d color names\n", COLOR_NAME_STRINGS.count);
    iterate_color_name_strings();

    printf("loaded %d color hexes\n", COLOR_HEX_STRINGS.count);
    iterate_color_hex_strings();
  }

  return(focused_pid);
}

color_rgb_t get_color_name_rgb(const char *COLOR_NAME){
  struct djbhash_node *HASH_ITEM;
  color_rgb_t         color_rgb  = { 0, 0, 0 };
  char                *color_row = get_color_name_row(COLOR_NAME);

  if (color_row == NULL) {
    return(color_rgb);
  }
  JSON_Value  *ColorLine;
  JSON_Object *ColorObject;

  ColorLine       = json_parse_string(color_row);
  ColorObject     = json_value_get_object(ColorLine);
  color_rgb.red   = json_object_dotget_number(ColorObject, "rgb.red");
  color_rgb.green = json_object_dotget_number(ColorObject, "rgb.green");
  color_rgb.blue  = json_object_dotget_number(ColorObject, "rgb.blue");
  if (ColorLine) {
    json_value_free(ColorLine);
  }
  return(color_rgb);
}

char * get_color_hex_name(const char *COLOR_HEX){
  struct djbhash_node *HASH_ITEM;

  HASH_ITEM = djbhash_find(&COLOR_HEX_HASH, (char *)COLOR_HEX);
  if (HASH_ITEM == NULL) {
    return(NULL);
  }
  return((char *)((HASH_ITEM)->value));
}

char * get_color_name_hex(const char *COLOR_NAME){
  struct djbhash_node *HASH_ITEM;

  HASH_ITEM = djbhash_find(&COLOR_NAME_HASH, (char *)COLOR_NAME);
  if (HASH_ITEM == NULL) {
    return(NULL);
  }
  return((char *)((HASH_ITEM)->value));
}

static void * get_color_name_row_property(const char *COLOR_NAME, const char *ROW_PROPERTY){
  void        *res = NULL;
  JSON_Value  *V;
  JSON_Value  *ROW = json_parse_string(get_color_name_row(COLOR_NAME));
  JSON_Object *O   = json_value_get_object(ROW);

  V = json_object_dotget_value(O, ROW_PROPERTY);
  switch (json_value_get_type(V)) {
  case JSONString:
    res = (void *)((char *)json_value_get_string(V));
    break;
  case JSONBoolean:
    res = (void *)((size_t)json_value_get_boolean(V));
    break;
  case JSONNumber:
    res = (void *)((size_t)json_value_get_number(V));
    break;
  }
  return((void *)res);
}

static char * get_color_name_row(const char *COLOR_NAME){
  struct djbhash_node *HASH_ITEM;

  HASH_ITEM = djbhash_find(&COLORS_HASH, (char *)COLOR_NAME);
  if (HASH_ITEM == NULL) {
    return(NULL);
  }
  return((char *)((HASH_ITEM)->value));
}

int load_color_names(){
  struct StringBuffer *NAME_STRINGS = stringbuffer_new();
  struct StringBuffer *HEX_STRINGS  = stringbuffer_new();
  struct djbhash_node *item         = djbhash_iterate(&COLORS_HASH);
  JSON_Value          *ColorLine;
  JSON_Object         *ColorObject;
  size_t              qty = 0;

  while (item) {
    char *row_data = (char *)((item)->value);
    if (strlen(row_data) > 0) {
      if (DEBUG_COLORS) {
        fprintf(stderr, "\t  - " AC_RESETALL AC_GREEN AC_ITALIC "#%lu> %s" AC_RESETALL "\n", qty, row_data);
      }
      ColorLine   = json_parse_string(row_data);
      ColorObject = json_value_get_object(ColorLine);
      const char *color_name = json_object_get_string(ColorObject, "name");
      const char *hex_string = json_object_get_string(ColorObject, "hex");
      stringbuffer_append_string(NAME_STRINGS, stringfn_trim(color_name));
      stringbuffer_append_string(NAME_STRINGS, "\n");
      stringbuffer_append_string(HEX_STRINGS, stringfn_trim(hex_string));
      stringbuffer_append_string(HEX_STRINGS, "\n");
      item = djbhash_iterate(&COLORS_HASH);
      qty++;
    }
  }
  COLOR_NAME_STRINGS = stringfn_split_lines(stringfn_trim(stringbuffer_to_string(NAME_STRINGS)));
  COLOR_HEX_STRINGS  = stringfn_split_lines(stringfn_trim(stringbuffer_to_string(HEX_STRINGS)));
  stringbuffer_release(NAME_STRINGS);
  stringbuffer_release(HEX_STRINGS);
  if (ColorLine) {
    json_value_free(ColorLine);
  }
  return(qty);
}

void iterate_color_hex_strings(){
  for (size_t i = 0; i < COLOR_HEX_STRINGS.count; i++) {
    char *color_name = get_color_hex_name(COLOR_HEX_STRINGS.strings[i]);
    char *color_row  = get_color_name_row(color_name);
    if (DEBUG_COLORS) {
      fprintf(stderr, "\t  - " AC_RESETALL AC_CYAN AC_ITALIC "#%lu> %s|%s- %s" AC_RESETALL "\n",
              i,
              COLOR_HEX_STRINGS.strings[i],
              color_name,
              color_row
              );
    }
  }
}

void iterate_color_name_strings(){
  for (size_t i = 0; i < COLOR_NAME_STRINGS.count; i++) {
    char *color_hex = get_color_name_hex(COLOR_NAME_STRINGS.strings[i]);
    char *color_row = get_color_name_row(COLOR_NAME_STRINGS.strings[i]);
    if (DEBUG_COLORS) {
      fprintf(stderr, "\t  - " AC_RESETALL AC_CYAN AC_ITALIC "#%lu> %s|%s- %s" AC_RESETALL "\n",
              i,
              COLOR_NAME_STRINGS.strings[i],
              color_hex,
              color_row
              );
    }
  }
}

void iterate_colors_hash(){
  for (size_t i = 0; i < COLOR_HEX_STRINGS.count; i++) {
    fprintf(stderr, "\t  - " AC_RESETALL AC_CYAN AC_ITALIC "#%lu> %s" AC_RESETALL "\n", i, COLOR_HEX_STRINGS.strings[i]);
  }
}

int load_colors_hash(){
  struct djbhash_node *HASH_ITEM;

  djbhash_init(&COLORS_HASH);
  djbhash_init(&COLOR_NAME_HASH);
  djbhash_init(&COLOR_HEX_HASH);
  int    qty;
  size_t total_ids = 0, unique_typeids_qty = 0, typeid_qty = 0, type_ids_size = 0, type_ids_qty = 0, unique_typeids_size = 0;
  char   *unique_typeids = (char *)colordb_get_distinct_typeids(DB->db, &unique_typeids_size, &unique_typeids_qty);

  fprintf(stderr, "read %lu bytes from %lu items\n", unique_typeids_size, unique_typeids_qty);
  size_t      row_type_ids_size = 0, row_type_ids_qty = 0, row_data_size = 0;
  JSON_Value  *ColorLine;
  JSON_Object *ColorObject;

  for (size_t processed_items = 0; (processed_items < unique_typeids_qty) && (qty <= MAX_COLORS); ) {
    if ((unique_typeids != NULL) && strlen(unique_typeids) > 0) {
      colordb_type row_typeid = atoi(unique_typeids);
      row_data_size = 0;
      char         *id_type_ids = (char *)colordb_get_typeid_ids(DB->db, row_typeid, &row_type_ids_size, &row_type_ids_qty);
      for (size_t _processed_items = 0; (_processed_items < row_type_ids_qty) && (_processed_items < MAX_COLORS); ) {
        if (strlen(id_type_ids) > 0) {
          int row_id = atoi(id_type_ids);
          if (row_id >= 0) {
            size_t row_data_size = 0;
            char   *row_data     = colordb_get(DB->db, row_id, &row_data_size);
            ColorLine   = json_parse_string(row_data);
            ColorObject = json_value_get_object(ColorLine);
            const char *color_name = json_object_get_string(ColorObject, "name");
            const char *color_hex  = json_object_get_string(ColorObject, "hex");
            if ((HASH_ITEM = djbhash_find(&COLORS_HASH, (char *)color_name)) == NULL) {
              if (strcmp(CUR_COLOR_NAME, "") == 0) {
                update_cur_color(color_name);
              }
              djbhash_set(&COLORS_HASH, (char *)color_name, (void *)row_data, DJBHASH_OTHER);
              djbhash_set(&COLOR_NAME_HASH, (char *)color_name, (char *)color_hex, DJBHASH_STRING);
              djbhash_set(&COLOR_HEX_HASH, (char *)color_hex, (char *)color_name, DJBHASH_STRING);
              qty++;
            }
          }
          id_type_ids += strlen(id_type_ids);
          _processed_items++;
        }else{
          id_type_ids++;
        }
      }
      unique_typeids += strlen(unique_typeids);
      processed_items++;
    }else{
      unique_typeids++;
    }
  }
  djbhash_reset_iterator(&COLORS_HASH);
  djbhash_reset_iterator(&COLOR_NAME_HASH);
  djbhash_reset_iterator(&COLOR_NAME_HASH);
  if (ColorLine) {
    json_value_free(ColorLine);
  }
  return(qty);
} /* load_colors_hash */
