/* 
 * File:   uartFunc.h
 * Author: Alassane
 *
 * Created on 24 août 2019, 20:59
 */

//set baudrate and interrupts
void uartInit(void);
void uartWT32IinitReceiver(void);
void WriteStringUsart1(char*);
void Write1USART1(unsigned char);
char receiveUsart(void);
void receiveUsartString(int);
void macAdress(void);
void renameDevice(char*);

    