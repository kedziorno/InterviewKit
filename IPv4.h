#ifndef IPV4_H
#define IPV4_H

#include <stdint.h>

uint64_t init(void); // Tools show than init() return some value (it's address of DB)

int add(unsigned int base, char mask);

int del(unsigned int base, char mask);

char check(unsigned int ip);

void clear(void);

// ghirda
void __do_global_dtors_aux(void);
void _fini(void);
void _init(void);
void frame_dummy(void);
uint32_t checkIfPrefixExists(uint32_t param_1,char param_2);
uint32_t checkIfPrefixIsValid(uint32_t param_1,char param_2);
void print(void);
void print_asIPV4(void);
void runTests(void*);
uint32_t size();

#endif

