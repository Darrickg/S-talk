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

static pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;

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

int enqueueLists(List* list, char* message){
    int prepVal;
    pthread_mutex_lock(&queueMutex);
    {
        prepVal = List_append(list, message);
    }
    pthread_mutex_unlock(&queueMutex);
    return prepVal;
}

char* dequeueLists(List* list){
    char* message;
    pthread_mutex_lock(&queueMutex);
    {
        message = List_trim(list);
    }
    pthread_mutex_unlock(&queueMutex);

    return message;
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