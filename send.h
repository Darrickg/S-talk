#ifndef SEND_H
#define SEND_H

#include "list.h"

void* sends(void* arg);
void send_init();
void send_signal();
void send_cancel();
void send_shutdown();

#endif