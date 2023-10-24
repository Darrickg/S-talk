#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "list.h"

void* keyboard(void* arg);
void keyboard_init(void);
void keyboard_signal();
void keyboard_cancel();
void keyboard_shutdown();

#endif