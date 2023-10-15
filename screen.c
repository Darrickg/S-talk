typedef struct sem {
    int value;
    list_of_processes plist; //(processes blocked on this semaphore)
} semaphore;

SEM full = 0; (# of items available for consumption)
SEM empty = N; (# of empty buffer entries)
SEM mutex = 1;
SEM wrt = 1;
int readcount = 0;

void wait_to_display(){
    while (1) {
        P (mutex) ;
        readcount++ ;
        if (readcount == 1)
            P (wrt);
        V (mutex)

        // reading is performed
        
        P (mutex) ;
        readcount-- ;
        if (readcount == 0)
            V (wrt) ;
        V (mutex) ;
    }

}

Consumer: while(1) {
P(full); (wait until at least 1 buffer is filled)
P(mutex); (wait for concurrency OK from producer)
-remove item from the buffer
V(mutex); (signal concurrency OK to producer)
V(empty); (record the addition of 1 empty buffer)
-consume item
}

while(1) {
flag[j] = true;
while(flag[i]) {
if (turn == i) {
flag[j] = false;
while (turn == i);
flag[j] = true;
}
}
Critical Section
turn = i;
flag[j] = false;
Non-critical Section
}
Pi: 

P(S) {
while (S <= 0);
S--;
}
V(S) {
S++;
}
