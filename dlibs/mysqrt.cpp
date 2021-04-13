#include "mysqrt.h"

double mysqrt(double inputValue){
    printf("my sqrt\n");
#if defined (HAVE_LOG) && defined (HAVE_EXP)
    return exp(log(inputValue)*0.5);
#else
    return sqrt(inputValue);
#endif
}