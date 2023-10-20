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

#define MESSAGE_LENGTH 1024

/*
waits for an input from the user, puts it in the sent list

arguments: our list, our mutex
*/

void* keyboard(void* arg) {

    struct KeyboardScreenArgs* keyboardArgs = (struct KeyboardScreenArgs*)arg;

    List* list = keyboardArgs->list;
    pthread_mutex_t mutex = keyboardArgs->mutex;

    while(1)
    {
        // gets the input from user
        char input[MESSAGE_LENGTH];
        printf("Enter message to send (or '!' to quit): ");
        fgets(input, sizeof(input), stdin);

        // locks the mutex before writing to the list
        // FIXME: should i make sure the string is not empty before i lock it?
        pthread_mutex_lock(&mutex);

        // add to the list of words to be sent
        // FIXME: im not actually sure if this is how add it to the list
        if (strcmp(input, "") == 0)
        {
            List_append(list, input);
        }

        // check to see if the user wants to exit
        if (strcmp(input, "!\n") == 0)
        {
            // unlocks the mutex and exit loop
            printf("keyboard: you have ended the chat");
            pthread_mutex_unlock(&mutex);
            break;
        }

        // unlocks the mutex
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}