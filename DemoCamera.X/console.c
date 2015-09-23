#include "console.h"

    void ConsoleInit(void)
    {
        U2BRG  = (CLOCK_FREQ/2/4)/BAUD_RATE-1;
        U2MODE = 0x8008;//enable UART2
                        //U2TX and U2RX pins are enabled and used
                        //Standard Speed mode (16 BRG clock cycles per bit)
                        //8-bit data, no parity
                        //One Stop bit

        U2STA = 0x2400;//Transmit is enabled, U2TX pin is controlled by UART2
        IEC1bits.U2RXIE = 1;
        IFS1bits.U2RXIF = 0;         /*clear interrupt flag*/
    }
    /* End by dat_a3cbq91 */
    void clearBuffer(void){
        U2TXREG = 0;
    }
    void ConsolePutROMString(ROM char* str)
    {
        BYTE c;

        while( (c = *str++) != 0 )
        ConsolePut(c);
    }

    void ConsolePut(BYTE c)
    {
        while(U2STAbits.TRMT == 0);
        U2TXREG = c;
    }

    ROM unsigned char CharacterArray[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    void PrintChar(BYTE toPrint)
    {
        BYTE PRINT_VAR;
        PRINT_VAR = toPrint;
        toPrint = (toPrint>>4)&0x0F;
        ConsolePut(CharacterArray[toPrint]);
        toPrint = (PRINT_VAR)&0x0F;
        ConsolePut(CharacterArray[toPrint]);
        return;
    }

    //programming by dat_a3cbq91
    void PrintWord(WORD toPrint)
    {
        BYTE MSB,LSB;
        MSB = toPrint >> 8;
        LSB = toPrint & 0x00FF;
        PrintChar(MSB);
        PrintChar(LSB);
    }
    //end by dat_a3cbq91

    BYTE ConsoleGet(void)
    {
        char Temp;

//        while(IFS1bits.U2RXIF == 0);
        while(U2STAbits.URXDA == 0);
        Temp = U2RXREG;
//        IFS1bits.U2RXIF = 0;
        return Temp;
    }
    void UART1Init(void)
        {
            U1BRG  =(CLOCK_FREQ/2/4)/BAUD_RATE_UART1-1;

            IEC0bits.U1RXIE = 1;
            IFS0bits.U1RXIF = 0;//enable UART1 interrupt

            U1MODE = 0x8008;//enable UART1
                            //U1TX and U1RX pins are enabled and used
                            //Standard Speed mode (16 BRG clock cycles per bit)
                            //8-bit data, no parity
                            //One Stop bit
            U1MODEbits.BRGH =1;
            U1STA = 0x0400;//Transmit is enabled, U1TX pin is controlled by UART1
        }

        void UART1Put(BYTE c)
        {
            while(U1STAbits.TRMT == 0);
            U1TXREG = c;
        }

        void UART1PutROMString(ROM char* str)
        {
            BYTE c;

            while( (c = *str++) != 0 )
            UART1Put(c);
        }

        void UART1PrintChar(BYTE toPrint)
        {
            BYTE PRINT_VAR;
            PRINT_VAR = toPrint;
            toPrint = (toPrint>>4)&0x0F;
            UART1Put(CharacterArray[toPrint]);
            toPrint = (PRINT_VAR)&0x0F;
            UART1Put(CharacterArray[toPrint]);
            return;
        }
        

        void UART1PrintWord(WORD toPrint)
        {
            BYTE MSB,LSB;
            MSB = toPrint >> 8;
            LSB = toPrint & 0x00FF;
            UART1PrintChar(MSB);
            UART1PrintChar(LSB);
        }
        void UART1PrintDWord(DWORD toPrint)
        {
            WORD N_MSB, N_LSB;
            N_MSB = toPrint>>16;
            N_LSB = toPrint & 0x0000FFFF;
            UART1PrintWord(N_MSB);
            UART1PrintWord(N_LSB);
        }

        BYTE UART1ConsoleGet(void)
        {
            char Temp;

            //while(IFS0bits.U1RXIF == 0);
        while(U1STAbits.URXDA == 0);
        Temp = U1RXREG;
//        IFS1bits.U2RXIF = 0;
        return Temp;
        }


