this is specifically only for Darrick to mess around with code to see what works, this wont be in the final assignemtn

this is in the git because i need a source control

client_sockets_play.c and server_sockets_play.c

    please read server sockets first then client sockets

    how to run:
    - use 2 terminals
    - dont forget to cd into the file cd darrick*

    terminal 1 (server, will recieve messages):
    gcc server_sockets_play.c -o server
    ./server

    terminal 2 (client, will send messages):
    gcc client_sockets_play.c -o client
    ./client

    PLEASE RUN SERVER FIRST THEN RUN CLIENT

threads_play.c

    literally the most basic use of thread

    gcc -o thread threads_play.c -lpthread
    ./thread

threads_play_2_electric_boogaloo.c

    gcc -o thread2 threads_play_2_electric_boogaloo.c -lpthread
    ./thread2