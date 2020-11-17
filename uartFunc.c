#include "pic18f26j50.h"
#include "usart.h"
#include "uartFunc.h"
#include <sw_uart.h>


    char BTName[] = "01";
    char SetName[] = "SET BT NAME   \r\n";
    char* SetA2DPSource = "SET PROFILE A2DP SOURCE\r\n";
    char* SetA2DPSink = "SET PROFILE A2DP SINK\r\n";
    char* inquiryName = "INQUIRY 3 NAME\r\n";
    char* PioControl = "SET CONTROL PIO\r\n";
    char* BTPower = "SET BT POWER";
    char* pinCode = "SET BT AUTH * XXXXX\r\n";
    char* ControlAudio = "SET CONTROL AUDIO INTERNAL INTERNAL\r\n";
    char* ControlBattery = "SET CONTROL BATTERY...\r\n";
    char* ControlCODEC = "SET CONTROL CODEC APT-X DUAL_CHANNEL 44100 0\r\n";
    char* preAmpControl ="SET CONTROL PREAMP 0 0\r\n";
    char* gain = "SET CONTROL GAIN 12 0\r\n";
    char* pair = "PAIR\r\n";
    char* reset = "RESET\r\n";
    //char  call[] = "CALL                   19 A2DP\r\n";
    //ou
    //char* call = "CALL                   110B A2DP\r\n";
    char* close = "CLOSE 0"; //close A2DP connection
    char* setBTClass = "SET BT CLASS 20043C\r\n";
    char* BTFilter = "SET BT FILTER 20043C 20043C\r\n"; // only scan for device with class 20043C
    char* escapeSequence = "+++";
    char* setControlBind = "SET CONTROL BIND...";
    char* setAuth = "SET AUTH ...";
    char* setPio = "SET PIO...";
    char* carrierDetect = "SET CONTROL CD \r\n";
    char* set = "SET\r\n";
    
void uartInit() //EUSART needs three registers to work : TXSTAx; RCSTAx; BAUDCONx. This function initialize the UART
{
   //Uart configuration par regisre direcetment
   
    RCSTA1bits.SPEN = 1; //enable serial port
    
   //Configure EUSART pin (in/out)
   //TRISCbits.TRISC7 = 1; //UART_RXalready equal to 1 on power on reset
   TRISCbits.TRISC6 = 0; //UART_TX

   //Transmit conrol config   
   TXSTA1bits.TX9 = 0; //8bit transmit
   TXSTA1bits.SYNC = 0; //asynchronous mode
   TXSTA1bits.BRGH = 1; //High speed baud rate
   TXSTA1bits.TXEN = 1; //Transmit enable
   //Baud configuration
   BAUDCON1bits.BRG16 = 1; //16 bit baud rate generator
   SPBRG1 = 103; //For 115200 baud rate with 48 Mhz clock
   
   //Receive control reg
    
   RCSTA1bits.RX9 = 0; //8 bit receive
   RCSTA1bits.CREN = 1; //enable reception
}

void Write1USART1(unsigned char data1)
{
   while((!TXSTA1bits.TRMT))
   {
       continue;
   }
    
  if(TXSTA1bits.TX9)  // 9-bit mode?
  {
    TXSTA1bits.TX9D = 0;       // Set the TX9D bit according to the
    if(USART1_Status.TX_NINE)  // USART1 Tx 9th bit in status reg
      TXSTA1bits.TX9D = 1;
  }

  TXREG1 = data1;      // Write the data byte to the USART2
  
}


void WriteStringUsart1(char*text)
{
    int i=0;
    while (text[i] !=  '\0')
    {
        Write1USART1(text[i]);
        i++;
    }
    
}


char receiveUsart (void)
{
    while (PIR1bits.RC1IF == 0) 
    { // wait for char
        if (RCSTA1bits.OERR == 1) 
        {
            RCSTA1bits.OERR = 0; // clear overrun if it occurs
            RCSTA1bits.CREN = 0; // clear error by disable uart reception
            RCSTA1bits.CREN = 1; //re-enable uart reception
        }
    }
return RCREG1;
}


void receiveUsartString(int length)
{
    //int ireceive = 0;
    int v1;
    char test2[150] ;
    while(v1<=length)
    {
        while(PIR1bits.RC1IF!=1) //RC1IF = 1 si on recoit un caractère
        {
            continue;
        }
            test2[v1++] = RCREG1;
            if (v1==199)
                v1=0; 
    }
    //while(1);
}

void uartWT32IinitReceiver()
{
    
    WriteStringUsart1(ControlAudio); 
    WriteStringUsart1(preAmpControl);
    WriteStringUsart1(ControlCODEC); 
    WriteStringUsart1(SetA2DPSource);
    WriteStringUsart1(setBTClass);
    WriteStringUsart1(BTFilter);
    WriteStringUsart1(reset);
    WriteStringUsart1(gain);
    //WriteStringUsart1(SetName);    
}


void macAdress(){
    int j = 0;
    int buff = 0;
    int length = 0;
    //char* MacAdress;
    char MacAdress[100];
    WriteStringUsart1(inquiryName);
    MacAdress[j] = RCREG1;
    while (MacAdress[j] != 'N')
    {
        while(PIR1bits.RC1IF != 1)
        {
            //waiting for a character
        }
            MacAdress[j] = RCREG1;    
    }
    
    while (length<10) //Nombre de caracteres attendus
    {
        while(PIR1bits.RC1IF != 1)
        {
            length++;
        }
        MacAdress[j++] = RCREG1;
    }

    if (MacAdress[11] == BTName[1] + 1)
    {
        if (MacAdress[10] == BTName[0])
        {
            //call
        }//revoir le cas ou on a 09 et 10
        else
        {
           // MacAdress = NULL;
        }
    } // sinon mac adress = null   
    else
    {
        //MacAdress = NULL;
    }
}

void renameDevice(char* newDeviceNumber)
{
    char SetName1[] = "SET BT NAME   \r\n";
    SetName1[12] = newDeviceNumber[0];//mettre ces deux lignes dans encoder + deconnecter et rconnecter wt32i dans la fonction
    SetName1[13] = newDeviceNumber[1];
    WriteStringUsart1(SetName1);   
    //WriteStringUsart1(inquiryName); 
}