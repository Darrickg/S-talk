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
    noecho();
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
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, "8080", &hints, &server_info) != 0) {
        perror("getaddrinfo failed");
        exit(EXIT_FAILURE);
    }

    // STEP 2: Create a socket
    int server_sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (server_sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (bind(server_sock, server_info->ai_addr, server_info->ai_addrlen) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    WINDOW* chatWin = newwin(LINES - 1, COLS, 0, 0);
    scrollok(chatWin, TRUE);

    while (1) {
        char buffer[1024];
        struct sockaddr_storage client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        int recv_len = recvfrom(server_sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&client_addr, &client_addr_len);

        if (recv_len == -1) {
            perror("recvfrom failed");
            continue;
        }

        buffer[recv_len] = '\0';

        if (strcmp(buffer, "!\n\0") == 0) {
            printw("Darrick has ended the chat\n");
            refresh();
            break;
        }

        wprintw(chatWin, "Darrick says: %s\n", buffer);
        wrefresh(chatWin);
    }

    cleanupNcurses();

    freeaddrinfo(server_info);
    close(server_sock);

    return 0;
}
