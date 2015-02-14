#include <pebble.h>
#include <pebble_fonts.h>

static Window *window;
static TextLayer *text_layer;
static BitmapLayer *bitmap_layer;
static GBitmap *background;
#define POSSIBLE_ANSWERS 20
const char * answers[POSSIBLE_ANSWERS] = {
  /* Positive */
  "It is certain",
  "It is decidedly so",
  "Without a doubt",
  "Yes definitely",
  "You may rely on it",
  "As I see it, yes",
  "Most likely",
  "Outlook good",
  "Yes",
  "Signs point to yes",
  /* Maybe */
  "Reply hazy try again",
  "Ask again later",
  "Better not tell you now",
  "Cannot predict now",
  "Concentrate and ask again",
  /* No */
  "Don't count on it",
  "My reply is no",
  "My sources say no",
  "Outlook not so good",
  "Very doubtful"
};

static void set_8ball_answer() {
  vibes_short_pulse();
  text_layer_set_text(text_layer, answers[rand() % POSSIBLE_ANSWERS]);
}


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  set_8ball_answer();
}

/*static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
}*/

/*static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
}*/

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  //window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  //window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 60 }, .size = { bounds.size.w, 48 } });
  bitmap_layer = bitmap_layer_create(GRect(0,0, bounds.size.w, bounds.size.h)); //(GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, bounds.size.h } });
  background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);

  bitmap_layer_set_bitmap(bitmap_layer, background);
  set_8ball_answer();

  text_layer_set_background_color(text_layer, GColorWhite);
  text_layer_set_font(text_layer, fonts_get_system_font("RESOURCE_ID_GOTHIC_24"));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(text_layer, GTextOverflowModeWordWrap);

  //text_layer_set_font(text_layer, resource_get_handle(RESOURCE_ID_GOTHIC_18));

  layer_add_child(window_layer, bitmap_layer_get_layer(bitmap_layer));
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  // Process tap on ACCEL_AXIS_X, ACCEL_AXIS_Y or ACCEL_AXIS_Z
  // Direction is 1 or -1
  set_8ball_answer();
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
  bitmap_layer_destroy(bitmap_layer);
}

static void init(void) {
  srand(time(NULL));
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  accel_tap_service_subscribe(&accel_tap_handler);

  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
  gbitmap_destroy(background);
  accel_tap_service_unsubscribe();

}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
  return 0;
}
