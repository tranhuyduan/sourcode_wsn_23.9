#include "p24FJ128GA306.h"
#include "libpic30.h"
#include "console.h"
#include "PTC06.h"
    _CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & LPCFG_OFF & ICS_PGx1 & WINDIS_OFF & FWDTEN_WDT_SW &  FWPSA_PR128 & WDTPS_PS512);
    _CONFIG2(IESO_ON & FNOSC_PRI & FCKSM_CSECME & OSCIOFCN_OFF & IOL1WAY_OFF & POSCMD_HS);
    _CONFIG3(BOREN_ON);
 BYTE data;
    int main(void){

    BYTE buffer;
   
    //__de
     //OSCCON = 0x2001;//current osc HS, new osc FRC
    __delay_ms(2000);
    CLKDIV = 0x0000;//Fcy = Fosc/2

    /************Maping Pins************/
    //unlock registers
    asm volatile ("mov #OSCCONL, w1  \n"
        "mov #0x46, w2     \n"
        "mov #0x57, w3     \n"
        "mov.b w2, [w1]    \n"
        "mov.b w3, [w1]    \n"
        "bclr OSCCON, #6");
     /*------UART1-----*/
    // RX1 >> RP22
    RPINR18bits.U1RXR = 22;
    // TX1 >> RP25
    RPOR12bits.RP25R  = 3;
        /*------UART2-----*/
    // RX2 >> RP24
    RPINR19bits.U2RXR = 24;
    // TX2 >> RP23
    RPOR11bits.RP23R  = 5;
      //lock register
    asm volatile ("mov #OSCCONL, w1  \n"
        "mov #0x46, w2     \n"
        "mov #0x57, w3     \n"
        "mov.b w2, [w1]    \n"
        "mov.b w3, [w1]    \n"
        "bset OSCCON, #6");

    CLRWDT();
    ENABLE_WDT();
    WSAN_LED_TRIS = 0;
    WSAN_LED = 1;
    
    UART1Init();
    ConsoleInit();
   // sendReset();
    
   
   //photoInit();
   //sendReset();
  // sendCleancache() ;
  //  __delay_ms(100);
    //demotakePhoto(buffer);
    while(1){
        CLRWDT();
    };
    return 1;
}
void _ISR __attribute__((interrupt, auto_psv)) _U2RXInterrupt(void)
{
    IFS1bits.U2RXIF = 0;
}
void _ISR __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void)
{
     IFS0bits.U1RXIF = 0;//enable UART1 interrupt
    data = U1RXREG;
    if(data == '1'){
    //setBaudRate();
    sendPhotoSize();
    sendCompressRate();
    sendCleancache();
    sendStartPhoto();
    sendGetLength();
    }
    if(data == '2'){
    sendGetPhoto();
       }
}