#ifndef math_h
#define math_h

#include "./default.h"

void byteAdd(byte first[8], byte second[8], byte res[8]);
void byteSub(byte first[8], byte second[8], byte res[8]);

u64 toInt(byte num[8]);
void toBytes(u64 num, byte res[8]);

void printarr(byte num[8]);
u64 power(u64 org, u64 times);

#endif