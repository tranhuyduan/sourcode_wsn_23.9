#ifndef _CONSOLE_H_
#define _CONSOLE_H_
#include "p24FJ128GA306.h"
#include "generic.h"
	#define ROM					const
	#define memcmppgm2ram(a,b,c)	memcmp(a,b,c)
	#define memcpypgm2ram(a,b,c)	memcpy(a,b,c)
	#define strcpypgm2ram(a, b)		strcpy(a,b)
	#define Reset()					asm("reset")
	#define SLEEP()					Sleep()
	#define CLRWDT()				ClrWdt()
	#define NOP()					Nop()
	#define DISABLE_WDT()			(RCONbits.SWDTEN = 0)
	#define ENABLE_WDT()			(RCONbits.SWDTEN = 1)
        //#define DISABLE_WDT()			(RCONbits.SWDTEN = 1)
  #define CLOCK_FREQ 16000000ul

    //Baudrate of UARTx module
    #define BAUD_RATE 115200ul
    #define BAUD_RATE_UART1 115200ul

    #define WSAN_LED_TRIS 	TRISFbits.TRISF3
    #define WSAN_LED 		LATFbits.LATF3
    void ConsoleInit(void);
    #define ConsoleIsPutReady()     (U2STAbits.TRMT)
    #define ConsoleIsGetReady()		(IFS1bits.U2RXIF)
    void ConsolePut(BYTE c);
    void ConsolePutROMString(ROM char* str);
    BYTE ConsoleGet(void);
    void clearBuffer(void);
void PrintChar(BYTE);
void PrintWord(WORD);
#define printf(x)	ConsolePutROMString((ROM char *)x)

void UART1Init(void);
        void UART1PutROMString(ROM char* str);
        void UART1Put(BYTE c);
        void UART1PrintChar(BYTE toPrint);
        void UART1PrintWord(WORD toPrint);
        BYTE UART1ConsoleGet(void);
        void UARTP1rintDWord(DWORD toPrint);
#define printf1(x)     UART1PutROMString((ROM char *)x)
#endif

