#pragma once

/*
    *** Voltage Reference ***
    - The voltage reference for the ADC may be selected by writing to the REFS[2:0] bits in ADMUX. The VCC supply,
    the AREF pin or an internal 1.1V / 2.56V voltage reference may be selected as the ADC voltage reference. 
    
    - Optionally the internal 2.56V voltage reference may be decoupled by an external capacitor at the AREF pin to improve
    noise immunity.

    *** Input Channel (For single-ended use) ***
    - The analog input channel and differential gain are selected by writing to the MUX[3:0] bits in ADMUX.
    Any of the four ADC input pins ADC[3:0] can be selected as single ended inputs to the ADC.
    
    -  If single ended channels are used, the gain amplifier is bypassed altogether.

    *** ADC ***
    - The ADC is enabled by setting the ADC Enable bit, ADEN in ADCSRA. Voltage reference and input channel selections will not go into effect until ADEN is set. The ADC does not consume power when ADEN is cleared, so it is
    recommended to switch off the ADC before entering power saving sleep modes.

    - The ADC generates a 10-bit result which is presented in the ADC Data Registers, ADCH and ADCL. By default,
    the result is presented right adjusted, but can optionally be presented left adjusted by setting the ADLAR bit in
    ADMUX.

    - If the result is left adjusted and no more than 8-bit precision is required, it is sufficient to read ADCH. Otherwise,
    ADCL must be read first, then ADCH, to ensure that the content of the data registers belongs to the same conversion. Once ADCL is read, ADC access to data registers is blocked. This means that if ADCL has been read, and a
    conversion completes before ADCH is read, neither register is updated and the result from the conversion is lost.
    When ADCH is read, ADC access to the ADCH and ADCL Registers is re-enabled.
    
    - The ADC has its own interrupt which can be triggered when a conversion completes. When ADC access to the
    data registers is prohibited between reading of ADCH and ADCL, the interrupt will trigger even if the result is lost.
*/

#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include "tc_io.h"

typedef enum
{
    TC_ADC_VREF_VCC = 0x0,
    TC_ADC_VREF_EXT_VREF = 0x1,
    TC_ADC_VREF_INTERNAL_1V1 = 0x2,
    TC_ADC_VREF_INTERNAL_2V56_NO_BYPASS_CAP = 0x6,
    TC_ADC_VREF_INTERNAL_2V56_BYPASS_CAP = 0x7
} tc_adc_vref_t;

void tc_adc_set_vref(tc_adc_vref_t vref);
void tc_adc_set_input_ch(tc_pin_t pin);
void tc_adc_enable(void);
void tc_adc_enable_left_adjust(void);
void tc_adc_disable_left_adjust(void);
void tc_adc_disable(void);
void tc_adc_start_conversion(void);
bool tc_adc_conversion_finished(void);
uint16_t tc_adc_get_result(void);
uint16_t tc_adc_read(void);
uint16_t tc_adc_read_iterative(size_t cnt_iter);