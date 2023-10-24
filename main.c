#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "list.h"
#include "keyboard.h"
#include "screen.h"
#include "receive.h"
#include "send.h"
#include "manage_lists.h"

#define THEIRADDRESS "127.0.0.1"
#define OURPORT "8081"
#define THEIRPORT "8080"

int main(int argc, char * argv[]) {

    // address and port stuff
    char* ourPort = OURPORT; //argv[1];
    char* theirAddress = THEIRADDRESS; // argv[2];
    char* theirPort = THEIRPORT; //argv[3];

    initalize_system(ourPort, theirPort, theirAddress);

    // create and start threads
    receive_init();
    screen_init();
    send_init();
    keyboard_init();

    // close threads
    receiver_shutdown();
    screen_shutdown();
    send_shutdown();
    keyboard_shutdown();

    end_lists();

    return 0;
}