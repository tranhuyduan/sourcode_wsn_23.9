#include "PTC06.h"
//#include "delay32.h"
BYTE length_photo[2];
DWORD photo_len;
BYTE reset_cmd[] =
{
	0x56,
	0x00,
	0x26,
	0x00,
        0x00
};
BYTE reset_rcv [] =
{
	0x76,
	0x00,
	0x26,
	0x00
};
BYTE start_photo_cmd [] =
{
	0x56,
	0x00,
	0x36,
	0x01,
	0x00
};
BYTE start_photo_rcv [] =
{
	0x76,
	0x00,
	0x36,
	0x00,
	0x00
};
BYTE read_len_cmd [] =
{
	0x56,
	0x00,
	0x34,
	0x01,
	0x00
};
BYTE read_len_rcv [] =
{
	0x76,
	0x00,
	0x34,
	0x00,
	0x04,
	0x00,
	0x00
};
BYTE get_photo_cmd [] =
{
	0x56,
	0x00,
	0x32,
	0x0C,
	0x00,
	0x0A,

	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,

	0xFF,
	0xFF

};
BYTE get_photo_rcv [] =
{
	0x76,
	0x00,
	0x32,
	0x00,
	0x00
};

#ifdef ZOOM_SIZE
BYTE zoom_size_cmd [] =
{
	0x56,
	0x00,
	0x54,
	0x01,
	0x11,
        0x00
};
#endif

BYTE zoom_size_rcv [] =
{
	0x76,
	0x00,
	0x54,
	0x00,
	0x00,
};
BYTE stop_photo_cmd [] =
{
	0x56,
	0x00,
	0x36,
	0x01,
	0x03
};

BYTE stop_photo_rcv [] =
{
	0x76,
	0x00,
	0x36,
	0x00,
	0x00
};
#ifdef SIZE_640x480
BYTE photo_size_cmd [] =
{
	0x56,
	0x00,
	0x31,
	0x05,
	0x04,
	0x01,
	0x00,
	0x19,
	0x00,
	0x00
};

#endif
#ifdef SIZE_320x240
BYTE photo_size_cmd [] =
{
	0x56,
	0x00,
	0x31,
	0x05,
	0x04,
	0x01,
	0x00,
	0x19,
	0x11,
	0x00
};
#endif
BYTE photo_size_rcv [] =
{
	0x76,
	0x00,
	0x31,
	0x00,
	0x00
};
BYTE save_power_cmd [] =
{
	0x56,
	0x00,
	0x3E,
	0x03,
	0x00,
	0x01,
	0x00
};
BYTE save_power_rcv [] =
{
	0x76,
	0x00,
	0x3E,
	0x00,
	0x00
};
BYTE compress_rate_cmd [] =
{
	0x56,
	0x00,
	0x31,
	0x05,
	0x01,
	0x01,
	0x12,
	0x04,
	0x80
};

BYTE compress_rate_rcv [] =
{
	0x76,
	0x00,
	0x31,
	0x00,
	0x00
};
BYTE set_bps_cmd [] =
{
	0x56,
	0x00,
	0x24,
	0x03,
	0x01,
#ifdef BAUD_RATE_115200
	0x0D,
	0xA6
#endif
#ifdef  BAUD_RATE_9600
        0xAE,
        0xC8
#endif
#ifdef  BAUD_RATE_19200
        0x56,
        0xE4
#endif
#ifdef  BAUD_RATE_38400
        0x2A,
        0xF2
#endif
#ifdef  BAUD_RATE_57600
        0x1C,
        0x4C
#endif
};
BYTE set_bps_rcv [] =
{
	0x76,
	0x00,
	0x24,
	0x00,
	0x00
};

BYTE clean_cache_cmd[]={
        0x56,
        0x00,
        0x36,
        0x01,
        0x03
};
BYTE clean_cache_rcv[]={
        0x76,
        0x00,
        0x36,
        0x00,
        0x00
};


void sendByte(BYTE byte){
    ConsolePut(byte);
}
// gui lenh xuong cmr
void sendComand(BYTE buffer[], WORD len){
    WORD i = 0;
    for(i = 0; i<len; i++){
        sendByte(buffer[i]);
    }
   
}
// nhan data tu cmr
BYTE getCommand( BYTE *buffer, WORD len){
    DWORD i=0;
    for(i = 0;i<len; i++){
          buffer[i]= ConsoleGet();
    }
   if(buffer[0] == CMD_CMR)
        return 1;
    else
        return 0;


}

// reset cmr

