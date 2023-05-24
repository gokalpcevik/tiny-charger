#pragma once

#include <avr/io.h>

typedef uint8_t tc_port_t;
typedef uint8_t tc_pin_t;

typedef enum
{
    TC_PIN_DIR_OUTPUT = 0x1,
    TC_PIN_DIR_INPUT = 0x0
} tc_dir_t;

typedef enum
{
    TC_HIGH = 1,
    TC_LOW = 0
} tc_pin_level_t;

void tc_set_pin_dir(tc_pin_t pin, tc_dir_t dir);
void tc_toggle_pin(tc_pin_t pin);
void tc_write_pin(tc_pin_t pin,tc_pin_level_t pin_level);
tc_pin_level_t tc_read_pin(tc_pin_t pin);