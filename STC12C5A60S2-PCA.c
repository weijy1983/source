#include "includes.h"

void PCA_isr() interrupt 7 using 1
{
    CCF0 = 0;                       //Clear interrupt flag
    CCAP0L = PCAvalue;
    CCAP0H = PCAvalue >> 8;            //Update compare value
    PCAvalue += tCar;
}