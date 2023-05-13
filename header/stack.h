#ifndef stack_h
#define stack_h

#include "./default.h"

typedef struct{
    byte mem[8][8];
    byte top;
}stack;

stack push(stack self, byte data[8]);
stack pop(stack self, byte callback[8]);

#endif