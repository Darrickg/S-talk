#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ncurses.h>

void initNcurses() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    scrollok(stdscr, TRUE);  // Enable scrolling
}

void cleanupNcurses() {
    endwin();
}

int main() {
    initNcurses();  // Initialize NCurses

    // STEP 1: Fill in address information using getaddrinfo
    struct addrinfo hints, *server_info;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo("127.0.0.1", "8080", &hints, &server_info) != 0) {
        perror("getaddrinfo failed");
        exit(EXIT_FAILURE);
    }

    // STEP 2: Create a socket
    int client_sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (client_sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    char message[1024];
    int ch;
    
    while (1) {
        printw("Enter message to send (or '!' to quit): ");
        refresh();
        
        int i = 0;
        while ((ch = getch()) != '\n') {
            message[i++] = ch;
            printw("%c", ch);  // Print the character as it's typed
            refresh();
        }
        message[i] = '\0';  // Null-terminate the message

        ssize_t send_len = sendto(client_sock, message, strlen(message), 0, server_info->ai_addr, server_info->ai_addrlen);
        
        if (strcmp(message, "!") == 0) {
            printw("You have ended the chat\n");
            refresh();
            break;
        }

        if (send_len == -1) {
            perror("sendto failed");
        }
    }

    cleanupNcurses();

    freeaddrinfo(server_info);
    close(client_sock);

    return 0;
}
