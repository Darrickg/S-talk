#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "manage_lists.h"
#include "list.h"
#include "receive.h"
#include "send.h"
#include "keyboard.h"
#include "screen.h"

static List *receiveList;    // theirlist
static List *sendList;       // ourlist
static char* myPort;        // myport
static char* theirPort;     // theirport
static char* theirAddress;  // theiraddress

static pthread_mutex_t receiveMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t sendMutex = PTHREAD_MUTEX_INITIALIZER;

void initalize_system(char* myP, char* theirP, char* theirAdd) {
    sendList = List_create();
    receiveList = List_create();
    myPort = myP;
    theirPort = theirP;
    theirAddress = theirAdd;
}

List* getReceiveList(){
    return receiveList;
}

List* getSendList(){
    return sendList;
}

int lockMutex(pthread_mutex_t mutex){
    pthread_mutex_lock(&mutex);
}

int unlockMutex(pthread_mutex_t mutex){
    pthread_mutex_unlock(&mutex);
}

char* getMyPort(){
    return myPort;
}

char* getTheirPort(){
    return theirPort;
}

char* getTheirAddress(){
    return theirAddress;
}

void cancelSystemFromSender(){
    receiver_cancel();
    keyboard_cancel();
}

void cancelSystemFromReceiver(){
    send_cancel();
    screen_cancel();
}

void end_lists(){
    List_free(receiveList, NULL);
    List_free(sendList, NULL);
}