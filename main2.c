#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "manage_thread.h"

#define THEIRADDRESS "127.0.0.1"
#define OURPORT "8081"
#define THEIRPORT "8080"

int main(int argc, char * argv[]) {
    // address and port stuff
    char* theirAddress = THEIRADDRESS; // argv[2];
    char* ourPort = OURPORT; //argv[1];
    char* theirPort = THEIRPORT; //argv[3];

    //initialize system
    initialize_system(ourPort, theirPort, theirAddress);
    initialize_threads();

    // wait for threads to finish
    shutdown_threads();

    // cleanup and exit
    shutdown_system();
    return 0;
}