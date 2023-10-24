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

arguments: our list, our mutex, their address, their port
*/

void* sends(void* arg) {

    struct SendArgs* sendArgs = (struct SendArgs*)arg;

    List* list = sendArgs->list;
    pthread_mutex_t mutex = sendArgs->our_mutex;
    char* address = sendArgs->address;
    char* port = sendArgs->port;

    // initialize address
    struct addrinfo hints, *server_info;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP

    // gets address information
    if (getaddrinfo(address, port, &hints, &server_info) != 0)
    {
        perror("getaddrinfo in send failed");
        exit(EXIT_FAILURE);
    }

    // create UDP socket for swending data
    int udpSocket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (udpSocket == -1) 
    {
        perror("UDP socket creation in send failed");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        // locks the mutex
        pthread_mutex_lock(&mutex);

        // if the list isnt empty
        if (List_count(list) > 0)
        {  
            // gets the message at the top of the queue
            // FIXME: im not sure if this is how u get a string from list
            char* input = List_first(list);

            int send_len = sendto(udpSocket, input, strlen(input), 0, server_info->ai_addr, server_info->ai_addrlen);

            if (send_len == -1)
            {
                perror("sendto failed");
            }

            if (strcmp(input, "!\n") == 0)
            {
                printf("Send: You have ended the chat\n");
                pthread_mutex_unlock(&mutex);
                pthread_mutex_unlock(&sendArgs->their_mutex);
                break;
            }

            // removes an item from the list
            List_remove(list);

        }

        pthread_mutex_unlock(&mutex);

    }

    // close UDP sockets when done
    close(udpSocket);
    freeaddrinfo(server_info);

    return NULL;
}