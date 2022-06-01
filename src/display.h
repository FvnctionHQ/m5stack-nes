#pragma once

#include <Arduino.h>

extern void display_begin();
extern "C" void display_init();
extern "C" void display_write_frame(const uint8_t *data[]);
extern "C" void display_clear();