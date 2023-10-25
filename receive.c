#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "list.h"
#include "mystructs.h"
#include "receive.h"
#include "manage_thread.h"

#define MESSAGE_LENGTH 1024

/*
takes an input from the other party and puts it into the list

arguments: their list, their mutex, local (our) port
*/
void* receive(void* arg) {

    struct RecvArgs* recvArgs = (struct RecvArgs*)arg;

    List* list = recvArgs->list;
    pthread_mutex_t mutex = recvArgs->mutex;
    int udpSocket = recvArgs->socket;

    while(1)
    {
        pthread_testcancel();
        
        char input[MESSAGE_LENGTH];
        struct sockaddr_storage client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        // check to see if a message is recieved
        int recv_len = recvfrom(udpSocket, input, sizeof(input) - 1, 0, (struct sockaddr*)&client_addr, &client_addr_len);

        if (recv_len == -1)
        {
            perror("recvfrom in recv failed");
            continue;
        }

        input[recv_len] = '\0';

        // push into list
        if (strcmp(input, "\n") != 0)
        {
            // locks the mutex
            pthread_mutex_lock(&mutex);

            List_append(list, input);

            // unlocks mutex
            pthread_mutex_unlock(&mutex);
        }

        if (strcmp(input, "!\n") == 0)
        {   
            *(recvArgs->flag) = 1;
            fputs("recv: they ended the chat\n", stdout);

            cancelKeyboard();
            cancelSend();
            break;
        }

    }

    return NULL;
}