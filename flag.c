#include "flag.h"

#include <stdbool.h>

static bool flag;

void initiateFlag(){
    flag = 0;
}
bool getFlag(){
    return flag;
}

void flipSwitch(){
    flag = 1;
}