/*
 * File:   main.c
 * Author: Alassane
 *
 * Created on 14 août 2019, 21:05
 */

#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <stdint.h>
#include "encoderFunctions.h"
#include "7segFunc.h"
#include "picInit.h"
#include "uartFunc.h"
#include <xc.h>
#include <pic18f26j50.h>
#include <usart.h>
#include <sw_uart.h>
#include <plib.h>
#include <portb.h>
#include <pps.h> //define pin as input or ouput, or interrupt

// CONFIG1L
#pragma config WDTEN = OFF       // Watchdog Timer (Enabled)
#pragma config PLLDIV = 2      // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset  (Enabled)
#pragma config XINST = OFF       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config CPUDIV = OSC1    // CPU System Clock Postscaler (No CPU system clock divide)
#pragma config CP0 = OFF        // Code Protect (Program memory is not code-protected)

// CONFIG2L
#pragma config OSC = INTOSCPLLO  // Oscillator (INTOSCPLL)
#pragma config T1DIG = OFF      // T1OSCEN Enforcement (Secondary Oscillator clock source may not be selected)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator (High-power operation)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor (Enabled)
#pragma config IESO = OFF        // Internal External Oscillator Switch Over Mode (Enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Postscaler (1:32768)

// CONFIG3L
#pragma config DSWDTOSC = INTOSCREF// DSWDT Clock Select (DSWDT uses INTRC)
#pragma config RTCOSC = T1OSCREF// RTCC Clock Select (RTCC uses T1OSC/T1CKI)
#pragma config DSBOREN = ON     // Deep Sleep BOR (Enabled)
#pragma config DSWDTEN = ON     // Deep Sleep Watchdog Timer (Enabled)
#pragma config DSWDTPS = G2     // Deep Sleep Watchdog Postscaler (1:2,147,483,648 (25.7 days))

// CONFIG3H
#pragma config IOL1WAY = ON     // IOLOCK One-Way Set Enable bit (The IOLOCK bit (PPSCON<0>) can be set once)
#pragma config MSSP7B_EN = MSK7 // MSSP address masking (7 Bit address masking mode)

// CONFIG4L
#pragma config WPFP = PAGE_63   // Write/Erase Protect Page Start/End Location (Write Protect Program Flash Page 63)
#pragma config WPEND = PAGE_WPFP// Write/Erase Protect Region Select (valid when WPDIS = 0) (Page WPFP<5:0> through Configuration Words erase/write protected)
#pragma config WPCFG = OFF      // Write/Erase Protect Configuration Region (Configuration Words page not erase/write-protected)
#define _XTAL_FREQ 48000000

// CONFIG4H
#pragma config WPDIS = OFF      // Write Protect Disable bit (WPFP<5:0>/WPEND region ignored)


    void macAdresss(void);
    void findMacAdress (void);
    //char* MacAdress = NULL;


    
    //Variable globale interruption (on les mets à 1 si interruption)
    volatile int flagEncoderInterrupt = 0;
    volatile int flagUartTxInterrupt = 0;
    volatile int flagUartRxInterrupt = 0;
    volatile int ireceive;
    
    int launchInquiry = 0;

//input fonction #define iPPSInput(fn,pin) fn=pin

//output fonction #define PPSOutput(pin,fn) iPPSOutput(OUT_PIN_##pin,OUT_FN_##fn)
//#define PPSOutput(PPS_RP12,PPS_NULL) 

volatile int i = 0; //compteur tableau receiving usart
char test[200];
int k = 0; //compteur tableau mac adress
char MacAdress[300];
//char MacAdress[600];
long i1 = 0;
int inquiryDone = 0;
char  call[] = "CALL                   19 A2DP\r\n";
char  *call1 = "CALL                   19 A2DP\r\n";
//volatile char  *call12 = "CALL                   19 A2DP\r\n";


