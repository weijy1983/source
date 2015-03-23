#include "includes.h"

#define INT8U  unsigned char
#define INT16U unsigned int

sbit DAT = P0^3;
sbit CLK = P0^4;

INT8U code DisMap[]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09,0x61,0xf3,0xfb};//数码管编码,61是E，F3是r,FB是-
//双字节定点数转换为5位bcd码,数组低位存数据高位.
void ToBCD16Bit(INT16U reg, INT8U *sp)
{   
	INT8U x;
    x=0;
    while(reg>=10000) {
        reg-=10000;
        x++;
    }
    *sp=x;
    x=0;
    while(reg>=1000) {
        reg-=1000;
        x++;
    }
    *(sp+1)=x;
    x=0;
    while(reg>=100) {
        reg-=100;
        x++;
    }
    *(sp+2)=x;
    x=0;
    while(reg>=10) {
        reg-=10;
        x++;
    }
    *(sp+3)=x;
    *(sp+4)=reg;
}

//向164发送一个字节
void PutChar164(INT8U dat)
{
    INT8U i;
	INT8U d;
    d = dat;
    for(i=8; i>0; i--)
    {
        CLK=0;
        DAT=d&0x01;
        CLK=1;
        d=d>>1;
    }
    CLK=0;
}
// 发送4位数据，从低位到高位
void SendStr164(INT8U *str, INT8U len)
{
	INT8U i;
	for(i=len;i>0;i--)
	{
		PutChar164(DisMap[*(str+i)]);
	}
}