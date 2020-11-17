#include <stdlib.h>
#include <stdio.h>
#include "encoderFunctions.h"
#include "7segFunc.h"
#include <xc.h>
#include <pic18f26j50.h>
#include <usart.h>
#include <sw_uart.h>
#include <plib.h>
#include <portb.h>
#include <sw_uart.h>
#include <pps.h> //define pin as input or ouput, or interrupt

void seg7PortInit()
{
//7 seg display outputs
//TRISx = define pin as input or output
     TRISBbits.TRISB0 = 0; //pin G
     TRISAbits.TRISA2 = 0; //pin A
     TRISCbits.TRISC2 = 0; //pin F
     TRISAbits.TRISA0 = 0; //pin A_DIG2
     TRISAbits.TRISA1 = 0; //pin A_DIG1        
     TRISAbits.TRISA3 = 0; //pin B
     TRISAbits.TRISA5 = 0; //pin C
     TRISBbits.TRISB1 = 0; //pin E
     TRISBbits.TRISB3 = 0; //pin D         
}