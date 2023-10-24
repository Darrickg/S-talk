#ifndef MANAGE_LISTS_H
#define MANAGE_LISTS_H

#include "list.h"

void initalize_system(char* myP, char* theirP, char* theirAdd);

List* getReceiveList();

List* getSendList();

int lockMutex(pthread_mutex_t mutex);

int unlockMutex(pthread_mutex_t mutex);

char* getMyPort();

char* getTheirPort();

char* getTheirAddress();

void cancelSystemFromSender();

void cancelSystemFromReceiver();

void end_lists();

#endif