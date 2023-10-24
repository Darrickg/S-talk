#ifndef MANAGE_LISTS_H
#define MANAGE_LISTS_H

#include "list.h"

void initalize_system(char* myP, char* theirP, char* theirAdd);

List* getReceiveList();

List* getSendList();

int enqueueLists(List* list, char* message);

char* dequeueLists(List* list);

char* getMyPort();

char* getTheirPort();

char* getTheirAddress();

void cancelSystemFromSender();

void cancelSystemFromReceiver();

void end_lists();

#endif