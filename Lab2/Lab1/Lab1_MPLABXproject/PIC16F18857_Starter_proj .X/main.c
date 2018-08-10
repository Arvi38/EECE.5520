/*
 * File:   PIC and Galileo communication          
 *         
 * 
 * simple PIC program example
 * for UMass Lowell 16.480/552
 * 
 * Author: Ioannis
 *
 * Created on 2017/9/21
 */

#include <pic16f18857.h>
#include "mcc_generated_files/mcc.h" //default library 
#define value 0x0
#define MSG_ACK 0xE
#define MSG_NOTHING 0xF
#define ADC_Temp 0b0000000000

/* Circuit Connections
   Signal STROBE   RC6
   Signal D0       RC2
   Signal D1       RC3
   Signal D2       RC4
   Signal D3       RC5
 */
void Timer2_Init(void)
{
// CCPTMRS0 = 0x01; //SELECTED TIMER 2 FOR PWM
T2CON = 0x80; //CONFIGURED TIMER 2
T2CLKCONbits.CS = 0x01; //clk in relation with osc frequency
T2HLT = 0x00; //TIMER MODE
T2RST = 0x00; //Reset Source
PR2 = 0xFF; //lOAD THE PR2 VALUE
TMR2 = 0x00; //PRESCALE VALUE IS 0
PIR4bits.TMR2IF = 0; // CLEAR THE INTERRUPT FLAG
//T2CONbits.ON = 1; // START THE TIMER
}
void PWM_Init(void)
{
CCP1CONbits.EN = 1; // ENABLING THE
CCP1CONbits.FMT = 0; //RIGHT ALLIGNED FORMAT
CCP1CONbits.MODE = 0xF; // SETTING THE MODE TO PWM
CCPR1H = 0x00; // RH TO 0
CCPR1L = 0X00; //RL TO 0
CCPTMRS0 = 0X01; // SELECTS TIMER2
}
void PWM_signal_out(unsigned int duty)
{
    T2CONbits.ON = 1; // START THE TIMER   
PMD3bits.PWM6MD = 0; //PWM 6 is enabled
CCPR1H = duty >>2; // 2 MSB'S IN CCPR1H
CCPR1L = (duty & 0x0003)<<6; //8 LSB'S IN CCPR1L
}
 int ADC_conversion_results()
{
    TRISAbits.TRISA0 = 1; // SETTING PORTA PIN0 TRISTATE REGISTER TO INPUT
    ANSELAbits.ANSA0 = 1; // SETTING PORTA PIN0 AS A ANALOG INPUT
    ADCON0bits.ADON = 1;  // ACTIVATING THE ADC MODULE
    ADCON0bits.GO = 1;    // START CONVERTING
    while(ADCON0bits.ADGO)// WAIT UNTIL THE CONVERSION
    {
    }
    int b = (ADRESH<<8)+(ADRESL); // MAKE THE ADC RESULT IN 10BITS 
      ADCON0bits.GO = 0; // STOP CONVERTING
    return(b); // RETURN THE RESULT VALUE 
}

void ADC_Init(void)  
{
    ADCON1 = 0x00; // setting control register 1 to 0
    ADCON2 = 0x00; // setting control register 2 to 0
    ADCON3 = 0x00; // setting control register 3 to 0
    ADSTAT = 0x00; // setting threshold register and not overflowed  to 0
    ADCAP  = 0x00; // disabling ADC capacitors
    ADACT  = 0x00; // disabling Auto conversion trigger control register
    ADPRE  = 0x00; // setting precharge time control to 0 
    ADCLK  = 0x00; // setting ADC clk
    ADREF  = 0x00; // setting ADC positive and negative reference voltages 
    ANSELAbits.ANSA0 = 1; // setting ADC analog channel input to 1
    ADCON0 = 0x84; // setting ADCON0 to the required mode.
    
}
 
void set_receive()
{  
   //1.set RC6 as digital input
   //2.set RC2, RC3, RC4 and RC5 as digital inputs
   //TRISC = 0xFF; 
    ANSELB = 0x00;
   TRISBbits.TRISB0=1;
   TRISBbits.TRISB1=1;
   TRISBbits.TRISB2=1;
   TRISBbits.TRISB3=1;
   TRISBbits.TRISB4=1;
}

void set_send()
{
    ANSELB = 0x00;
    TRISBbits.TRISB0= 0;
    TRISBbits.TRISB1= 0;
    TRISBbits.TRISB2= 0;
    TRISBbits.TRISB3= 0;
}

unsigned char receive_msg()
{
    /* 1.wait strobe high
    2.wait strobe low
    3.read the data
    4.wait strobe high
    5.return the data*/
    set_receive();
    while(PORTBbits.RB4 == 0);
    unsigned char message = 0x00;
     message = ((PORTBbits.RB0)| (PORTBbits.RB1<<1) | (PORTBbits.RB2<<2) | (PORTBbits.RB3<<3));
    while(PORTBbits.RB4 == 1);
    return message;
}


void Strobe(char message)
{
      ANSELB = 0x00;
    TRISBbits.TRISB0=0;
    TRISBbits.TRISB1=0;
    TRISBbits.TRISB2=0;
    TRISBbits.TRISB3=0;
    TRISBbits.TRISB4 = 1;
    /*PORTBbits.RB0 = 1;
    PORTBbits.RB1 = 1;
    PORTBbits.RB2 = 1;
    PORTBbits.RB3 = 1;
      */
    while(PORTBbits.RB4==1);
    LATBbits.LATB0 = message & 0x01;
    LATBbits.LATB1 = (message>>1)&0x01;
    LATBbits.LATB2 = (message>>2)&0x01;
    LATBbits.LATB3 = (message>>3)&0x01;
    while(PORTBbits.RB4==0);
}


void SendADC(int ADCValue)
{
   
    set_send();
    char a = (ADCValue & 0x0F);
    char b = (ADCValue & 0xF0)>>4;
    char c = (ADCValue & 0x300)>>8;
    Strobe(a);
    Strobe(b);
    Strobe(c);
   }
// Main program
void main (void)
{
    
    int ADC;
    SYSTEM_Initialize();
    ADC_Init();
    Timer2_Init();
    PWM_Init();
    TRISCbits.TRISC2=0;
    unsigned char msg;
//    ANSELC =0;    
    while(1)
    {  
    msg=receive_msg();
    switch(msg)
    {
        //Reset
        case 0x00:
          Strobe(MSG_ACK);
          __delay_ms(1000);
          SYSTEM_Initialize();
          break;
          //PING
        case 0x01:
            Strobe(MSG_ACK);
            break;
         //Get
        case 0x02:
            Strobe(MSG_ACK);
            ADC = ADC_conversion_results();
            SendADC(ADC);
            break;
        //TURN 30
        case 0x03:
            PWM_signal_out(100);
            for(int i=0;i<=35;i++)
            {
                PORTCbits.RC2 = 1;
                __delay_ms(3.500);
                PORTCbits.RC2 = 0;
                __delay_ms(16.500);
            }
            break;
        //TURN 90    
        case 0x04:
            for(int i=0;i<=35;i++)
            {
                PORTCbits.RC2 = 1;
                __delay_ms(3.500);
                PORTCbits.RC2 = 0;
                __delay_ms(16.500);
            }
            break;
        //TURN 120    
        case 0x05:
            for(int i=0;i<=35;i++)
            {
                PORTCbits.RC2 = 1;
                __delay_ms(3.500);
                PORTCbits.RC2 = 0;
                __delay_ms(16.500);
            }
            break;
    } 
    }
}
