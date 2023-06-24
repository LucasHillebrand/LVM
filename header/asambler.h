#ifndef asm_h
#define asm_h

#include "./default.h"
#include "./instructionset.h"
#include "./string.h"
#include "./math.h"
#include "./file.h"

#define pub 1
#define priv 0


struct val {
    char* names[1024];
    byte* value[8];
    u64 size;
    u64 curr;
};

typedef struct{
    struct {
        char* names;
        byte* value;
        uint size;
        u64 curr;
    } values[2];

    u64 vpc;
} sysvar;

typedef struct{
    char instruction[1024];
    byte args[3][8];
} command;
struct result{byte found;u64 where;};

sysvar* vinit(uint basesize);
void vclr(sysvar* self);
void vadd(sysvar* self, byte type /*0=priv, 1=pub, 2=app*/, char name[64], byte value[8]);
void vsearch(sysvar* self, byte type /*0=priv, 1=pub, 2=app*/, char keyword[64], byte buff[8]);
void vrewind(sysvar* self);
struct result search(sysvar* self, byte type, char keyword[64]);

command lex(LINE* ln, sysvar* vars);
byte parse(FILE* dst, command cmd);

#endif