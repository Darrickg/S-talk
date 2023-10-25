#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "list.h"
#include "mystructs.h"
#include "keyboard.h"
#include "screen.h"
#include "receive.h"
#include "send.h"
#include "manage_thread.h"

#define THEIRADDRESS "127.0.0.1"
#define OURPORT "8081"
#define THEIRPORT "8080"

int main(int argc, char * argv[]) {

    // address and port stuff
    // FIXME: obviously this needs to change
    char* theirAddress = THEIRADDRESS; // argv[2];
    char* ourPort = OURPORT; //argv[1];
    char* theirPort = THEIRPORT; //argv[3];

    // initialise lists
    List* ourList = List_create();
    List* theirList = List_create();

    // initialise mutexes
    pthread_mutex_t ourMutex;
    pthread_mutex_t theirMutex;

    // initialize the mutexes
    pthread_mutex_init(&ourMutex, NULL);
    pthread_mutex_init(&theirMutex, NULL);

    // create flag
    int flag = 0;

    // create structs
    struct KeyboardScreenArgs keyboardArgs;
    keyboardArgs.list = ourList;
    keyboardArgs.mutex = ourMutex;
    keyboardArgs.flag = &flag;

    struct KeyboardScreenArgs screenArgs;
    screenArgs.list = theirList;
    screenArgs.mutex = theirMutex;
    screenArgs.flag = &flag;

    // recv sockets
    // initialise address
    struct addrinfo hints_recv, *server_info_recv;
    memset(&hints_recv, 0, sizeof(hints_recv));
    hints_recv.ai_family = AF_INET;         // IPv4
    hints_recv.ai_socktype = SOCK_DGRAM;    // UDP
    hints_recv.ai_flags = AI_PASSIVE;       // local IP address

    // gets address information
    if (getaddrinfo(NULL, ourPort, &hints_recv, &server_info_recv) != 0)
    {
        perror("getaddrinfo in recv failed");
        exit(EXIT_FAILURE);
    }

    // create UDP socket for receiving data
    int udpSocket_recv = socket(server_info_recv->ai_family, server_info_recv->ai_socktype, server_info_recv->ai_protocol);
    if (udpSocket_recv == -1) 
    {
        perror("UDP socket creation in recv failed");
        exit(EXIT_FAILURE);
    }

    // bind socket to local address
    if (bind(udpSocket_recv, server_info_recv->ai_addr, server_info_recv->ai_addrlen) == -1) {
        perror("UDP socket bind failed");
        exit(EXIT_FAILURE);
    }

    struct RecvArgs recvArgs;
    recvArgs.list = theirList;
    recvArgs.mutex = theirMutex;
    recvArgs.socket = udpSocket_recv;
    recvArgs.flag = &flag;

    // send sockets
    // initialise address
    struct addrinfo hints_send, *server_info_send;
    memset(&hints_send, 0, sizeof(hints_send));
    hints_send.ai_family = AF_INET;       // IPv4
    hints_send.ai_socktype = SOCK_DGRAM;  // UDP

    // gets address information
    if (getaddrinfo(theirAddress, theirPort, &hints_send, &server_info_send) != 0)
    {
        perror("getaddrinfo in send failed");
        exit(EXIT_FAILURE);
    }

    // create UDP socket for swending data
    int udpSocket_send = socket(server_info_send->ai_family, server_info_send->ai_socktype, server_info_send->ai_protocol);
    if (udpSocket_send == -1) 
    {
        perror("UDP socket creation in send failed");
        exit(EXIT_FAILURE);
    }

    struct SendArgs sendArgs;
    sendArgs.list = ourList;
    sendArgs.mutex = ourMutex;
    sendArgs.socket = udpSocket_send;
    sendArgs.server_info = server_info_send;
    sendArgs.flag = &flag;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);


    // create and start threads
    pthread_t keyboardThread, screenThread, receiveThread, sendThread;

    // initialize_threads(pthread_create(&keyboardThread, NULL, keyboard, (void*)&keyboardArgs), pthread_create(&screenThread, NULL, screen, (void*)&screenArgs), pthread_create(&receiveThread, NULL, receive, (void*)&recvArgs), pthread_create(&sendThread, NULL, sends, (void*)&sendArgs));

    pthread_create(&keyboardThread, NULL, keyboard, (void*)&keyboardArgs);
    pthread_create(&screenThread, NULL, screen, (void*)&screenArgs);
    pthread_create(&receiveThread, NULL, receive, (void*)&recvArgs);
    pthread_create(&sendThread, NULL, sends, (void*)&sendArgs);

    initialize_threads(keyboardThread, screenThread, receiveThread, sendThread);

    // wait for threads to finish
    pthread_join(keyboardThread, NULL);
    pthread_join(screenThread, NULL);
    pthread_join(receiveThread, NULL);
    pthread_join(sendThread, NULL);

    // cleanup and exit
    close(udpSocket_recv);
    freeaddrinfo(server_info_recv);

    close(udpSocket_send);
    freeaddrinfo(server_info_send);

    pthread_mutex_destroy(&ourMutex);
    pthread_mutex_destroy(&theirMutex);
    List_free(ourList, NULL);
    List_free(theirList, NULL);
    return 0;
}