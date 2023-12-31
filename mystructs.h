#ifndef MYSTRUCTS_H
#define MYSTRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>

#include "list.h"

struct KeyboardScreenArgs {
    List* list;
    pthread_mutex_t mutex;
};

struct RecvArgs {
    List* list;
    pthread_mutex_t mutex;
    int socket;
};

struct SendArgs {
    List* list;
    pthread_mutex_t mutex;
    int socket;
    struct addrinfo *server_info;
};

#endif