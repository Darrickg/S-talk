#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "keyboard.h"
#include "screen.h"
#include "receive.h"
#include "send.h"
#include "manage_thread.h"

static pthread_t keyboardThread, screenThread, receiveThread, sendThread;

void initialize_threads(pthread_t keyboard, pthread_t screen, pthread_t receive, pthread_t send){
    keyboardThread = keyboard;
    screenThread = screen;
    receiveThread = receive;
    sendThread = send;
}

void cancelReceive(){
    fputs("recv: cancel called\n", stdout);
    pthread_cancel(receiveThread);
}

void cancelKeyboard(){
    fputs("keyboard: cancel called\n", stdout);
    pthread_cancel(keyboardThread);
}

void cancelScreen(){
    
    pthread_cancel(screenThread);
}

void cancelSend(){
    fputs("send: cancel called\n", stdout);
    pthread_cancel(sendThread);
}