
// PIC18F25K80 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
/*MCLR (pin 1) MCLR/V PP /RE3
Vmeasure (pin 2) RA0
J1850 Volts (pin 3)RA1
J1850 Bus+ (pin 4)RA2
Memory (pin 5)RA3
Baud Rate (pin 6)RA4
LFmode (pin 7)RA5
VSS (pins 8 and 19)V SS
XT1 (pin 9) and RA7
XT2 (pin 10)RA6
VPW In (pin 11)RC0
ISO In (pin 12)RC1
PWM In (pin 13)RC2
J1850 Bus- (pin 14)RC3

RTS (pin 15)RC4
Busy (pin 16)RC5
RS232UartTx (pin 17)RC6
RS232UartRx (pin 18)RC7
VSS (pin 19)V SS
VDD (pin 20)V DD
ISO K (pin 21)  RB0
ISO L (pin 22)RB1
CAN Tx (pin 23) RB2
CAN Rx (pin 24)RB3
RS232 Rx LED (pin 25), RB4
RS232 Tx LED (pin 26),RB5
OBD Rx LED (pin 27) RB6/PGC
OBD Tx LED (pin 28)RB7/PGD
*/

#include <xc.h>
//#include <string.h>
#include <pic18f46k80.h>

#define _XTAL_FREQ 4000000
#include <plib.h>
#include <xc.h>
#include "io_bus.h"
#include <p18cxxx.h>
#include "config.h"

#include <stdio.h>
//#include <stdarg.h>
#include <stdlib.h>


//#include "usart.h"

#define OBDTX_LED        PORTBbits.RB7
#define OBDRX_LED        PORTBbits.RB6
#define RSTX_LED     PORTBbits.RB5
#define RSRX_LED     PORTBbits.RB4
#define CANRX        PORTBbits.RB3
#define CANTX        PORTBbits.RB2
#define ISOL        PORTBbits.RB1
#define ISOK        PORTBbits.RB0
#define FCY 4000000
#define UART_BITRATE 38400

void Delay(unsigned int count);
void Heartbeat(void);

int heartbeatCount;
int heartbeatCountTwo;
unsigned char Txdata[] = "\r\niGeek ODB2 Open Sourced ELM327 Clone firmware ";
unsigned char Newline[] = "\n\r";
unsigned char spacing[] = "   ";
unsigned char inttext[] = "   INT ";
unsigned char newvals[] = "";
float volts;
unsigned int ADCValue = 0;
unsigned char ADCStringVal[4];
unsigned char ADCStringValTwo[4];


void setup (void)
{
    TRISB = 0b00000000;
    TRISC = 0;
  //  TRISA=0;
    ANCON1 = 0;
    WPUB  = 0b00000000;
   // ADCON1=0x7F;

    
    PORTBbits.RB0 = 1;
    PORTBbits.RB1 = 1;
    PORTBbits.RB2 = 1;
    PORTBbits.RB3 = 1;
    PORTBbits.RB4 = 1;
    PORTBbits.RB5 = 1;
    PORTBbits.RB6 = 1;
    PORTBbits.RB7 = 1;
    
}
void Delay(unsigned int count)
{
    // Countdown until equal to zero and then return
    while(count--);
}    

void Heartbeat(void)
{
    // Toggle LED every 256th time this function is called
    if (heartbeatCount < 255)
    {
        heartbeatCount++;
    }
    else
    {
        heartbeatCount = 0;
        RSRX_LED ^= 1;
        RSTX_LED ^= 1;
      //  RSTX_LED ^= 1;
        OBDRX_LED ^= 1;
        OBDTX_LED ^= 1;
       // OBDTX_LED ^= 1;
    
    }
}



