#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	8

// TODO: i basically stole this from the threads example in the website source, im tryna figure out what this does so

// basically, whats going on here is that it lets the computer runs all the threads at once and itll print them in whatever order the computer finishes processing them

char *messages[NUM_THREADS];

void *PrintHello(void *threadid)
{
   long taskid;

   sleep(1);
   taskid = (long) threadid;
   printf("Thread %d: %s\n", taskid, messages[taskid]);
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    long taskids[NUM_THREADS];
    int rc, t;

    messages[0] = "English: Hello World!";
    messages[1] = "French: Bonjour, le monde!";
    messages[2] = "Spanish: Hola al mundo";
    messages[3] = "Klingon: Nuq neH!";
    messages[4] = "German: Guten Tag, Welt!"; 
    messages[5] = "Russian: Zdravstvuyte, mir!";
    messages[6] = "Japan: Sekai e konnichiwa!";
    messages[7] = "Latin: Orbis, te saluto!";

    for(t=0;t<NUM_THREADS;t++) {
    taskids[t] = t;
    printf("Creating thread %d\n", t);

    // pthread_create(pthread variable, attributes, fucntion the thread will execute, data passed to the function it will execute)
    rc = pthread_create(&threads[t], NULL, PrintHello, (void *) taskids[t]);

    if (rc) 
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
  }

pthread_exit(NULL);
}