#ifndef string_h
#define string_h

#include "./default.h"

byte strcomp(char* first, char* second);
u64 strlength(char* str);
u64 strToInt(byte* str);
void strcopy(char* org, char* res, u64 bufsize);

u64 nextchars(u64 startpos, char keychar,char* orgstr, char* buff, u64 buffsize);


#endif