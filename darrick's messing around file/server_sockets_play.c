#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// This also has a step by step guide on how to do sockets, written by yours truly. Ill try to explain whats going on with my barely understanding knowledge :D

// server side: recieves the message

// start reading from here

void server() {
    
    // TODO: STEP 1: fill in address information using getaddrinfo
    struct addrinfo hints, *server_info;

    // making sure the struct is empty
    memset(&hints, 0, sizeof(hints));

    // set family, socket type, type of IP address (i think?)
    // structure of addrinfo can be found in beej
    hints.ai_family = AF_INET;        // AF_UNSPEC means it can be for ipv4 or 6, but we are doing ipv4
    hints.ai_socktype = SOCK_DGRAM;   // UDP ip
    hints.ai_flags = AI_PASSIVE;      // use local IP address

    // call getaddrinfo to obtain address information, basically fills in the blanks in our hints object that we didnt set
    // NULL is the IP address, we dont specify it and the system will fill it in for us, since we used local IP address
    // "8080 is the port number"
    // &hints is pointer to addrinfo object
    // $server_info is pointer to a linked list of addrinfo structures (this is different from our own linked list, addrinfo has its own internal linked list that connects to other addrinfos)
    if (getaddrinfo(NULL, "8080", &hints, &server_info) != 0)
    {
        // if its not 0 then its some type of error, who cares
        perror("getaddrinfo failed");
        exit(EXIT_FAILURE);
    }

    // TODO: STEP 2: create a socket (the thing that will recieve and send our messages, an "airport" if u will)
    // SF_INET and SOCK_DGRAM refers to it being ipv4 and UDP
    // 0 means use default protocol
    // return -1 if failed
    int server_sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (server_sock == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // TODO: STEP 3: bind socket to the address, in step 1 we created the address, step 2 we created the socket. this is connecting them together

    // bind does that
    // bind(socket, address, lenght of address)
    if (bind(server_sock, server_info->ai_addr, server_info->ai_addrlen) == -1)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // infinite loop that will keep checking if we get a message
    while(1)
    {
        char buffer[1024];                                      // the sentence
        struct sockaddr_storage client_addr;                    // structure to store client address
        socklen_t client_addr_len = sizeof(client_addr);        // size of clients address

        // TODO: STEP 4: recieve UDP datagram, aka actually checking if the server recieved a message

        // reads incoming data from client side
        // this will set the client address etc
        // recvform(socket, storage to store sentence, size of sentence minus null termination, flags, pointer to client address, pointer to client address length)
        // returns -1 on error
        int recv_len = recvfrom(server_sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&client_addr, &client_addr_len);

        // error
        if (recv_len == -1)
        {
            perror("recvfrom failed");
            continue;
        }

        // Null-terminate the recieved data (end of sentence)
        buffer[recv_len] = '\0';

        // if user types !, end the chat
        if (strcmp(buffer, "!\n") == 0)
        {
            printf("Darrick has ended the chat\n");
            break;
        }

        // TODO: STEP 5: process recieved data (printing it lmao)
        printf("Darrick says: %s", buffer);
    }

    // TODO: STEP 6: free the address and close the socket
    freeaddrinfo(server_info);
    close(server_sock);
}

// runs server
int main() {
    server();
    return 0;
}