unsigned char* returnString(unsigned char inputValue)
{
    static unsigned char processedString[4]; // Return a string of 3 digits.
    unsigned char firstDigitCounter = 0; // Brute-force counter for first digit.
    unsigned char secondDigitCounter = 0; // Brute-force counter for second digit.
    if (inputValue > 99) // If we have a 3 digit number,
    {
        while (inputValue > 99) // Until our number is 3 digits, i.e. bigger than 99,
        {
            inputValue -= 100; // Subtract 100 and..
            firstDigitCounter++; //.. increment first digit.
        }
        while (inputValue > 9) // Until our number is 3 digits, i.e. bigger than 9,
        {
            inputValue -= 10; // Subtract 10 and..
            secondDigitCounter++; //.. increment second digit.
        }

        // Now, we have left the 'inputValue' as a single digit.

        processedString[0] = firstDigitCounter + 0x30; // First digit
        processedString[1] = secondDigitCounter + 0x30; // Second digit
        processedString[2] = inputValue + 0x30; // Third digit
        processedString[3] = '\0'; // String terminator.
    }
    else // If we have a 2 digit number,
    {
        while (inputValue > 9) // Until our number is 3 digits, i.e. bigger than 99,
        {
            inputValue -= 10; // Subtract 10 and..
            secondDigitCounter++; //.. increment second digit.
        }
        processedString[0] = secondDigitCounter + 0x30; // Second digit
        processedString[1] = inputValue + 0x30; // Third digit
        processedString[2] = '\0'; // String terminator.
    }
    return processedString; // Return the processed string.
}
void UART1Init(long baud_rate){
    RCSTA1bits.SPEN = 1;	// enable port
    TRISCbits.TRISC7 = 1;	// make rx pin an input
    RCSTA1bits.CREN = 1;	// enable receive
    TRISCbits.TRISC6 = 0;	// make tx pin an output
    TXSTA1bits.TXEN = 1;	// enable transmit
    TXSTA1bits.SYNC = 0;	// use async serial
    TXSTA1bits.BRGH = 1;	// high speed mode
    BAUDCON1bits.BRG16 = 1;	// use 16 bit baud rate generator
    SPBRG1 = (_XTAL_FREQ/baud_rate/4)-1;	// set baud rate generator
    return;
}

unsigned int ADCRead(unsigned char ch)
{
   if(ch>13) return 0;  //Invalid Channel
 //  ADCON0=0x00;
   ADCON0=(ch<<2);   //Select ADC Channel
    ADCON0bits.ADON=1;//switch on the adc module
    __delay_us(5);
    ADCON0bits.GODONE=1;  //Start conversion
    while(ADCON0bits.GODONE); //wait for the conversion to finish
    ADCON0bits.ADON=0;//switch off adc
 

   return ADRES;
   //return (ADRESH << 8) | ADRESL;
}



void config_adc(void)
{
    ANCON0bits.ANSEL0 = 1; // analog input A0 on
    TRISAbits.TRISA0 = 1;//pin A0 input analog
    PORTAbits.RA0 = 0;

    ADCON1bits.TRIGSEL =0b00; // trigger select
    ADCON1bits.VCFG = 0b11;        // 11 on board V4.1, b10 onboard V2.0, 0b01 External vref ,00 AVDD 
    ADCON1bits.VNCFG =0b0; //1 external vss vref-
    ADCON1bits.CHSN =0b000; //analouge chanel select vref- 


    ADCON2bits.ADFM = 0b1;        // 0b1  10 bit ADC result is right justified, 00 left
    ADCON2bits.ACQT = 0b001; //Time select bits Tad
    ADCON2bits.ADCS = 0b100; //clock select bits 

  
    ADCON0bits.ADON = 1;        // ADC is enabled
   
}
void do_voltage(void)
{
     if (heartbeatCountTwo < 255)
    {
        heartbeatCountTwo++;
    }
    else
    {
        heartbeatCountTwo = 0;
       ADCValue=ADCRead(0);     
  //   volts = 7 * ((float)ADCValue)/1024.0; //VCFG = 0b01 VNCFG=0b1
      volts = 5.7 * ((float)ADCValue)/1024.0; //VCFG = 0b11 VNCFG=0b1
      printf("Voltage V%6.2f\r\n", volts);
    
    }
 
}
void main(void) {
    setup();
    unsigned int vale=0;
    unsigned int stats;
    volatile float voltage;  

    config_adc();

    UART1Init(UART_BITRATE);
          puts1USART((char *)Txdata);
    while (1) {
      while (PIR1bits.TX1IF == 0);

  

        do_voltage();
   //   puts1USART('\r');
     // newvals[0]=ftoa(volts,0);
    //   puts1USART(ftoa(volts,0)); //convert to string

      Heartbeat();
      

     //__delay_ms(1000);
     
     //TXREG = 65;
    }


    return;
}
 void putch(char data) {    
     while (!TXIF)    
         continue;    
     TXREG = data;    
 }    
 