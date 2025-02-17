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

uint64_t* find_newSet();

void case1();
void case2();
void case3();

typedef struct {
  uint32_t address;
  uint8_t mask;
} bunch_ipv4_addresses_t;

int main (int argc, char *argv[], char *env[]) {
  (void)argc;
  (void)argv;
  (void)env;
  runTests((void *)0); // from IPv4.so, function cleaning structure at end

  // MAIN TESTS
  case1 ();
  case2 ();
  case3 ();

  clear ();
  init ();

  uint32_t uVar2;

  print ();
  print_asIPV4 ();

  uVar2 = size();
  ASSERT_EQUAL_INT32 ("Empty database", 0, uVar2); // we have empty database

  print ();
  print_asIPV4 ();

  uVar2 = add(0xc0a80100,0x18); // .0/24
  ASSERT_EQUAL_INT32 ("Add good address", 0, uVar2); // Add good address
  ASSERT_EQUAL_INT32 ("Check mask 0x18", 0x18, check (0xc0a80100));

  uVar2 = add(0xc0a801fe,0x18); // .254/24
  ASSERT_EQUAL_INT32 ("Add bad address", -1, uVar2); // Add bad address
  ASSERT_NOT_EQUAL_INT32 ("Check mask 0x00", 0x00, check (0xc0a801fe));

  uVar2 = add(0xc0a801fe,0x20); // .254/32
  ASSERT_EQUAL_INT32 ("Add good address", 0, uVar2); // Add good address
  ASSERT_EQUAL_INT32 ("Check mask 0x20", 0x20, check (0xc0a801fe));

  uVar2 = add(0xc0a801fe,0x00); // .254/0
  ASSERT_EQUAL_INT32 ("Add good address", 0, uVar2); // Add good address
  ASSERT_NOT_EQUAL_INT32 ("Check mask 0x00", 0x00, check (0xc0a801fe));

  uVar2 = add(0xc0a80100,0x20); // .0/32
  ASSERT_EQUAL_INT32 ("Add good address", 0, uVar2); // Add good address
  ASSERT_EQUAL_INT32 ("Check mask 0x20", 0x20, check (0xc0a80100));

  uVar2 = size();
  ASSERT_EQUAL_INT32 ("We have 5 items", 4, uVar2); // count from 0 - after add's

  print ();
  print_asIPV4 ();

  uVar2 = del(0xc0a80100,0x18); // .0/24
  ASSERT_EQUAL_INT32 ("Delete address", 0, uVar2); // Delete address
  ASSERT_EQUAL_INT32 ("Check mask 0x20 on deleted address", 0x20, check (0xc0a80100)); // line 104

  uVar2 = del(0xc0a80100,0x20); // .0/32
  ASSERT_EQUAL_INT32 ("Delete address", 0, uVar2); // Delete address
  ASSERT_EQUAL_INT32 ("Check if address exists in database", -1, check (0xc0a80100)); // Not exists in database

  uVar2 = size();
  ASSERT_EQUAL_INT32 ("After two deletes we have 3 items", 2, uVar2); // count from 0 - after one delete

  print ();
  print_asIPV4 ();

  uVar2 = del(0xc0a801fe,0x00); // .254/0
  ASSERT_EQUAL_INT32 ("Delete address", 0, uVar2); // Delete address
  ASSERT_NOT_EQUAL_INT32 ("Check after delete address", 0x00, check (0xc0a801fe));

  uVar2 = del(0xc0a801fe,0x00); // .254/0
  ASSERT_EQUAL_INT32 ("Delete address", 0, uVar2); // Delete address
  ASSERT_NOT_EQUAL_INT32 ("Check after delete address", 0x00, check (0xc0a801fe));

  uVar2 = size();
  ASSERT_EQUAL_INT32 ("After two deletes we have 2 items", 1, uVar2); // count from 0 - after one delete

  print ();
  print_asIPV4 ();

  uVar2 = del(0xc0a801fe,0x1f); // .254/32 -> .254/31
  ASSERT_EQUAL_INT32 ("Delete address", 0, uVar2); // unset
  ASSERT_NOT_EQUAL_INT32 ("This test fail when replace .254/32 -> .254/31", 0x1f, check (0xc0a801fe)); // fail : must have / 31

  uVar2 = size();
  ASSERT_EQUAL_INT32 ("Check size after one delete", 1, uVar2); // count from 0 - after one delete
  ASSERT_NOT_EQUAL_INT32 ("Some mask for address 0xc0a801fe exists", 0x00, check (0xc0a801fe));

  clear ();

  uVar2 = size();
  ASSERT_NOT_EQUAL_INT32 ("We don't have empty database - one element", 0, uVar2); // not empty and print's IP's make SEGFAULT

  //print (); // SIGV
  //print_asIPV4 (); // SIGV

  exit (EXIT_SUCCESS);
}

void case1() { // TODO test for int64/uint64
  uint64_t* newSet_address = find_newSet();
  uint64_t init_address = init();
  ASSERT_EQUAL_INT32 ("Pointer to newSet is equal - variables", *newSet_address, init_address); // address for new test is the same as address get from dlsym (newSet)
  //ASSERT_EQUAL_UINT32 ("Pointer to newSet is equal - functions", (uint32_t)init(), (uint32_t)find_newSet()); // address for new test is the same as address get from dlsym (newSet) // TODO Make int64/uint64 test
}