void main(void) {

/*config horloge 48 Mhz*/
    //PLLDIV<2:0> = 110
    //CPDIV<1:0> = 11(no div)
    OSCTUNEbits.PLLEN = 1;// => 96 MHz PLL is enabled
    OSCCONbits.SCS1 = 0; //SCS = 00 => Primary clock source = CPU Divider
    OSCCONbits.SCS0 = 0;
    OSCCONbits.IRCF0 = 0;//IRCF = 110 => 4 Mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    
    
volatile char  *call12 = "CALL                   19 A2DP\r\n";
//volatile char var1 = *call12;
call12[2] = 'r';
    //Output
    //unused pinconfigured as outout
    //iPPSOutput(OUT_PIN_PPS_RP12,OUT_FN_PPS_NULL); //unused i/o port, will have to be used for uart2 on RP11 and RP12
    picInit(); //define unused pin as outputs
    
    //Initialisation : deactivate useless peripheral
    PORTB=0x00;

    //7 seg display outputs
    seg7PortInit();
    
    //PORTx = read value pin, TRISx = define pin as input or output, LATx = write value on an output pin
    //Rotary encoder interrupt inputs
    uartInit(); 
    //call1[0] = 'r';
    __delay_ms(500);


    
    char test1[200];
    //Configutration port B interruptions
    //OpenPORTB(PORTB_CHANGE_INT_ON); //à  désactiver quand une interruption a lieu
     
    //désactiver toutes les intterruptions et activer interruptions du port b dans encoder functions
    //priority  ipen bit
     INTCONbits.RBIF = 0; //flag interuption mis à 0
     RCONbits.IPEN = 1; //Enable interrupt priority feature
     //INTCON = 0x00; already equal to 0 on reset
     INTCON2bits.RBPU = 1; //activiation priority feature
     INTCON2bits.RBIP = 0; //Activation Port B interrupt low priority
     INTCONbits.RBIE = 1; //activation port B on change interrupt
     PIE1bits.TX1IE = 0; //Disable usart  TX interrupt
     PIE1bits.RC1IE = 1; //disable usart RX interrupts
     INTCONbits.GIE_GIEH = 1; //Enable global interrupts high priority
     INTCONbits.PEIE_GIEL = 1; //Enable Low priority
     //RCSTA1bits.SPEN = 1; //Serial port activation (already done in a function)
     
     uartWT32IinitReceiver();
     encoderInit();
     encoderInterrupt();

//     doubleDIm[1][1] = 'a';
     //WriteStringUsart1("INQUIRY 3 NAME\r\n");
     //macAdress();
     //receiveUsartString(50);
     //while(1);
     //S'assurer que buffer reception est vide dans la boucle suivante

     int j=0;
     int v = 0;
     for (v = 0;v<200;v++)
     {
         test1[v] = '0';
     }
     v=0;

    while(1)
    {
        //macAdresss();
       // TRISCbits.TRISC6=0;
       // while(PIR1bits.RC1IF == 0)
        //{
            if(flagEncoderInterrupt == 1)
            {   
                flagEncoderInterrupt = 0;
                encoderInterrupt();
                INTCONbits.RBIE = 1;
            }
              
            macAdresss(); //fonction recuperation macAdress et connection (un flag après interrupton de l'encodeur permet de récupérer l'adresse MAC)
            
            if(flagUartTxInterrupt == 1) //Voir si on désactive interruptions emissions uart
            {
                flagUartTxInterrupt = 0;
            }
            //findMacAdress ();
        //}
         //read rcreg three times or more(before sending inquiry) to empty receive buffer (Two char in FIFO, one in RSR)
         //ON POURRAIT ACTIVER LES INTERRUPTIONS DE L UART APRES
         // ENVOIE DE LA COMMANDE INQUIRY 
        
       /*     
        while(PIR1bits.RC1IF !=1) //RC1IF = 1 si on recoit un caractère
        {
        }
            flagUartRxInterrupt = 0; 
            test1[v++] = RCREG1;
            if (v==199)
                v=0;*/
        //}
  
    }
    return; 
}


void __interrupt(high_priority) allInterrupts (void)
{
    
    //PIE1bits.TX1IE = 0; 
    PIE1bits.RC1IE = 0;//disable usart Rx interrupt
    //desactiver global interrupts ou pas??
    
    if(PIR1bits.RC1IF == 1) //Usart receive interrupt flag cleared only when RCREG is read
    {
        if(launchInquiry!=1) //ou while (lauchInquiry nous permet d'indiquer si une inquiry (fct pour chercher macAdress) a été lancée. launchInquiry repasse à 0 lorsqu'une mac adress a été trouvée)
        {
            i1 = 0;
            if(i == 200)
            {
                i = 0;
            }
            PIE1bits.RC1IE = 0;
            flagUartRxInterrupt = 1; 
            test[i++] = RCREG1; //Tableau pour enregistrer tout ce qui vient du wt32i et n'est pas l'adresse mac
            
        }
        else    //Si inquiry a été lancée (launchInquiry = 1), on enregistre les nouvelles valeurs provenant du wt32i dans tableau macAdress
        //if(launchInquiry==1)
        {  
            if(k == 299)
            {
                k = 0;
            }
            PIE1bits.RC1IE = 0;
            flagUartRxInterrupt = 1; 
            MacAdress[k++] = RCREG1; //tableau pour enregistrer adresse mac   
        }
        /*else
        {
            RCREG1;
        }*/
    }  
    PIE1bits.RC1IE = 1; //enbale usart Rx interrupt
}


