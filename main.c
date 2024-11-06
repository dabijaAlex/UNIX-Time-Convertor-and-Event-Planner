#include <stdio.h>

int main () {
    unsigned int x = 1328;
    unsigned int mask = 481;
    unsigned y = 0;
    unsigned final = 0;
    // y= mask & x;
    // printf("%d\n",y);
    // final = y >> 5;
    // printf("%d",final);
    // mask = mask >> 1;
    // printf("%u", mask);
    for(int i = 0; i < 10; ++i)
        printf("%d", i);
    return 0;
}