#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("Total arguments received: %d\n", argc);
    
    for(int idx = 0; idx < argc; idx++) {
        printf("arg[%d] -> %s\n", idx, argv[idx]);
    }
    
    return 0; // Linux expects return 0 instead of exit(0)
}
