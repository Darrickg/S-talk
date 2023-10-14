#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// TODO: honestly, i dont really have a clue how this is gonna be useful in the assignment but atleast i know how it works now. maybe we will get more insight on monday/tuesday lecture

// FIXME: i realise i didnt put any safeguards here, do an if statement for every function so it does not return an error and just messes up our whole code

// mutex is for avoiding conflicts in a thread, basically like a hall pass telling the thread that its their turn to go
// im honestly still not sure how the mutex will work in our assignment, figure it out one of these days
pthread_mutex_t lock;

// each thread will do this
void *print_message_function(void* ptr)
{
    // locks the mutex, telling all the other threads to not run
    pthread_mutex_lock(&lock);

    // prints message and delay 2 seconds
    char* message;
    message = (char*) ptr;
    printf("%s \n", message);
    sleep(2);

    // unlock mutex, telling the others that it is done
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char *argv[])
{

    // initializes lock with the variable mutex lock
    pthread_mutex_init(&lock, NULL);

    // create pthreads 1 and 2, and messages for those threads
    pthread_t thread1, thread2;
    char *message1 = "Thread 1";
    char *message2 = "Thread 2";
    int iret1, iret2;

    // actually creating the thread
    // pthread_create(pthread variable, attributes, fucntion the thread will execute, data passed to the function it will execute)
    // returns 0 if succesful
    iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
    iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);

    // pthread_join just means wait until that specific thread is done before main continues.
    // necessary so main does not end abruptly and just ends the program
    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);

    // prints return values
    printf("Thread 1 returns: %d\n",iret1);
    printf("Thread 2 returns: %d\n",iret2);
    exit(0);
}