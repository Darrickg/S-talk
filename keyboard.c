typedef struct sem {
    int value;
    list_of_processes plist; //(processes blocked on this semaphore)
} semaphore;

SEM full = 0; (# of items available for consumption)
SEM empty = N; (# of empty buffer entries)
SEM mutex = 1;

void Wait_For_Input(){
    while (1){
        pthread_mutex_lock(mutex){
            pthread_cond_wait(condition, mutex)
        }
        pthread_mutex_unlock(mutex)
    }
}
Producer: while (1) {
-produce new item

pthread_mutex_lock(mutex){
    pthread_cond_wait(condition, mutex)
}
pthread_mutex_unlock(mutex)

P(empty); (make sure we have at least 1 empty buffer)
P(mutex); (to guard against concurrent buffer manipulation)
-add the new item to the buffer
V(mutex); (signal OK for buffer access by consumer)
V(full); (records addition of item to buffer)
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
