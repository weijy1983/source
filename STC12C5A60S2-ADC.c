#include "includes.h"
//---------与STC12C5A32S2单片机ADC相关的寄存器声明------------------
/*sfr                P1ASF            =0x9d;                //P1口模数转换功能控制寄存器
sfr                ADC_CONTR        =0xbc;                //AD转换控制寄存器
sfr                ADC_RES          =0xbd;                //AD转换结果寄存器高
sfr                ADC_RESL         =0xbe;                //AD转换结果寄存器低
sfr                AUXR1            =0xa2;                //AD转换结果存储方式控制位
sfr                P1M1             =0x91;
sfr                P1M0             =0x92;*/
//------------------------------------------------------------------
//P1ASF寄存器：8位，对应P1口8根口线,用于指定哪根口线用作ADC功能
//哪个口用作ADC就应置相应的位为“1”，注意：不能位寻址
//------------------------------------------------------------------
#define                ADC_POWER       0x80   //ADC电源开
#define                ADC_SPEED       0x60   //设为90个时钟周期ADC一次
#define                ADC_START       0x08   //ADC启动控制位设为开
#define                ADC_FLAG        0x10   //ADC结束标志位
/*
ADC_CTRL寄存器:
ADC_POWER   SPEED1  SPEED0   ADC_FLAG   ADC_STAR   CHS2 CHS1 CHS0
    1          2      3         4          5         6    7    8
第 1 位:  =1 打开ADC电源；=0 关闭ADC电源；  ADC前要一定要打开
第2-3位:  =1 1  90个时钟周期ADC一次；    =1 0 180个时钟周期ADC一次；
                  =0 1 360个时钟周期ADC一次；    =0 0 540个时钟周期ADC一次；
第4位:          ADC结束标志位，每次ADC结束时自动=1，需要用软件清零才可以进行下一次ADC
第5位:          ADC启动控制位，置“1”则ADC转换开始，转换结束后为0
第6-7-8位:ADC通道选择 000-->P1.0 ........111-->P1.7
*/
//-------------------------------------------------------------
void ADC_int(unsigned char n)        //第n通道ADC初始化函数
{   n&=0x07;            //确保n=0----7通道
    AUXR1|=0x04;         //转换结果存储格式：数据的高2位放ADC_RES,低8位放ADC_RESL
    P1ASF=P1ASF|(1<<n);          //将P1.n设为ADC采样功能
}
//---------------------------------------------------------------
INT16U ADC_GET(unsigned char n)     //第n通道ADC采样函数
{   unsigned int adc_data;
    n&=0x07;                  //确保n=0----7通道
    ADC_RES=0;                //清零
    ADC_RESL=0;               //清零
    ADC_CONTR=0;              //清零，以便重构
    //ADC_CONTR|=(ADC_POWER|ADC_SPEED|n|ADC_START);      //打开AD转换电源、设定转换速度、设定通道号、AD转换开始
    //ADC_CONTR=0xE9;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();           //延时4个时钟周期左右
    //ADC_CONTR=0xE9;
    switch(n)
    {
    case 0:
        ADC_CONTR=0xE8;
        break;
    case 1:
        ADC_CONTR=0xE9;
        break;
    case 2:
        ADC_CONTR=0xEA;
        break;
    case 3:
        ADC_CONTR=0xEB;
        break;
    case 4:
        ADC_CONTR=0xEC;
        break;
    case 5:
        ADC_CONTR=0xED;
        break;
    case 6:
        ADC_CONTR=0xEE;
        break;
    case 7:
        ADC_CONTR=0xEF;
        break;
    default:
        ADC_CONTR=0xE8;
    }

    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();


    while(!(ADC_CONTR&ADC_FLAG));                      //等待转换结束 =0x10   ADC_FLAG位=1
    //adc_data=(ADC_RES&0x03)*256+ADC_RESL;

    adc_data=ADC_RES<<8|ADC_RESL;                     //转换结果计算,取高位结果存储器的低2位 + 低位结果存储器
    ADC_CONTR&=~ADC_FLAG;                             //清零转换结束标志位(ADC_FLAG位=0)
    return adc_data;                                  //返回ADC的值（0----1023）
}
//======================================
