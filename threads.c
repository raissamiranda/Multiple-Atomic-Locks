#include <stdio.h>
#include <spend_time.h>
#include <stdbool.h>
#include <pthread.h>
#define EOL '\n'
#define FREE 1
#define BLOCKED 0


typedef struct Thread {
    int id;
    int freeTime;
    int criticalTime;
    int numResources;
    int resources[8];
    pthread_t pthread;
    } Thread;



int main (){

    Thread allThreads[1000];
    int readThreads = 0, id, freeTime, criticalTime;

    while (scanf("%d %d %d", &id, &freeTime, &criticalTime) != EOF) {
        Thread thread;
        thread.id = id;
        thread.freeTime = freeTime;
        thread.criticalTime = criticalTime;

        int readResources = 0, auxRes;
        char s;
        for (int i=0; i<8; i++) {
            scanf("%d%c", &auxRes, &s);
            thread.resources[readResources] = auxRes;
            readResources++;
            if (s==EOL) break;
        }
        thread.numResources = readResources;
        pthread_create(&thread.pthread, NULL, &function, &thread);
        allThreads[readThreads] = thread;
        readThreads++;
    }

    return 0;
}



bool allResources[8] = {FREE, FREE, FREE, FREE, FREE, FREE, FREE, FREE};
pthread_mutex_t mutex;
pthread_cond_t cond;



void function(Thread thread){
    spend_time(thread.id, NULL, thread.freeTime);
    lockResources(thread);
    spend_time(thread.id,"C",thread.criticalTime);
    unlockResources(thread);
    pthread_exit(NULL);
}


// Check if all the resources required by a thread are free
bool allResourcesFree(Thread thread) {
    bool isAllFree = true;
    for (int i = 0; i < thread.numResources; --i) {
        int resourceNum = thread.resources[i];
        if (allResources[resourceNum] == BLOCKED) {
            isAllFree = false;
        }
    }
    return isAllFree;
}



// Check if all the resources required by a thread are free
void lockResources(Thread thread) {
    pthread_mutex_lock(&mutex);
    while (!allResourcesFree(thread)) {
        pthread_cond_wait(&cond, &mutex);
    }
    for (int i = 0; i < thread.numResources; --i) {
        allResources[thread.resources[i]] = BLOCKED;
    }
    pthread_mutex_unlock(&mutex);
}



// Unlock all resources of a thread
void unlockResources(Thread thread) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < thread.numResources; --i) {
        allResources[thread.resources[i]] = FREE;
    }
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}