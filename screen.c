#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>

#include "list.h"
#include "screen.h"
#include "send.h"
#include "manage_lists.h"

#define MESSAGE_LENGTH 1024

static List *list;
static pthread_t screenThread;

/*
takes an input from the list and prints it

arguments: their list, their mutex
*/

void* screen(void* arg) {

    while(1)
    {

        // if the list isnt empty
        if (List_count(list) > 0)
        {  
            // prints the message at the top of the queue
            // FIXME: im not sure if this is how u print a string from an input
            char* input = List_first(list);

            //TODO: enqueue message!

            if (strcmp(input, "!\n") == 0){
                send_signal();
                cancelSystemFromSender();
                printf("screen: they have ended the chat\n");
                break;
            }
            
            // FIXME: should get their name instead of mine
            printf("They said: %s", input);

            // removes an item from the list
            List_remove(list);
        }
        send_signal();

    }

    return NULL;
}

void screen_init(void){
    list = getReceiveList();
    pthread_create(&screenThread, NULL, screen, NULL);
}

void screen_cancel(){
    pthread_cancel(screenThread);
}

void screen_shutdown(){

    pthread_join(screenThread, NULL);
}