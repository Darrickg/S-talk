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
#include "mystructs.h"
#include "screen.h"

#define MESSAGE_LENGTH 1024

/*
takes an input from the list and prints it

arguments: their list, their mutex
*/

void* screen(void* arg) {

    struct KeyboardScreenArgs* screenArgs = (struct KeyboardScreenArgs*)arg;

    // create arguments from structs
    List* list = screenArgs->list;
    pthread_mutex_t mutex = screenArgs->mutex;

    while(1)
    {
        // checks for thread cancels
        pthread_testcancel();
    
        // locks the mutex
        pthread_mutex_lock(&mutex);

        // if the list isnt empty
        if (List_count(list) > 0)
        {  
            // prints the message at the top of the queue
            // FIXME: im not sure if this is how u print a string from an input
            char* input = List_first(list);

            // if they send a !
            if (strcmp(input, "!\n") == 0)
            {
                pthread_mutex_unlock(&mutex);
                break;
            }
            
            fputs("\nTheir Message: ", stdout);
            fputs(input, stdout);
            fputs("\n", stdout);

            // removes an item from the list
            List_remove(list);
        }

        // unlocks the mutex
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}