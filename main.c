#include <stdio.h>
#include "IPv4.h"

int main (int argc, char *argv[], char *env[]) {
init();
runTests((void *)0); // function cleaning structure at end

uint32_t uVar1;
typedef unsigned long ulong;

clear();
init();
print();
uVar1 = add(0xaaaaaaaa,0x0);
uVar1 = add(0xbbbbbbbb,0x0);
print();
print_asIPV4();
uVar1 = size();
printf("Size for python %u\n",(ulong)uVar1);

}

