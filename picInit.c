#include "pic18f26j50.h"

void picInit()
{
    //Output
    //unused pin configured as outout
    //TRISx = define pin as input or output
   //iPPSOutput(OUT_PIN_PPS_RP12,OUT_FN_PPS_NULL); //unused i/o port, will have to be used for uart2 on RP11 and RP12
    TRISAbits.TRISA7 = 0;
    TRISAbits.TRISA6 = 0;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC4 = 0;
}