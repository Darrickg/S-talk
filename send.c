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
#include "send.h"
#include "manage_lists.h"

#define MESSAGE_LENGTH 1024

/*
checks the list to see if there is anything, send it to the other party if there is

arguments: our list, our mutex, their address, their port
*/
static List *list;
static pthread_t sendThread; 
static struct addrinfo *server_info;
static int udpSocket;
static pthread_mutex_t sendAvailableCondMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t sendAvailableCond = PTHREAD_COND_INITIALIZER; // initlizing con here
 

void* sends(void* arg) {
    // initialize address
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP

    // gets address information
    if (getaddrinfo(getTheirAddress(), getTheirPort(), &hints, &server_info) != 0)
    {
        perror("getaddrinfo in send failed");
        exit(EXIT_FAILURE);
    }

    // create UDP socket for swending data
    udpSocket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (udpSocket == -1) 
    {
        perror("UDP socket creation in send failed");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        send_signal();
        //TODO: dequeue message!
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
                break;
            }

            // removes an item from the list
            List_remove(list);

        }
    }

    // close UDP sockets when done
    
    return NULL;
}

void send_init(){
    list = getSendList();
    pthread_create(&sendThread, NULL, sends, NULL);
}

void send_signal(){
    pthread_mutex_lock(&sendAvailableCondMutex);
    {
        pthread_cond_signal(&sendAvailableCond); 
    }
    pthread_mutex_unlock(&sendAvailableCondMutex);
}

void send_cancel(){
     pthread_cancel(sendThread);
}
void send_shutdown(){
    freeaddrinfo(server_info);
    close(udpSocket);

    pthread_join(sendThread, NULL);
}