BYTE sendReset(void){
    BYTE buffer[10];
    BYTE i = 0;
    UART1PutROMString("send Reset Photo\n");
    sendComand(reset_cmd, 5);
   // __delay_ms(100);
    if(!getCommand(buffer, 4)){
       for(i =0;i<4;i++)
         UART1Put(buffer[i]);
       UART1PutROMString("send Reset Photo Error\n");
       return 0;
    }
    else{
    for( i =0; i<4; i++){
        //UART1Put(buffer[i]);
        if(buffer[i]!= reset_rcv[i]) return 0;
      }
    UART1PutROMString("send Reset Photo End\n");
    return 1;
    }
   
}
BYTE sendGetLength(void){
    BYTE buffer[7];
    BYTE i = 0;
    UART1PutROMString("send Get Length\n");
    sendComand(read_len_cmd, 5);
    if(!getCommand(buffer, 9)) return 0;
    for(i = 0; i<7; i++){
        UART1Put(buffer[i]);
        if(buffer[i]!=read_len_rcv[i]){
             UART1PutROMString("send Get Length error\n");
            return 0;
             }
    }
    setPhotoLen(buffer[7], buffer[8]);
    UART1PutROMString("\n");
    for(i = 0; i<9; i++){
        UART1PrintChar(buffer[i]);
         }
    UART1PutROMString("\nsend Get Length end\n");
    return 1;
}
void setPhotoLen(BYTE len1, BYTE len2){
    DWORD temp;
    temp  = len1*0x100 +len2;
    get_photo_cmd[12] =len1;
    get_photo_cmd[13] = len2;
    photo_len =temp;
    UART1PutROMString("photo_len=");
    UART1Put(photo_len);
   // UART1PrintChar(photo_len);
}
BYTE sendStartPhoto(void){
    BYTE buffer[5];
    BYTE i = 0;
    UART1PutROMString("\nSend start photo\n");
    sendComand(start_photo_cmd, 10);
    if(!getCommand(buffer, 5)) return 0;
    for(i = 0; i<5; i++){
        UART1PrintChar(buffer[i]);
        if(buffer[i]!= start_photo_rcv[i]){
             UART1PutROMString("Send start photo Error\n");
            return 0;
        }
          
    }
            //UART1PrintChar(buffer[i]);
    UART1PutROMString("Send start photo End\n");
    return 1;
}
DWORD sendGetPhoto(void){
    BYTE *buf;
    UART1PutROMString("Begining start capture \r\n\r");
    DWORD i = 0;
    BYTE j = 0;
    BYTE result;  
    DWORD temp;
    temp=10+photo_len;
    BYTE txbuffer[5];
    BYTE txbuffer1[7500];
    BYTE txbuffer2[7500];
    //if(*photo_len >8000) return 0;
    //for(i = 0;i<16;i++)
    //UART1PrintChar(get_photo_cmd[i]);
    //UART1PutROMString("start get photo \r\n");
    //UART1PrintChar(temp);
    sendComand(get_photo_cmd, 16);
    //__delay_ms(50);
    //UART1PutROMString("to step 1\r\n");
    //if(photo_len >8000)
    //UART1PutROMString(">8000 \n");
    //UART1PutROMString("to step 2\n");
    
  // result = getCommand(txbuffer, 6);
    buf = txbuffer;
    result = getCommand(buf, 5);

    

//    for(i = 0; i<20; i++){
//        buf = *(txbuffer1+i);
//        getCommand(buf, 200);
//    }

 
    /**buf = txbuffer1;
        getCommand(buf, 200);
        buf = txbuffer2;
        getCommand(buf, 200);*/
         //buf = txbuffer2;
       // getCommand(buf, 255);
       //  buf = txbuffer3;
      //  getCommand(buf, 255);
   // }
    
   
    //buf = txbuffer2;
  //  getComand1(buf);
   // __delay_ms(3000);
    if(!result){
        return 0;
    }
    UART1PutROMString("Printf 5 first bytes\r\n\r");
    for(i = 0; i<5; i++){
         UART1PrintChar(txbuffer[i]);
        if(txbuffer[i]!=get_photo_rcv[i]){
            UART1PutROMString("error 2\n");
            return 0;
        }
    }
        
     //UART1PrintDWord(temp);
    UART1PutROMString("Printf 100 first bytes\r\n\r");
    //for(j = 0; j<20; j++)
     for(i = 0; i<7500; i++){
             UART1PrintChar(txbuffer1[i]);
             UART1PrintDWord("aaaaaaaaa\r");
         }
    for(i = 0; i<7000; i++){
             UART1PrintChar(txbuffer2[i]);
             //UART1PrintChar(result);
         }

   // }
    //for(i = 0; i<5; i++)
        //if(*(rxbuf
   // }
    //for(i = 0; i<5fer +i +5)!=get_photo_cmd[i]) return 0;
   /** if (txbuffer[5] != 0xFF || txbuffer[6] != 0xD8 ||
		txbuffer[3+*photo_len] != 0xFF ||txbuffer[4+*photo_len] != 0xD9)
    {
	UART1PutROMString("error 3\n");
        return 0;}
    //for (i = 0; i < *photo_len; i++)
		//buffer[i] = buffer[i+5];*/
    //UART1PrintChar(result);
    UART1PutROMString("get photo enddddd\n");
    return 1;
}
DWORD takePhoto(BYTE *buffer){
    DWORD len;
    //if(!photoInit()) goto error_exit;
    if(!sendStartPhoto()) goto error_exit;
    if(!sendGetLength()) goto error_exit;
   // len = sendGetPhoto(buffer);
    if(len = 0) goto error_exit;
    return len;
    error_exit:
    return 0;
}
DWORD demotakePhoto(BYTE *buffer){
    sendStartPhoto();
     //__delay_ms(10);
    sendGetLength();
      //__delay_ms(10);
    //sendGetPhoto(buffer);
}

