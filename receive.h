#ifndef RECEIVE_H
#define RECEIVE_H

#include "list.h"

void* receive(void* arg);
void receive_init(void);
void receiver_cancel();
void receiver_shutdown();

#endif