#include "includes.h"
//---------��STC12C5A32S2��Ƭ��ADC��صļĴ�������------------------
/*sfr                P1ASF            =0x9d;                //P1��ģ��ת�����ܿ��ƼĴ���
sfr                ADC_CONTR        =0xbc;                //ADת�����ƼĴ���
sfr                ADC_RES          =0xbd;                //ADת������Ĵ�����
sfr                ADC_RESL         =0xbe;                //ADת������Ĵ�����
sfr                AUXR1            =0xa2;                //ADת������洢��ʽ����λ
sfr                P1M1             =0x91;
sfr                P1M0             =0x92;*/
//------------------------------------------------------------------
//P1ASF�Ĵ�����8λ����ӦP1��8������,����ָ���ĸ���������ADC����
//�ĸ�������ADC��Ӧ����Ӧ��λΪ��1����ע�⣺����λѰַ
//------------------------------------------------------------------
#define                ADC_POWER       0x80   //ADC��Դ��
#define                ADC_SPEED       0x60   //��Ϊ90��ʱ������ADCһ��
#define                ADC_START       0x08   //ADC��������λ��Ϊ��
#define                ADC_FLAG        0x10   //ADC������־λ
/*
ADC_CTRL�Ĵ���:
ADC_POWER   SPEED1  SPEED0   ADC_FLAG   ADC_STAR   CHS2 CHS1 CHS0
    1          2      3         4          5         6    7    8
�� 1 λ:  =1 ��ADC��Դ��=0 �ر�ADC��Դ��  ADCǰҪһ��Ҫ��
��2-3λ:  =1 1  90��ʱ������ADCһ�Σ�    =1 0 180��ʱ������ADCһ�Σ�
                  =0 1 360��ʱ������ADCһ�Σ�    =0 0 540��ʱ������ADCһ�Σ�
��4λ:          ADC������־λ��ÿ��ADC����ʱ�Զ�=1����Ҫ���������ſ��Խ�����һ��ADC
��5λ:          ADC��������λ���á�1����ADCת����ʼ��ת��������Ϊ0
��6-7-8λ:ADCͨ��ѡ�� 000-->P1.0 ........111-->P1.7
*/
//-------------------------------------------------------------
void ADC_int(unsigned char n)        //��nͨ��ADC��ʼ������
{   n&=0x07;            //ȷ��n=0----7ͨ��
    AUXR1|=0x04;         //ת������洢��ʽ�����ݵĸ�2λ��ADC_RES,��8λ��ADC_RESL
    P1ASF=P1ASF|(1<<n);          //��P1.n��ΪADC��������
}
//---------------------------------------------------------------
INT16U ADC_GET(unsigned char n)     //��nͨ��ADC��������
{   unsigned int adc_data;
    n&=0x07;                  //ȷ��n=0----7ͨ��
    ADC_RES=0;                //����
    ADC_RESL=0;               //����
    ADC_CONTR=0;              //���㣬�Ա��ع�
    //ADC_CONTR|=(ADC_POWER|ADC_SPEED|n|ADC_START);      //��ADת����Դ���趨ת���ٶȡ��趨ͨ���š�ADת����ʼ
    //ADC_CONTR=0xE9;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();           //��ʱ4��ʱ����������
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


    while(!(ADC_CONTR&ADC_FLAG));                      //�ȴ�ת������ =0x10   ADC_FLAGλ=1
    //adc_data=(ADC_RES&0x03)*256+ADC_RESL;

    adc_data=ADC_RES<<8|ADC_RESL;                     //ת���������,ȡ��λ����洢���ĵ�2λ + ��λ����洢��
    ADC_CONTR&=~ADC_FLAG;                             //����ת��������־λ(ADC_FLAGλ=0)
    return adc_data;                                  //����ADC��ֵ��0----1023��
}
//======================================
