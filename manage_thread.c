#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "keyboard.h"
#include "screen.h"
#include "receive.h"
#include "send.h"
#include "manage_thread.h"
#include "mystructs.h"

static List *receiveList;    // theirlist
static List *sendList;       // ourlist
static char* myPort;        // myport
static char* theirPort;     // theirport
static char* theirAddress;  // theiraddress
static pthread_t keyboardThread, screenThread, receiveThread, sendThread;
static pthread_mutex_t ourMutex;
static pthread_mutex_t theirMutex;

void initialize_system(char* myP, char* theirP, char* theirAdd) {
    sendList = List_create();
    receiveList = List_create();
    myPort = myP;
    theirPort = theirP;
    theirAddress = theirAdd;

    // initialize the mutexes
    pthread_mutex_init(&ourMutex, NULL);
    pthread_mutex_init(&theirMutex, NULL);
}

void initialize_threads(){
    // create structs
    struct KeyboardScreenArgs keyboardArgs;
    keyboardArgs.list = sendList;
    keyboardArgs.mutex = ourMutex;

    struct KeyboardScreenArgs screenArgs;
    screenArgs.list = receiveList;
    screenArgs.mutex = theirMutex;

    struct RecvArgs recvArgs;
    recvArgs.list = receiveList;
    recvArgs.mutex = theirMutex;
    recvArgs.port = myPort;

    struct SendArgs sendArgs;
    sendArgs.list = sendList;
    sendArgs.mutex = ourMutex;
    sendArgs.address = theirAddress;
    sendArgs.port = theirPort;

    pthread_create(&keyboardThread, NULL, keyboard, (void*)&keyboardArgs);
    pthread_create(&screenThread, NULL, screen, (void*)&screenArgs);
    pthread_create(&receiveThread, NULL, receive, (void*)&recvArgs);
    pthread_create(&sendThread, NULL, sends, (void*)&sendArgs);
}

void cancelSystemFromSender(){
    pthread_cancel(receiveThread);
    pthread_cancel(keyboardThread);
}

void cancelSystemFromReceiver(){
    pthread_cancel(screenThread);
    pthread_cancel(sendThread);
}

void shutdown_threads(){
    pthread_join(keyboardThread, NULL);
    pthread_join(screenThread, NULL);
    pthread_join(receiveThread, NULL);
    pthread_join(sendThread, NULL);
}

void shutdown_system(){
    pthread_mutex_destroy(&ourMutex);
    pthread_mutex_destroy(&theirMutex);
    List_free(sendList, NULL);
    List_free(receiveList, NULL);

}