void case2() {
  uint64_t* p1 = find_newSet();
  uint32_t uVar1;
  uint32_t *q, *s;
  #define IPV4_ADDRESSES 9
  bunch_ipv4_addresses_t bunch_ipv4_addresses[IPV4_ADDRESSES] = {
    { 0xaaaaaaaa, 0x00 },
    { 0x11111111, 0x00 },
    { 0x22222222, 0x00 },
    { 0x33333333, 0x00 },
    { 0x44444444, 0x00 },
    { 0x55555555, 0x00 },
    { 0x66666666, 0x00 },
    { 0x77777777, 0x00 },
    { 0x88888888, 0x00 },
  };

  clear();
  init();

  print();
  print_asIPV4();

  for (uint8_t index = 0; index < IPV4_ADDRESSES; index++) {
    //const char *info = sprintf ("Add address %08x with mask 0x02x\n", bunch_ipv4_addresses[index].address, bunch_ipv4_addresses[index].mask); 
    ASSERT_EQUAL_INT32 ("Add address from bunch_ipv4_addresses", add (bunch_ipv4_addresses[index].address, bunch_ipv4_addresses[index].mask), 0);
    ASSERT_NOT_EQUAL_INT32 ("Add address from bunch_ipv4_addresses - negate logic", add (bunch_ipv4_addresses[index].address, bunch_ipv4_addresses[index].mask), ~0);
  }

  print();
  print_asIPV4();

  ASSERT_EQUAL_INT32 ("After add's, we have bunch addresses equal to" #IPV4_ADDRESSES, IPV4_ADDRESSES, size ());

  for (uint8_t i = 0;  i < IPV4_ADDRESSES; i++) { // TODO Fix index i
    const char buffer1 [512] = { 0 };
    const char buffer2 [512] = { 0 };
    q = (uint32_t*)*p1+(i*2)+0;
    s = (uint32_t*)*p1+(i*2)+1;
    printf ("%d(%x) : %08x,%08x\n", i, i, *q, *s);
    const char *test_src = sprintf (&buffer1, "%08x,%08x\n\0", *q, *s);
    const char *test_dst = sprintf (&buffer2, "%08x,%08x\n\0", bunch_ipv4_addresses[i].address, bunch_ipv4_addresses[i].mask);
    ASSERT_EQUAL_STR ("Test bunch addresses - for equal address in bunch and stored in newSet, index =" #i, &buffer1, &buffer2);
    ASSERT_NOT_EQUAL_STR ("Test bunch addresses - for NOT equal address in bunch and stored in newSet, index =" #i, &buffer1+1, &buffer2+2);
    ASSERT_EQUAL_UINT32 ("Test bunch addresses - compare address(32) at i", *q, bunch_ipv4_addresses[i].address);
    ASSERT_EQUAL_UINT8 ("Test bunch addresses - compare mask(8) at i", *s, bunch_ipv4_addresses[i].mask);
  }
}

void case3() { // Behaviour init() and clear()
  bunch_ipv4_addresses_t ipv4[4] = {
    { 0x7f000001, 0x00 },
    { 0x7f000001, 0x20 },
    { 0xc0a80001, 0x00 },
    { 0xc0a80001, 0x20 },
  };

  for (uint8_t index = 0; index < sizeof (ipv4); index++) {
    // empty
    init ();
    ASSERT_EQUAL_INT32 ("Call init(), we have empty database", 0, size());
    clear ();
    ASSERT_EQUAL_INT32 ("Call clear(), we have empty database also", 0, size());
    init ();
    ASSERT_EQUAL_INT32 ("Call init again(), we have empty database also", 0, size());
    // one address, init again
    ASSERT_EQUAL_INT32 ("Add test address " #ipv4[index].address "/" #ipv4[index].mask, 0, add (ipv4[index].address, ipv4[index].mask));
    print ();
    print_asIPV4 ();
    ASSERT_EQUAL_INT32 ("Add test address " #ipv4[index].address "/" #ipv4[index].mask ", we have 1 item", 1, size());
    init ();
    ASSERT_EQUAL_INT32 ("Call init() on filler database, we expect 0 items - reset", 0, size());
    // one address, next() clear
    ASSERT_EQUAL_INT32 ("Add test address " #ipv4[index].address "/" #ipv4[index].mask, 0, add (ipv4[index].address, ipv4[index].mask));
    print ();
    print_asIPV4 ();
    ASSERT_EQUAL_INT32 ("Add test address " #ipv4[index].address "/" #ipv4[index].mask ", we have 1 item", 1, size());
    clear ();
    ASSERT_EQUAL_INT32 ("Call clear() on filler database, we expect 0 items - reset", 0, size());
  }
}

uint64_t* find_newSet() {
  void *handle = NULL;
  uint64_t *p1 = NULL;
  char *error = NULL;
  handle = dlopen ("../IPv4.so", RTLD_NOW | RTLD_GLOBAL);
  if (!handle) {
    fprintf(stderr, "dlopen : %s\n", dlerror());
    return p1;
  }
  dlerror();
  p1 = (uint64_t *)dlsym(handle, "newSet"); // variable from IPv4.so
  error = dlerror();
  if (error != NULL) {
    fprintf(stderr, "dlsym %s\n", error);
    return p1;
  }
  dlclose(handle);
  printf ("newSet address = 0x%08lx\n", p1);
  return p1;
}

