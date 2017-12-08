#include "The Stress Watchface.h"

static Window* s_main_window;
static TextLayer *s_time_layer;

// shape layers
static Layer* dark_bar_layer;
static Layer* light_bar_layer;


static GFont s_time_font;


void update_time() {
    // Get a tm structure
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    // Write the current hours and minutes into a buffer
    static char s_buffer[8];
    strftime(s_buffer, sizeof(s_buffer),
            clock_is_24h_style() ? "%k:%M" : "%k:%M", tick_time);

    // Display this time on the TextLayer
    text_layer_set_text(s_time_layer, s_buffer);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

void main_window_load(Window *window) {
    // Get information about window_create
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    // Create the TextLayer with specific bounds
    s_time_layer = text_layer_create(GRect(0, 52, bounds.size.w, 50));

    // Improve the layout to me more like a Watchface
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorBlack);
    text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

    // Initialize shapes
    layer_set_update_proc(light_bar_layer, update_light_health_bar);
    layer_set_update_proc(dark_bar_layer, update_dark_health_bar);
    layer_mark_dirty(light_bar_layer);
    layer_mark_dirty(dark_bar_layer);


    // Add it as a child layer to the Window's root layer
    layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

    update_time();

}

void main_window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_time_layer);
    layer_destroy(dark_bar_layer);
    layer_destroy(light_bar_layer);
}


void init() {
    // Create main window
    s_main_window = window_create();

    // Set handlers to manage the elements inside the window
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });

    // show the window on the watch, load w/o animation
    window_stack_push(s_main_window, false);
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

void deinit() {
    window_destroy(s_main_window);
}

int main() {
    APP_LOG(APP_LOG_LEVEL_INFO, "Loaded application");
    init();
    app_event_loop();
    deinit();
}

void update_light_health_bar(Layer *layer, GContext *ctx) {
    graphics_context_set_stroke_color(ctx, GColorLightGray);
    graphics_context_set_fill_color(ctx, GColorLightGray);

    GRect rect_bounds = GRect(10, 20, 10, 20);
    int corner_radius = 0;
    graphics_fill_rect(ctx, rect_bounds, corner_radius, GCornersAll);
}

void update_dark_health_bar(Layer *layer, GContext *ctx) {
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_context_set_fill_color(ctx, GColorBlack);


    GRect rect_bounds = GRect(50, 20, 10, 20);
    int corner_radius = 0;
    graphics_fill_rect(ctx, rect_bounds, corner_radius, GCornersAll);
}
