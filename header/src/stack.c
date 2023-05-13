#include "../stack.h"

stack push(stack self, byte data[8]){
    if (self.top < 8){
        for (int i = 0;i<8;i++){
            self.mem[self.top][i]=data[i];
        }
        self.top++;
    }
    return self;
}

stack pop(stack self, byte data[8]){
    if (self.top > 0){
        self.top--;
        for (int i=0; i<8;i++){
            data[i]=self.mem[self.top][i];
        }
    }
    return self;
}