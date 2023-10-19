// /*
// takes whatever is in the recieved list
// outputs it to the screen/terminal
// */
// typedef struct sem {
//     int value;
//     list_of_processes plist; //(processes blocked on this semaphore)
// } semaphore;

// SEM full = 0; (# of items available for consumption)
// SEM empty = N; (# of empty buffer entries)
// SEM mutex = 1;
// SEM wrt = 1;
// int readcount = 0;

// void wait_to_display(){
//     while (1) {
//         P (mutex) ;
//         readcount++ ;
//         if (readcount == 1)
//             P (wrt);
//         V (mutex)

//         // reading is performed
        
//         P (mutex) ;
//         readcount-- ;
//         if (readcount == 0)
//             V (wrt) ;
//         V (mutex) ;
//     }

// }

// Consumer: while(1) {
// P(full); (wait until at least 1 buffer is filled)
// P(mutex); (wait for concurrency OK from producer)
// -remove item from the buffer
// V(mutex); (signal concurrency OK to producer)
// V(empty); (record the addition of 1 empty buffer)
// -consume item
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
takes an input from the list and prints it
*/

void screen(List* list, pthread_mutex_t* mutex) {

    while(1)
    {
        // locks the mutex
        pthread_mutex_lock(&mutex);

        // if the list isnt empty
        if (List_count(list) > 0)
        {  
            // prints the message at the top of the queue
            // FIXME: im not sure if this is how u print a string from an input
            char input[MESSAGE_LENGTH] = List_first(list);

            if (strcmp(input, "!\n") == 0)
        {
            printf("Darrick has ended the chat\n");
            pthread_mutex_unlock(&mutex);
            break;
        }
            
            // FIXME: should get their name instead of mine
            printf("Darrick says: %s", input);

            // removes an item from the list
            List_remove(list);
        }

        // unlocks the mutex
        pthread_mutex_unlock(&mutex);
    }
}