#include "control.h"

uint8_t tc_get_adjusted_bias(uint16_t Vsource,uint16_t Vfeedback)
{
    if (Vsource >= 840 && Vsource < 900) return TC_OUTPUT_DEFAULT_BIAS - 4; 
    else if (Vsource >= 900 && Vsource < 950) return TC_OUTPUT_DEFAULT_BIAS - 6; 
    else if (Vsource >= 950 && Vsource < 1000) return TC_OUTPUT_DEFAULT_BIAS - 8; 
    else if (Vsource >= 1000 && Vsource < 1050) return TC_OUTPUT_DEFAULT_BIAS - 10; 
    
    if(Vsource < 750 && Vsource >= 720) return TC_OUTPUT_DEFAULT_BIAS + 3;
    else if(Vsource < 720 && Vsource >= 680) return TC_OUTPUT_DEFAULT_BIAS + 4;
    else if(Vsource < 680 && Vsource >= 630) return TC_OUTPUT_DEFAULT_BIAS + 6;
    else if(Vsource < 630 && Vsource >= 570) return TC_OUTPUT_DEFAULT_BIAS + 8;
    else if(Vsource < 570 && Vsource >= 520) return TC_OUTPUT_DEFAULT_BIAS + 10;
    else if(Vsource < 520 && Vsource >= 450) return TC_OUTPUT_DEFAULT_BIAS + 12;
    else if(Vsource < 450 && Vsource >= 420) return TC_OUTPUT_DEFAULT_BIAS + 14;
    else if(Vsource < 420) return TC_OUTPUT_DEFAULT_BIAS + 16;

    return TC_OUTPUT_DEFAULT_BIAS;
}