//VOIR POURQUOI INTERRUPTION PORT B S'ACTIVE PLUSIEURS FOIS (s'active plusieurs fois car plusieurs bits changent en même temps)

void __interrupt(low_priority) allInterruptsLow (void) //Pour interruption du port B
{
    
    INTCONbits.RBIE = 0;//Desactivation port b on change interrupt, reactivation done in main function
    
    if(INTCONbits.RBIF == 1) //Port B interrupt on change flag
    { 
        PORTB; // read portb to clear rbif (port B interrupt flag)
        INTCONbits.RBIF = 0;
        flagEncoderInterrupt = 1; //Flag d'interruption de l'encodeur mis à 1
        i1 = 0; //On remet compteur avant enregistrement dans deuxieme tableau à 0 
        launchInquiry = 0;
        inquiryDone = 0; //launchInquiry et inquiryDone (fonction MacAdress() remis à 0 pour permettre d'enregistrer nouvelles valeurs de mac adress dans tableau MacAdress)
        WriteStringUsart1("close 0\r\n");//interrupt connection by using escape sequence by sending "+"
        //SET NAME
    }
}



void macAdresss()
{   
    int j;
    
    if(inquiryDone != 1)    //Si on fait un premier inquiry, la fonction ne s'execute plus car inquiryDone passe à 1
    {
 
        if ((i1<800000)&&(PIR1bits.RC1IF == 0)&&(PIE1bits.RC1IE == 1)) //Boucle d'attente utilisé pour recevoir tous les paramètres du WT32i avant inquiry
        {
            i1++;//i1 mis à 0 dans l'interruption; 
        }
        
        if(i1>=800000)
        {
            //PIE1bits.RC1IE = 0;
            if(launchInquiry != 1)
            {
              WriteStringUsart1("INQUIRY 4 NAME\r\n");
              launchInquiry = 1;
              inquiryDone = 1;
            }
            __delay_ms(3000);
            __delay_ms(3000); //On met des délais afin de recevoir l'ensemble des adresses MAC
            findMacAdress();
        /*
            while(PIR1bits.RC1IF == 1) //Usart receive interrupt flag cleared only when RCREG is read
            {
                if(i==200)
                    i=0;
                PIE1bits.RC1IE = 0;
                flagUartRxInterrupt = 1; 
                test45[i++][j] = RCREG1;
            }
        */
        }

    }

}



//recherche dans tableau macAdress
// "\nNAME xx:xx:xx:xx:xx:xx:xx:xx "01" "
void findMacAdress (void) //REFAIRE INQUIRY EN BOUCLE SI AUCUNE MAC ADRESSE TROUVE
{
 int pos = 0; //position dans le tableau d'adresses mac
 int iMac = 0;
 int tailleDuTableau = 299;
 int macAdressPos = 0;
 int x = 0;
    while(iMac<=tailleDuTableau)
    {
        pos = iMac;
        if (MacAdress[pos] == '\n')
        {
            macAdressPos = iMac + 6;

            if(MacAdress[pos+1] == 'N')
            {
               // if(macAdress[pos+31] == deviceNumber[1]+1)  //encoderInterrupt doit retourner nom du device
                if(MacAdress[pos+26] == '2') //si mac adress pos = nom du wt32i + 1
                {
                    //NAME 88:6b:0f:d3:8e:73 "02"
                    for(x=5;x<22;x++)
                    {
                        call[x] = MacAdress[macAdressPos];   
                        macAdressPos++;
                    }
                    x = 0;
                    launchInquiry = 0; //pour ne pas enregistrer dans tableau MacAdress et dire qu'on a trouvé la bonne mac adresse
                    WriteStringUsart1(call);
                }
            }
        }
        
        iMac++;
    }
 
     /* for (x = 0;x<300;x++)
     {
         MacAdress[x] = '0';
     }*/
     
 }
 