#include "adc.h"

/*
    -- ADC is right-adjusted by default, which gives 10-bit resolution. It can be left adjusted by setting the
    ADLAR bit in ADMUX, which I will not do in this application.
p
    -- ADC does not consume power when ADEN is cleared, it is recommended to switch off the ADC before entering
    power saving sleep modes.

    -- If the result is left adjusted and no more than 8-bit precision is required, it is sufficient to read ADCH. Otherwise, ADCL must be read first, then ADCH, to ensure that the content of the data registers belongs to the same conversion. Once ADCL is read, ADC access to data registers is blocked. This means that if ADCL has been read, and a conversion completes before ADCH is read, neither register is updated and the result from the conversion is lost. When ADCH is read, ADC access to the ADCH and ADCL Registers is re-enabled.

    */

void tc_adc_set_vref(tc_adc_vref_t vref)
{
    // Clear the Voltage Reference Selection bits
    ADMUX &= ~( (1 << REFS2) | (1 << REFS1) | (1 << REFS0));
    switch(vref)
    {
        case TC_ADC_VREF_VCC:
        {
            // Do not change anything as it was already cleared just above
            // ADMUX |= ( (0 << REFS2) | (0 << REFS1) | (0 << REFS0));
            break;
        }
        case TC_ADC_VREF_EXT_VREF: 
        {
            ADMUX |= ( (0 << REFS2) | (0 << REFS1) | (1 << REFS0));
            break;
        }
        case TC_ADC_VREF_INTERNAL_1V1:
        {
            ADMUX |= ( (0 << REFS2) | (1 << REFS1) | (1 << REFS0));
            break;
        }
        case TC_ADC_VREF_INTERNAL_2V56_NO_BYPASS_CAP:
        {
            ADMUX |= ( (1 << REFS2) | (1 << REFS1) | (0 << REFS0));
            break;
        }
        case TC_ADC_VREF_INTERNAL_2V56_BYPASS_CAP:
        {
            ADMUX |= ( (1 << REFS2) | (1 << REFS1) | (1 << REFS0));
            break;
        }
    }
}


void tc_adc_set_input_ch(tc_pin_t pin)
{
    // Clear the ADMUX MUXN bits
    ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    switch(pin)
    {
        case PORTB5:
            break;
        case PORTB4:
            {
                ADMUX |= (0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (0 << MUX0);
                break;
            }
        case PORTB3:
            {
                ADMUX |= (0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (1 << MUX0);
                break;
            }
        case PORTB2:
            {
                ADMUX |= (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (1 << MUX0);
                break;
            }
        default:
        break;
    }
}

void tc_adc_enable(void)
{
    ADCSRA |= (1 << ADPS1);
    // Turn on the ADC Enable bit
    ADCSRA |= (1 << ADEN);
}

void tc_adc_enable_left_adjust(void)
{
    ADMUX |= 1 << ADLAR;
}

void tc_adc_disable_left_adjust(void)
{
    ADMUX &= ~(1 << ADLAR);
}

void tc_adc_disable(void)
{
    // Turn off the ADC Enable bit
    ADCSRA &= ~(1 << ADEN);
}

void tc_adc_start_conversion(void)
{
    // Turn on the ADC Start Conversion bit
    // *ADSC* Writing zero to this bit has no effect.
    ADCSRA |= (1 << ADSC);
}

bool tc_adc_conversion_finished(void)
{
    // Check the ADC Start Conversion bit to see if it is zero yet
    return (ADCSRA & (1 << ADSC)) == 0;
}

uint16_t tc_adc_get_result(void)
{
    uint16_t result = 0x0;
    result = ADCL;
    result |= (ADCH << 8);
    return result;
}

uint16_t tc_adc_read(void)
{
    tc_adc_start_conversion();
    while(!tc_adc_conversion_finished());
    return tc_adc_get_result();
}
