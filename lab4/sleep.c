#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    // Check if the user provided the exact number of arguments
    if(argc != 2) {
        fprintf(2, "Error: valid usage is sleep <ticks>\n");
        exit(1);
    }
    
    // Parse the input and pause execution
    int wait_ticks = atoi(argv[1]);
    pause(wait_ticks); // Replaced sleep() with pause() for the new xv6 version
    
    exit(0);
}
