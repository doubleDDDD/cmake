// 计算一个数的平方根
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TutorialConfig.h"
#ifdef USE_MYMATH
#include "mysqrt.h"
#endif

int main(int argc, char *argv[])
{
    if (argc < 2){
        fprintf(stdout,"%s Version %d.%d\n", argv[0], Tutorial_VERSION_MAJOR, Tutorial_VERSION_MINOR);
        fprintf(stdout,"Usage: %s number\n",argv[0]);
        return 1;
    }
    double inputValue = atof(argv[1]);
    double outputValue;
    if(inputValue < 0){
        outputValue = 0;
        goto out;
    }
#ifdef USE_MYMATH
    outputValue = mysqrt(inputValue);
#else
    outputValue = sqrt(inputValue);
#endif
out:
    fprintf(stdout,"The square root of %g is %g\n", inputValue, outputValue);
    return 0;
}