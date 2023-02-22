#include <stdio.h>

int main() {
    unsigned long int num = 0x123456789ABCDEF;
    int count = __builtin_popcountl(num);
    printf("Number of 1 bits in 0x%016lX: %d\n", num, count);
    return 0;
}
