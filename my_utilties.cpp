#include "my_utilties.h"


QTime myTimer;

bool isNegativeClass(double weight)
{
    return weight<=0;
}

bool isPositiveClass(double weight)
{
    return !isNegativeClass(weight);
}
