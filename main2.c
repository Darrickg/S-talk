#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

    struct RecvArgs recvArgs;
    recvArgs.list = theirList;
    recvArgs.mutex = theirMutex;
    recvArgs.port = ourPort;
    recvArgs.flag = &flag;

    struct SendArgs sendArgs;
    sendArgs.list = ourList;
    sendArgs.mutex = ourMutex;
    sendArgs.address = theirAddress;
    sendArgs.port = theirPort;
    sendArgs.flag = &flag;

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
    printf("\nit goes here\n");
    pthread_join(screenThread, NULL);
    printf("\nit goes here\n");
    pthread_join(receiveThread, NULL);
    printf("\nit goes here\n");
    pthread_join(sendThread, NULL);

    printf("\nit goes here\n");

    // cleanup and exit
    pthread_mutex_destroy(&ourMutex);
    pthread_mutex_destroy(&theirMutex);
    List_free(ourList, NULL);
    List_free(theirList, NULL);

    return 0;
}