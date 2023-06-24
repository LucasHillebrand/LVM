#include "./header/default.h"
#include "./header/stack.h"
#include "./header/math.h"
#include "./header/vmbase.h"
#include "./header/string.h"
#include "header/file.h"
#include <stdio.h>

int main(int argc, char** argv){
    u64 memsize = 4*GB;
    char filename[1024] = {0};
    byte run = 1;
    byte debug = 0;

    for (int i=0;i<argc;i++){
        if(strcomp(argv[i], "-f")){
            i++;
            strcopy(argv[i], (char*)filename, 1024);
        }else if (strcomp(argv[i], "-m")){
            i++;
            memsize=strToInt((byte*)argv[i]);
            switch (argv[i][strlength(argv[i])-1]) {
            case 'G':
                memsize*=GB;
                break;
            case 'M':
                memsize*=MB;
                break;
            case 'K':
                memsize*=KB;
                break;
            }
        }else if (strcomp(argv[i], "--help")){
            printf("-f [filename]\n-m [{memsize}{K/M/G}]\n-d == debug function on\n-ed == extended debugging > with memdump !!!! not recomended\n//later:\n  -f <=> -b \n  -h == harddrive(something.vhd) => -h [vhd file)]\n  -cvh == create hardrive => -cvh [output file] [size{G/M/K}]\n");
            run=0;
        }else if (strcomp(argv[i], "-d")) {
            debug=1;
        }else if (strcomp(argv[i], "-ed")) {
            debug=2;
        }
    }
    
    if(run){
    SOC main = createVM(memsize);
    FILE* file = fopen(filename, "r");
    byte curr;
    u64 fs=getFileSize(file);
    rewind(file);
    for (u64 i = 0;i<fs;i++){
        main.mem[i] = fgetc(file);
    }
    fclose(file);

    main.cpu.threads[0].init=1;

    while (main.cpu.active == 1) {
        for (u64 i = 0; i<8;i++){
            if (main.cpu.threads[i].init == 1){
                main = exec(main, i);
            }
        }
    }

    if (debug){printVM(main,debug-1);}
    cleanVM(main);
    }
}