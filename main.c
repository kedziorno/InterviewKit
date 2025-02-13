#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>

#include "test_macros_int8.h"
#include "test_macros_int16.h"
#include "test_macros_int32.h"

#include "test_macros_uint8.h"
#include "test_macros_uint16.h"
#include "test_macros_uint32.h"

#include "test_macros_str.h"

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

  // MAIN TESTS

  clear ();

  uint32_t uVar2;

  uVar2 = init ();
  ASSERT_EQUAL_INT32 (*p1, uVar2); // address for new test is the same as address get from dlsym (newSet)

  print ();
  print_asIPV4 ();

  uVar2 = size();
  ASSERT_EQUAL_INT32 (0, uVar2); // we have empty database

  print ();
  print_asIPV4 ();

  uVar2 = add(0xc0a80100,0x18); // .0/24
  ASSERT_EQUAL_INT32 (0, uVar2); // Add good address
  ASSERT_EQUAL_INT32 (0x18, check (0xc0a80100));

  uVar2 = add(0xc0a801fe,0x18); // .254/24
  ASSERT_EQUAL_INT32 (-1, uVar2); // Add bad address
  ASSERT_NOT_EQUAL_INT32 (0x00, check (0xc0a801fe));

  uVar2 = add(0xc0a801fe,0x20); // .254/32
  ASSERT_EQUAL_INT32 (0, uVar2); // Add good address
  ASSERT_EQUAL_INT32 (0x20, check (0xc0a801fe));

  uVar2 = add(0xc0a801fe,0x00); // .254/0
  ASSERT_EQUAL_INT32 (0, uVar2); // Add good address
  ASSERT_NOT_EQUAL_INT32 (0x00, check (0xc0a801fe));

  uVar2 = add(0xc0a80100,0x20); // .0/32
  ASSERT_EQUAL_INT32 (0, uVar2); // Add good address
  ASSERT_EQUAL_INT32 (0x20, check (0xc0a80100));

  uVar2 = size();
  ASSERT_EQUAL_INT32 (4, uVar2); // count from 0 - after add's

  print ();
  print_asIPV4 ();

  uVar2 = del(0xc0a80100,0x18); // .0/24
  ASSERT_EQUAL_INT32 (0, uVar2); // Delete address
  ASSERT_EQUAL_INT32 (0x20, check (0xc0a80100)); // line 104

  uVar2 = del(0xc0a80100,0x20); // .0/32
  ASSERT_EQUAL_INT32 (0, uVar2); // Delete address
  ASSERT_EQUAL_INT32 (-1, check (0xc0a80100)); // Not exists in database

  uVar2 = size();
  ASSERT_EQUAL_INT32 (2, uVar2); // count from 0 - after one delete

  print ();
  print_asIPV4 ();

  uVar2 = del(0xc0a801fe,0x00); // .254/0
  ASSERT_EQUAL_INT32 (0, uVar2); // Add good address
  ASSERT_NOT_EQUAL_INT32 (0x00, check (0xc0a801fe));

  uVar2 = del(0xc0a801fe,0x00); // .254/0
  ASSERT_EQUAL_INT32 (0, uVar2); // Add good address
  ASSERT_NOT_EQUAL_INT32 (0x00, check (0xc0a801fe));

  uVar2 = size();
  ASSERT_EQUAL_INT32 (1, uVar2); // count from 0 - after one delete

  print ();
  print_asIPV4 ();

  uVar2 = del(0xc0a801fe,0x1f); // .254/32 -> .254/31
  ASSERT_EQUAL_INT32 (0, uVar2); // unset
  ASSERT_NOT_EQUAL_INT32 (0x1f, check (0xc0a801fe)); // fail : must have / 31

  uVar2 = size();
  ASSERT_EQUAL_INT32 (1, uVar2); // count from 0 - after one delete
  ASSERT_NOT_EQUAL_INT32 (0x00, check (0xc0a801fe));

  clear ();

  uVar2 = size();
  ASSERT_NOT_EQUAL_INT32 (0, uVar2); // not empty and print's IP's make SEGFAULT

  //print ();
  //print_asIPV4 ();

  exit (EXIT_SUCCESS);

}

