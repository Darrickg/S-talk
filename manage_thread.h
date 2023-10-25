#ifndef MANAGE_THREAD_H
#define MANAGE_THREAD_H

#include "list.h"

void initialize_threads(pthread_t keyboard, pthread_t screen, pthread_t receive, pthread_t send);

void cancelReceive();

void cancelKeyboard();

void cancelScreen();

void cancelSend();

#endif 