void sendBuffer(BYTE const *buffer, WORD len){
    int i = 0;
    for(i = 0; i<len; i++){
        ConsolePut(buffer[i]);
    }
}
WORD getBuffer(BYTE *buffer, WORD len){
    WORD i =0;
    for(i = 0; i<len; i++)
        buffer[i] = ConsoleGet();
    if(buffer[0] == CMD_CMR)
        return 1;
    else
        return 0;
}

BYTE sendStopPhoto(void){
    BYTE i;
    BYTE buffer[5];
    UART1PutROMString("Send stop photo \n");
    sendComand(stop_photo_cmd, 5);
    if(!getCommand(buffer, 5)) return 0;
    for(i = 0; i<5; i++)
        //UART1Put(buffer[i]);
        if(buffer[i]!=stop_photo_rcv[i]){
         UART1PutROMString("Send stop photo error\n");
            return 0;
        }
    UART1PutROMString("Send stop photo end\n");
    return 1;
}
BYTE sendPhotoSize(void)
{
    BYTE i;
    BYTE buffer[5];
    UART1PutROMString("Send photo Size\r\n");
    sendComand(photo_size_cmd, 10);
    if(!getCommand(buffer, 5)){
        return 0;
    }
    for(i = 0; i<5; i++)
       // UART1Put(buffer[i]);
        if(buffer[i] !=photo_size_rcv[i]){
            UART1PutROMString("Send photo size Error\n");
            return 0;
            
        }
    UART1PutROMString("Send Photo Size End\r\n");
    return 1;
}
BYTE sendCleancache(void){
     BYTE i;
     BYTE buffer[5];
    UART1PutROMString("send Clean cache\n");
     sendComand(clean_cache_cmd,5);
     if (!getCommand(buffer, 5)) {
       return 0;
    }
	for (i = 0; i < 5; i++){
               // UART1Put(buffer[i]);
		if (buffer[i] != clean_cache_rcv[i]){
                 UART1PutROMString("send Clean cache error\n");
                    return 0;
                }
        }
     UART1PutROMString("send Clean cache  end\n");
	return 1;
}
BYTE sendZoomSize(void)
{
    BYTE i;
    BYTE buffer[5];
#ifdef ZOOM_SIZE
    sendComand(zoom_size_cmd, 6);
#endif
    if(!getBuffer(buffer, 5)) return 0;
    for(i = 0; i<5; i++)
        if(buffer[i]!= zoom_size_rcv[i]) return 0;
    return 1;
}
BYTE sendSavePower(void)
{
    BYTE i;
    BYTE buffer[5];
    sendComand(save_power_cmd, 7);
    if(!getBuffer(buffer, 5)) return 0;
    for(i = 0; i<5; i++)
        if(buffer[i]!= save_power_rcv[i]) return 0;
    return 1;
}
BYTE photoInit(void)
{
     UART1PutROMString("Photo Init \n");
     //if(!sendReset()) goto error;
#ifdef ZOOM_SIZE
    if(!sendZoomSize()) goto error;
#endif
    if(!sendReset()) goto error;
    if(!sendPhotoSize()) goto error;
    if(!sendStopPhoto()) goto error;
    if(!sendCompressRate()) goto error;
     if(!sendCleancache()) goto error;
    UART1PutROMString("Photo Init End\n");
    return 1;
    error:
    UART1PutROMString("Photo Init Error\n");
    return 0;
}
BYTE sendCompressRate(void){
    BYTE i;
    BYTE buffer[5];
    UART1PutROMString("Send Compress Photo\n");
     sendComand(compress_rate_cmd, 9);
    if(!getBuffer(buffer, 5)) return 0;
    for(i = 0; i<5; i++){
       // UART1Put(buffer[i]);
        if(buffer[i]!= compress_rate_rcv[i]){
            UART1PutROMString("Send Compress photo Error\n");
            return 0;
        }
    }
     UART1PutROMString("Send Compress Photo End\n");
    return 1;

}
BYTE setBaudRate(void)
{
    BYTE i;
    BYTE buffer[5];
    sendComand(set_bps_cmd, 7);
        

    if(!getBuffer(buffer, 5)) return 0;
    for(i = 0; i<5; i++)
        if(buffer[i]!= set_bps_rcv[i]) return 0;
        //UART1PutROMString("set baurate1\n");

    return 1;

}
WORD convertHextoDec(BYTE msb, BYTE lsb){
    WORD result;
    //result = msb *
}
BYTE demo(void){
}