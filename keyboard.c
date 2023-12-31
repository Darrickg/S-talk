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
#include "keyboard.h"
#include "manage_thread.h"

#define MESSAGE_LENGTH 1024

/*
waits for an input from the user, puts it in the sent list

arguments: our list, our mutex
*/

void* keyboard(void* arg) {

    struct KeyboardScreenArgs* keyboardArgs = (struct KeyboardScreenArgs*)arg;

    List* list = keyboardArgs->list;
    pthread_mutex_t mutex = keyboardArgs->mutex;

    fputs("Enter message to send (or '!' to quit)\n", stdout);

    while(1)
    {
        pthread_testcancel();
      
        // gets the input from user
        char input[MESSAGE_LENGTH];
        fgets(input, sizeof(input), stdin);

        // add to the list of words to be sent
        if (strcmp(input, "\n") != 0)
        {
            // locks the mutex before writing to the list
            pthread_mutex_lock(&mutex);

            List_append(list, input);

            // unlocks the mutex
            pthread_mutex_unlock(&mutex);
        }

        // check to see if the user wants to exit
        if (strcmp(input, "!\n") == 0)
        {
            // exit loop
            fputs("\nYou have ended the chat\n", stdout);

            cancelReceive();
            cancelScreen();
            
            break;
        }

    }

    return NULL;
}