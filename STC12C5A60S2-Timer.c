#include "includes.h"
//定时器0中断程序==================================================
void Time0() interrupt 1
{
	static INT8U cnt1 = 0;
    TL0=0X00;
    TH0=0X4C;//25ms(22.1184)
	cnt1++;
	if(cnt1 >= 10)
	{
		cnt1 = 0;
		fDis = 1;
	}
}

/* --- T1 Series Programmable Clock Output Demo ------------*/


