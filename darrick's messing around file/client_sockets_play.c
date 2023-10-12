#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// This also has a step by step guide on how to do sockets, written by yours truly. Ill try to explain whats going on with my barely understanding knowledge :D

// client side: sends the message

// make sure to read the server side first

void client() {

    // TODO: STEP 1: fill in address information using getaddrinfo
    struct addrinfo hints, *server_info;

    // making sure struct is empty
    memset(&hints, 0, sizeof(hints));

    // set family, socket type, type of IP address (i think?)
    // structure of addrinfo can be found in beej
    hints.ai_family = AF_INET;        // AF_UNSPEC means it can be for ipv4 or 6, but we are doing ipv4
    hints.ai_socktype = SOCK_DGRAM;   // UDP ip

    // call getaddrinfo to obtain address information, basically fills in the blanks in our hints object that we didnt set
    // 127.0.0.1 is the IP address, we specify it here unlike in server side because 127.0.0.1 is ip server of localhost, which is what it defaults to in the server when we didnt specify it
    // "8080 is the port number"
    // &hints is pointer to addrinfo object
    // $server_info is pointer to a linked list of addrinfo structures (this is different from our own linked list, addrinfo has its own internal linked list that connects to other addrinfos)
    // return not 0 when failed
    if (getaddrinfo("127.0.0.1", "8080", &hints, &server_info) != 0)
    {
        perror("getaddrinfo failed");
        exit(EXIT_FAILURE);
    }

    // TODO: STEP 2: create a socket (the thing that will recieve and send our messages, an "airport" if u will)
    // SF_INET and SOCK_DGRAM refers to it being ipv4 and UDP
    // 0 means use default protocol
    // return -1 if failed
    int client_sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (client_sock == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // infinite loop that will check if we sent a message
    while (1)
    {
        char message[1024];
        printf("Enter message to send (or '!' to quit): ");

        // reads message from user, stdin means standard input
        fgets(message, sizeof(message), stdin);

        // TODO: STEP 3: send the message to server
        // sendto sends data to the socket
        // sendto(socket, message, length of message, flags, address info of server, length of address)
        ssize_t send_len = sendto(client_sock, message, strlen(message), 0, server_info->ai_addr, server_info->ai_addrlen);

        if (send_len == -1)
        {
            perror("sendto failed");
        }

        // check to quit, the reason we have \n in both here and server side is because fgets immediately adds \n to message
        if (strcmp(message, "!\n") == 0)
        {
            printf("you have ended the chat\n");
            break;
        }
    }

    // TODO: STEP 4: free everything
    freeaddrinfo(server_info);
    close(client_sock);
}

int main() {
    client();
    return 0;
}