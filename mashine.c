#include "./header/default.h"
#include "./header/stack.h"
#include "./header/math.h"
#include "./header/vmbase.h"
#include "./header/string.h"

int main(int argc, char** argv){

    SOC main = createVM(4*GB);
    FILE* file = fopen(argv[1], "r");
    byte curr;
    for (u64 i = 0;!feof(file);i++){
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

    printVM(main);
    cleanVM(main);
}