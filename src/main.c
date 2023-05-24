// Written by Gökalp, 5/23/2023.

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>

#include "timer.h"
#include "adc.h"
#include "tc_io.h"
#include "control.h"

//uint16_t reevaluate_Vout_counter = 0;
// Around 55%, w.r.t OCR1C=166
uint8_t duty_cycle = 92U;
// Default value: 9V/4.5=2V from voltage divider
// 2V/2.56V * 1023 = ~799
uint16_t Vsource = 799U;
// Default Value: 5V/4.5=1.111V from voltage divider
// 1.111V/2.56V * 1023 = ~444
uint16_t Vfeedback = 444U;

int main(void)
{
  tc_set_pin_dir(TC_PWM_PIN, TC_PIN_DIR_OUTPUT);
  tc_set_pin_dir(TC_VSOURCE_PIN, TC_PIN_DIR_INPUT);
  tc_set_pin_dir(TC_VOUT_PIN, TC_PIN_DIR_INPUT);

  /* ADC Setup */
  tc_adc_set_vref(TC_ADC_VREF_INTERNAL_2V56_NO_BYPASS_CAP);
  tc_adc_enable();
  /* --------- */

  /* Timer1 Setup */
  tc_enable_pll();
  tc_tim1_enable_async_mode();
  tc_tim1_enable_clear_timer_on_comp_match();
  tc_tim1_select_prescaler(0x4);
  // For output compare B
  tc_tim1_gtcc_set_pwm_mode(TC_PWM_CNC_ON_COMPARE_MATCH);
  /* ----------- */

  _delay_ms(10);

  OCR1C = TC_OC1C;

  tc_adc_set_input_ch(TC_VSOURCE_PIN);

  uint16_t Vinitial = 0;
  
  for(uint8_t i = 0; i < TC_ADC_NUM_ITER; ++i)
  {
    Vinitial += tc_adc_read();
  }
  Vinitial = Vinitial / TC_ADC_NUM_ITER;

  OCR1B = (uint8_t)(TC_ADC_VALUE_VSS * TC_OC1C / Vinitial + TC_OUTPUT_DEFAULT_BIAS);

  tc_tim1_enable_pwm(TC_PWM_B);

  // Wait 100ms for the output voltage to stabilize
  _delay_ms(100);

  while (1)
  {
    /* Read Vfeedback */

    // Set the input channel 
    tc_adc_set_input_ch(TC_VOUT_PIN);

    // Iterate ADC readings and average out
    uint16_t Vfeedback_last = 0;
    for(uint8_t i = 0; i < TC_ADC_NUM_ITER; ++i)
    {
      Vfeedback_last += tc_adc_read();
    }
    Vfeedback_last = Vfeedback_last / TC_ADC_NUM_ITER;
    /*---------------------------------------------------------*/


    /* Read battery(source) voltage */

    // Set the input channel 
    tc_adc_set_input_ch(TC_VSOURCE_PIN);
    
    // Iterate ADC readings and average out
    uint16_t Vsource_last = 0;
    for(uint8_t i = 0; i < TC_ADC_NUM_ITER; ++i)
    {
      Vsource_last += tc_adc_read();
    }
    Vsource_last = Vsource_last / TC_ADC_NUM_ITER;
    /*--------------------------------------------------------*/

    int delta_Vsource = abs(Vsource_last - Vsource);
    int delta_Vfeedback = abs(Vfeedback_last - Vfeedback);

    bool Vsource_changed = delta_Vsource > TC_VSOURCE_HYSTERESIS_THRESHOLD;
    bool Vfeedback_changed = delta_Vfeedback > TC_VOUT_HYSTERESIS_THRESHOLD;
    // Causes oscillations
    //bool needs_reevaluation = reevaluate_Vout_counter > TC_REEVALUATE_VOUT_IF_GREATER;

    if (Vsource_changed || Vfeedback_changed)
    {
      //reevaluate_Vout_counter = 0;

      if (Vsource_changed)  Vsource = Vsource_last;
      if (Vfeedback_changed)  Vfeedback = Vfeedback_last;

      uint32_t calc_duty_cycle = ((2UL * (uint32_t)TC_ADC_VALUE_VSS - (uint32_t)Vfeedback_last) * (uint32_t)TC_OC1C) / (uint32_t)Vsource_last;

      // Add the bias
      duty_cycle = (uint8_t)calc_duty_cycle + tc_get_dynamic_bias(Vsource_last,Vfeedback_last);

      // Finally, set the timer value to the duty cycle(w.r.t to OC1C)
      OCR1B = duty_cycle;
    }
    else
    {
      //++reevaluate_Vout_counter;
    }
    _delay_ms(1);
  }
}
