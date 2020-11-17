/* 
 * File:   encoderFunctions.c
 * Author: Alassane
 *
 * Created on 23 août 2019, 18:13
 */

#include "pic18f26j50.h"
#include "uartFunc.h"



void encoderInit ()
{
//Assing encoder pin to pic pins
    //PORTx = read value pin
    //TRISx = define pin as input or output
    //LATx = read/write value on a pin
    //Rotary encoder interrupt inputs

    char deviceNumber[2];
    TRISBbits.TRISB4 = 1; //pin 1 encoder
    TRISBbits.TRISB5 = 1; //pin 2 encoder
    TRISBbits.TRISB6 = 1; //pin 4 encoder
    TRISBbits.TRISB7 = 1; //pin 8 encoder

    deviceNumber[0] = '0';
    deviceNumber[1] = '0';
     
    //renameDevice(deviceNumber);//appel fonction pour renommer device wt32i
     //activate port B interupts
    //return deviceNumber;
}

void encoderInterrupt()
{
    //char* deviceNumber;
    //INTCONbits.RBIF = 0; //flag d'interruption mis à 0 dans l'interruption
    //PORTB; //PORTB need to be read to set RBIF interrution flag to 0
    char deviceNumber[2];
    int PortBRead;
    PortBRead = PORTB;
    PortBRead = PortBRead & 0xF0; //Masque sur les 4 bits de poids faible (ne sont pas reliés à l'encodeur rotatif))
    LATB = LATB & 0x0F;
        if (PortBRead  == 0x00)
        {
           LATAbits.LATA2 = 0; //pin A        
           LATAbits.LATA3 = 0; //pin B 
           LATAbits.LATA5 = 0; //pin C
           LATBbits.LATB3 = 0; //pin D
           LATBbits.LATB1 = 0; //pin E
           LATCbits.LATC2 = 0; //pin F
           LATBbits.LATB0 = 0; //pin G
           LATAbits.LATA0 = 0; //pin A_DIG2 n
           LATAbits.LATA1 = 1; //pin A_DIG1
           deviceNumber[0] = '0';
           deviceNumber[1] = '0';
           //interrupt connection by using escape sequence by sending "+" (déja fait dans l'interruption)
           //renameDevice(deviceNumber);//appel fonction pour renommer device BTName ="88";
           //Reprendre étape initialisation wt32i_init()         
        }

        else if (PortBRead  == 0x10)
        {
           LATAbits.LATA2 = 0; //pin A        
           LATAbits.LATA3 = 1; //pin B 
           LATAbits.LATA5 = 1; //pin C
           LATBbits.LATB3 = 0; //pin D
           LATBbits.LATB1 = 0; //pin E
           LATCbits.LATC2 = 0; //pin F
           LATBbits.LATB0 = 0; //pin G
           LATAbits.LATA0 = 1; //pin A_DIG2 n
           LATAbits.LATA1 = 1; //pin A_DIG1  deviceNumber[0] = '0';
           deviceNumber[0] = '0';
           deviceNumber[1] = '1';
           //interrupt connection by using escape sequence by sending "+"
           //renameDevice(deviceNumber);//appel fonction pour renommer device BTName ="88";
           //Reprendre étape initialisation wt32i_init() 
        }

        else if (PortBRead  == 0x20)
        {
           LATAbits.LATA2 = 1; //pin A        
           LATAbits.LATA3 = 1; //pin B 
           LATAbits.LATA5 = 0; //pin C
           LATBbits.LATB3 = 1; //pin D
           LATBbits.LATB1 = 1; //pin E
           LATCbits.LATC2 = 0; //pin F
           LATBbits.LATB0 = 1; //pin G
           LATAbits.LATA0 = 1; //pin A_DIG2 n
           LATAbits.LATA1 = 1; //pin A_DIG1  deviceNumber[0] = '0';
           deviceNumber[0] = '0';
           deviceNumber[1] = '2';
           //interrupt connection by using escape sequence by sending "+"
           //renameDevice(deviceNumber);//appel fonction pour renommer device BTName ="88";
           //Reprendre étape initialisation wt32i_init() 
        }
        
        else if (PortBRead  == 0x30)
        {
           LATAbits.LATA2 = 1; //pin A        
           LATAbits.LATA3 = 1; //pin B 
           LATAbits.LATA5 = 1; //pin C
           LATBbits.LATB3 = 1; //pin D
           LATBbits.LATB1 = 0; //pin E
           LATCbits.LATC2 = 0; //pin F
           LATBbits.LATB0 = 1; //pin G
           LATAbits.LATA0 = 1; //pin A_DIG2 n
           LATAbits.LATA1 = 1; //pin A_DIG1  deviceNumber[0] = '0';
           deviceNumber[0] = '0';
           deviceNumber[1] = '3';
           //interrupt connection by using escape sequence by sending "+"
           //renameDevice(deviceNumber);//appel fonction pour renommer device BTName ="88";
           //Reprendre étape initialisation wt32i_init() 
        }
        
        else if (PortBRead  == 0x40)
        {
           LATAbits.LATA2 = 0; //pin A        
           LATAbits.LATA3 = 1; //pin B 
           LATAbits.LATA5 = 1; //pin C
           LATBbits.LATB3 = 0; //pin D
           LATBbits.LATB1 = 0; //pin E
           LATCbits.LATC2 = 1; //pin F
           LATBbits.LATB0 = 1; //pin G
           LATAbits.LATA0 = 1; //pin A_DIG2 n
           LATAbits.LATA1 = 1; //pin A_DIG1  deviceNumber[0] = '0';
           deviceNumber[0] = '0';
           deviceNumber[1] = '4';
           //interrupt connection by using escape sequence by sending "+"
           //renameDevice(deviceNumber);//appel fonction pour renommer device BTName ="88";
           //refaire inquiry et connect
           //Reprendre étape initialisation wt32i_init() 
        }
    
        else
        {
           deviceNumber[0] = '8';
           deviceNumber[1] = '8';
           //interrupt connection by using escape sequence by sending "+"
           //renameDevice(deviceNumber);//appel fonction pour renommer device BTName ="88";
           //Reprendre étape initialisation wt32i_init() 
        }
            
           renameDevice(deviceNumber);

        //appeler fonction pour initialisation wt32i
          // return deviceNumber;
}


void encoderInterruptBis(int encoderVal )
{
           LATAbits.LATA2 = (encoderVal>>7)&0x01; //pin A        
           LATAbits.LATA3 = (encoderVal>>7)&0x01; //pin B 
           LATAbits.LATA5 = (encoderVal>>7)&0x01; //pin C
           LATBbits.LATB3 = (encoderVal>>7)&0x01; //pin D
           LATBbits.LATB1 = (encoderVal>>7)&0x01; //pin E
           LATCbits.LATC2 = (encoderVal>>7)&0x01; //pin F
           LATBbits.LATB0 = (encoderVal>>7)&0x01; //pin G
           LATAbits.LATA0 = (encoderVal>>7)&0x01; //pin A_DIG2 n
           LATAbits.LATA1 = (encoderVal>>7)&0x01; //pin A_DIG1  deviceNumber[0] = '0';
          // deviceNumber[0] = '0';
           //deviceNumber[1] = '4';
}