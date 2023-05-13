#ifndef string_h
#define string_h

#include "./default.h"

byte strcomp(char* first, char* second);
u64 strlength(char* str);
u64 strToInt(byte* str);
char** split(byte* str, byte* keyword);


#endif