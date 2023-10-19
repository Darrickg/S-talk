// typedef struct sem {
//     int value;
//     list_of_processes plist; //(processes blocked on this semaphore)
// } semaphore;

// SEM full = 0; (# of items available for consumption)
// SEM empty = N; (# of empty buffer entries)
// SEM mutex = 1;

// void Wait_For_Input(){
//     while (1){
//         pthread_mutex_lock(mutex){
//             pthread_cond_wait(condition, mutex)
//         }
//         pthread_mutex_unlock(mutex)
//     }
// }
// Producer: while (1) {
// -produce new item

// pthread_mutex_lock(mutex){
//     pthread_cond_wait(condition, mutex)
// }
// pthread_mutex_unlock(mutex)

// P(empty); (make sure we have at least 1 empty buffer)
// P(mutex); (to guard against concurrent buffer manipulation)
// -add the new item to the buffer
// V(mutex); (signal OK for buffer access by consumer)
// V(full); (records addition of item to buffer)
// }

// while(1) {
// flag[j] = true;
// while(flag[i]) {
// if (turn == i) {
// flag[j] = false;
// while (turn == i);
// flag[j] = true;
// }
// }
// Critical Section
// turn = i;
// flag[j] = false;
// Non-critical Section
// }
// Pi: 

// P(S) {
// while (S <= 0);
// S--;
// }
// V(S) {
// S++;
// }

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

#define MESSAGE_LENGTH 1024

/*
waits for an input from the user
puts it in the sent list
*/

void keyboard(List* list, pthread_mutex_t mutex) {

    while (1)
    {
        // gets the input from user
        char input[MESSAGE_LENGTH];
        printf("Enter message to send (or '!' to quit): ");
        fgets(input, sizeof(input), stdin);

        // locks the mutex before writing to the list
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
            printf("You have ended the chat");
            pthread_mutex_unlock(&mutex);
            break;
        }

        // unlocks the mutex
        pthread_mutex_unlock(&mutex);
    }

    return;
}