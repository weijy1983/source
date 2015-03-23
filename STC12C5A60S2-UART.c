#include "includes.h"

// 串口1中断
void COM(void) interrupt 4
{
    unsigned char Uart_Temp;
    if((SCON&0x01)==0)
        return;
    SCON&=0xfe;
    ES=0;
    {
        Uart_Temp=SBUF;
        if(Uart_Temp==0xa5)
        {
			Uart_Send_Byte(0x6A);
        }
        if(Uart_Temp==0x50)
        {
            Uart_Send_Byte(0x50);
            Uart_Delay();
            IAP_CONTR=0x60;
        }
    }
    ES=1;
}

/*********串口发送一个字节*********/
void Uart_Send_Byte(INT8U Dat)
{
    ES=0;
    SBUF=Dat;
    while((SCON&0x02)==0x00);//  S2CON
    SCON&=0xfd;
    ES=1;
}

void Uart_Delay()      //下载延时
{
    unsigned int i,j;
    for(i=0; i<1000; i++)
    {
        j=700;
        while(j--);
    }
}