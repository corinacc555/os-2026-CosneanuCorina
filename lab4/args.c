#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    printf("Total arguments received: %d\n", argc);
    
    // Loop through and print each argument
    for(int idx = 0; idx < argc; idx++) {
        printf("arg[%d] -> %s\n", idx, argv[idx]);
    }
    
    exit(0);
}
