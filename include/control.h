#pragma once
#include <stdint-gcc.h>

#define TC_VSOURCE_PIN PB2
#define TC_VOUT_PIN    PB3
#define TC_PWM_PIN     PB4
#define TC_OC1C 166U

// Is 'hysteresis' the correct term here?
#define TC_VSOURCE_HYSTERESIS_THRESHOLD 25UL
#define TC_VOUT_HYSTERESIS_THRESHOLD 10UL
#define TC_REEVALUATE_VOUT_IF_TIMER_CNT_GREATER 1000UL

#define TC_ADC_NUM_ITER 12UL
// Due to non idealities(such as forward drop voltage of the schottky, non-ideal transistor switching and switching losses...) 
// adding a bias to the calculated duty cycle adjusts the output voltage to be 5V.
#define TC_OUTPUT_DEFAULT_BIAS 24U

// This value represents the ADC result when Vout=5V
// It is found as 5/4.5/2.56*1023=444.01...
#define TC_CONVERTER_VREF 444UL

uint8_t tc_get_adjusted_bias(uint16_t Vsource,uint16_t Vfeedback);