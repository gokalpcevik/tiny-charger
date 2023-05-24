#include "timer.h"

void tc_enable_pll(void)
{
    // Enable PLL
    PLLCSR |= (1 << PLLE);
    // The usual wait is 100us before the PLL stabilizes
    _delay_us(120);

    // Wait for PLL to reach steady-state(poll PLL Lock Detector bit)
    while ((PLLCSR & (1 << PLOCK)) == 0);
}

void tc_tim1_enable_async_mode(void)
{
    PLLCSR |= 1 << PCKE;
}

void tc_tim1_set_OCRB(uint8_t val)
{
    OCR1B = val;
}

void tc_tim1_set_OCRC(uint8_t val)
{
    OCR1C = val;
}

void tc_tim1_gtcc_set_pwm_mode(tc_tim_pwm_mode mode)
{
    // Turn off the bits (to default values)
    GTCCR &= ~((1 << COM1B1) | (1 << COM1B0));

    switch (mode)
    {
    case TC_PWM_NO_CONNECT:
        break;
    case TC_PWM_CS_ON_COMPARE_MATCH:
        GTCCR |= (1 << COM1B0);
        break;
    case TC_PWM_CNC_ON_COMPARE_MATCH:
        GTCCR |= (1 << COM1B1);
        break;
    case TC_PWM_SNC_ON_COMPARE_MATCH:
        GTCCR |= (1 << COM1B1)  | (1 << COM1B0);
        break;
    default:
        break;
    }
}

void tc_tim1_enable_pwm(tc_pwm_select modulator)
{
    if(modulator == TC_PWM_A)
        TCCR1 |= (1 << PWM1A);
    else if (modulator == TC_PWM_B)
        GTCCR |= (1 << PWM1B);
}

void tc_tim1_disable_pwm(tc_pwm_select modulator)
{
    if(modulator == TC_PWM_A)
        TCCR1 &= ~(1 << PWM1A);
    else if (modulator == TC_PWM_B)
        GTCCR &= ~(1 << PWM1B);
}

void tc_tim1_select_prescaler(uint8_t prescaling_src)
{
    uint8_t clrd = prescaling_src;
    // Clear the 4 left-most bits just in case the input is bad
    clrd &= ~(0b1111 << 4);
    // reset the prescale select bits
    TCCR1 &= ~(0b1111);
    TCCR1 |= clrd;
}

void tc_tim1_enable_clear_timer_on_comp_match(void)
{
    TCCR1 |= (1 << CTC1);
}
