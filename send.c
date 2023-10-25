#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "list.h"
#include "mystructs.h"
#include "send.h"

#define MESSAGE_LENGTH 1024

/*
checks the list to see if there is anything, send it to the other party if there is

arguments: our list, our mutex, send socket (their address and their port)
*/

void* sends(void* arg) {

    struct SendArgs* sendArgs = (struct SendArgs*)arg;

    // initialise arguments from struct
    List* list = sendArgs->list;
    pthread_mutex_t mutex = sendArgs->mutex;
    int udpSocket = sendArgs->socket;
    struct addrinfo *server_info = sendArgs->server_info;

    while(1)
    {
        // checks for cancel
        pthread_testcancel();

        // locks the mutex
        pthread_mutex_lock(&mutex);

        // if the list isnt empty
        if (List_count(list) > 0)
        {  
            // gets the message at the top of the queue
            char* input = List_first(list);

            // sends message
            int send_len = sendto(udpSocket, input, strlen(input), 0, server_info->ai_addr, server_info->ai_addrlen);

            // if failed to send
            if (send_len == -1)
            {
                perror("sendto failed");
            }

            // if ! is sent
            if (strcmp(input, "!\n") == 0)
            {
                pthread_mutex_unlock(&mutex);
                break;
            }

            // removes an item from the list
            List_remove(list);

        }

        pthread_mutex_unlock(&mutex);

    }

    return NULL;
}