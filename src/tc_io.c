#include "tc_io.h"

void tc_set_pin_dir(tc_pin_t pin, tc_dir_t dir)
{
    DDRB ^= (-(uint8_t)dir ^ DDRB) & (1UL << pin);
}

void tc_toggle_pin(tc_pin_t pin)
{
    PINB |= (1 << pin);
}

void tc_write_pin(tc_pin_t pin, tc_pin_level_t pin_level)
{
    PORTB ^= (-(uint8_t)pin_level^ PORTB) & (1UL << pin);
}

tc_pin_level_t tc_read_pin(tc_pin_t pin)
{
    return PORTB & (1 << pin);
}
