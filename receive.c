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
#include "receive.h"
#include "screen.h"
#include "manage_lists.h"
#include "keyboard.h"

#define MESSAGE_LENGTH 1024

static pthread_t receiveThread;
static int udpSocket;
static struct addrinfo *server_info;
static List *list;

/*
takes an input from the other party and puts it into the list

arguments: their list, their mutex, local (our) port
*/
void* receive(void* arg) {

    // initialize address
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP
    hints.ai_flags = AI_PASSIVE; // local IP address FIXME: might need to change this

    // gets address information
    // FIXME: some of the variables might need to be changed
    if (getaddrinfo(NULL, getMyPort(), &hints, &server_info) != 0)
    {
        perror("getaddrinfo in recv failed");
        exit(EXIT_FAILURE);
    }

    // create UDP socket for receiving data
    udpSocket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (udpSocket == -1) 
    {
        perror("UDP socket creation in recv failed");
        exit(EXIT_FAILURE);
    }

    // bind socket to local address
    if (bind(udpSocket, server_info->ai_addr, server_info->ai_addrlen) == -1) {
        close(udpSocket);
        perror("UDP socket bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Send: You have ended the chat\n");

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
        if (strcmp(input, "\n") != 0)
        {
            //TODO: enqueue with mutex!
             printf("YOU HAVE RECEIVED A MESSAGE\n");
            printf("%s \n", input);
        }

        if (strcmp(input, "!\n") == 0)
        {   
            keyboard_signal();
            cancelSystemFromReceiver();
            printf("recv: they ended the chat\n");
            break;
        }

        keyboard_signal();
    }

    freeaddrinfo(server_info);
    return NULL;
}

void receive_init(){
    list = getReceiveList();
    pthread_create(&receiveThread, NULL, receive, NULL);
}

void receiver_cancel(){
    pthread_cancel(receiveThread);
}

void receiver_shutdown(){
    close(udpSocket);
    freeaddrinfo(server_info);
    pthread_join(receiveThread, NULL);
}