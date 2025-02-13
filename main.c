#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>

#include "test_macros_uint8.h"
#include "test_macros_uint16.h"
#include "test_macros_uint32.h"

#include "IPv4.h"

int main (int argc, char *argv[], char *env[]) {

(void)argc;
(void)argv;
(void)env;

void *handle;
handle = dlopen ("../IPv4.so", RTLD_NOW | RTLD_GLOBAL);
if (!handle) {
  fprintf(stderr, "%s\n", dlerror());
  exit(EXIT_FAILURE);
}
dlerror();

init();

runTests((void *)0); // function cleaning structure at end

clear();

init();

print();

uint32_t uVar1;

uVar1 = add(0xaaaaaaaa,0x0);
uVar1 = add(0x11111111,0x0);
uVar1 = add(0x22222222,0x0);
uVar1 = add(0x33333333,0x0);
uVar1 = add(0x44444444,0x0);
uVar1 = add(0x55555555,0x0);
uVar1 = add(0x66666666,0x0);
uVar1 = add(0x77777777,0x0);
uVar1 = add(0x88888888,0x0);

print();

print_asIPV4();

uVar1 = size();
printf("Size %u\n", uVar1);

uint64_t *p1 = NULL;
p1 = (uint64_t *)dlsym(handle, "newSet"); // variable from IPv4.so
printf ("newSet heap address = 0x%08lx\n", *p1);

char *error = dlerror();
if (error != NULL) {
  fprintf(stderr, "aaaa %s\n", error);
  exit(EXIT_FAILURE);
}

for (uint32_t i = 0;  i < uVar1; i++) {
  uint32_t *q = (uint32_t*)*p1+(i*2)+0;
  uint32_t *s = (uint32_t*)*p1+(i*2)+1;
  printf ("%d(%x) : %08x,%08x\n", i, i, *q, *s);
}

printf ("\n");

dlclose(handle);

exit (EXIT_SUCCESS);
}

