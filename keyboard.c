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
#include "keyboard.h"

#define MESSAGE_LENGTH 1024

static pthread_t keyboardThread;
static pthread_mutex_t writeAvailableCondMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t writeAvailableCond = PTHREAD_COND_INITIALIZER;

/*
waits for an input from the user, puts it in the sent list

arguments: our list, our mutex
*/

void* keyboard(void* arg) {

    while(1)
    {
        keyboard_signal();

        //TODO: dequeue from list

        // gets the input from user
        char input[MESSAGE_LENGTH];
        printf("Enter message to send (or '!' to quit): ");
        fgets(input, sizeof(input), stdin);

        // add to the list of words to be sent
        // FIXME: im not actually sure if this is how add it to the list
        if (strcmp(input, "\n") != 0)
        {
            // locks the mutex before writing to the list
            // FIXME: should i make sure the string is not empty before i lock it?

            //TODO: enqueue on to list!

            // unlocks the mutex
        }

        // check to see if the user wants to exit
        if (strcmp(input, "!\n") == 0)
        {
            // unlocks the mutex and exit loop
            printf("keyboard: you have ended the chat\n");
            lockMutex()
            break;
        }

    }

    return NULL;
}

void keyboard_init(void){
    pthread_create(&keyboardThread, NULL, keyboard, NULL);
}

void keyboard_signal(){
    pthread_mutex_lock(&writeAvailableCondMutex);
    {
        pthread_cond_wait(&writeAvailableCond, &writeAvailableCondMutex);
    }
    pthread_mutex_unlock(&writeAvailableCondMutex);
}

void keyboard_cancel(){
    pthread_cancel(keyboardThread);
}

void keyboard_shutdown(){
    pthread_join(keyboardThread, NULL);
}