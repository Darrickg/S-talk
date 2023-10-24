#ifndef MANAGE_THREAD_H
#define MANAGE_THREAD_H

void initialize_system(char* myP, char* theirP, char* theirAdd);
void initialize_threads();
void cancelSystemFromSender();
void cancelSystemFromReceiver();
void shutdown_threads();
void shutdown_system();

#endif