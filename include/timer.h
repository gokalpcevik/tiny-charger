#pragma once

#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz CPU clock (using 8MHz internal oscillator with DIV8)
#endif

#include <util/delay.h>

typedef enum
{
    TC_PWM_NO_CONNECT = 0x0,
    // Clear OC1x on CM, Set OC1x bar on CM
    TC_PWM_CS_ON_COMPARE_MATCH = 0x1,
    // Clear OC1x on CM, do not connect OC1x bar
    TC_PWM_CNC_ON_COMPARE_MATCH = 0x2,
    // Set OC1x on CM, do not connect OC1x bar
    TC_PWM_SNC_ON_COMPARE_MATCH = 0x3
} tc_tim_pwm_mode;

typedef enum
{
    TC_PWM_A=0,
    TC_PWM_B=1
} tc_pwm_select;

void tc_enable_pll(void);

/// @brief Only call this function after calling tc_enable_pll() or you have started the PLL.
void tc_tim1_enable_async_mode(void);
void tc_tim1_set_OCRB(uint8_t val);
void tc_tim1_set_OCRC(uint8_t val);
void tc_tim1_gtcc_set_pwm_mode(tc_tim_pwm_mode mode);
void tc_tim1_enable_pwm(tc_pwm_select modulator);
void tc_tim1_disable_pwm(tc_pwm_select modulator);
void tc_tim1_select_prescaler(uint8_t prescaling_src);
void tc_tim1_enable_clear_timer_on_comp_match(void);
