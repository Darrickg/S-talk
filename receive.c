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
#include "receive.h"

#define MESSAGE_LENGTH 1024

/*
takes an input from the other party and puts it into the list

arguments: their list, their mutex, local (our) port
*/
void* receive(void* arg) {

    struct RecvArgs* recvArgs = (struct RecvArgs*)arg;

    List* list = recvArgs->list;
    pthread_mutex_t mutex = recvArgs->mutex;
    char* port = recvArgs->port;

    // initialize address
    struct addrinfo hints, *server_info;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP
    hints.ai_flags = AI_PASSIVE; // local IP address FIXME: might need to change this

    // gets address information
    // FIXME: some of the variables might need to be changed
    if (getaddrinfo(NULL, port, &hints, &server_info) != 0)
    {
        perror("getaddrinfo in recv failed");
        exit(EXIT_FAILURE);
    }

    // create UDP socket for receiving data
    int udpSocket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (udpSocket == -1) 
    {
        perror("UDP socket creation in recv failed");
        exit(EXIT_FAILURE);
    }

    // bind socket to local address
    if (bind(udpSocket, server_info->ai_addr, server_info->ai_addrlen) == -1) {
        perror("UDP socket bind failed");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        char input[MESSAGE_LENGTH];
        struct sockaddr_storage client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        // check to see if a message is recieved
        int recv_len = recvfrom(udpSocket, input, sizeof(input) - 1, 0, (struct sockaddr*)&client_addr, &client_addr_len);

        if (recv_len == -1)
        {
            perror("recvfrom in recv failed");
            continue;
        }

        input[recv_len] = '\0';

        // push into list
        // FIXME: im not actually sure if this is how add it to the list
        if (strcmp(input, "\n") != 0)
        {
            // locks the mutex
            pthread_mutex_lock(&mutex);

            List_append(list, input);

            // unlocks mutex
            pthread_mutex_unlock(&mutex);
        }

        if (strcmp(input, "!\n") == 0)
        {   

            pthread_mutex_unlock(&mutex);
            printf("recv: they ended the chat\n");
            break;
        }

    }

    // Close the UDP socket when done
    close(udpSocket);
    freeaddrinfo(server_info);
    return NULL;
}