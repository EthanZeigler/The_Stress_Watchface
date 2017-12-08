#include <pebble.h>

void update_time();

void tick_handler(struct tm *tick_time, TimeUnits units_changed);

void tick_handler(struct tm *tick_time, TimeUnits units_changed);

void main_window_load(Window *window);

void main_window_unload(Window *window);

void init();

void deinit();

int main();

void update_light_health_bar(Layer *layer, GContext *ctx);

void update_dark_health_bar(Layer *layer, GContext *ctx);