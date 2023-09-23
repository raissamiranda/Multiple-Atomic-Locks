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



void* function(void* t);
bool allResourcesFree(int numResources, int resources[]);
void lockResources(int numResources, int resources[]);
void unlockResources(int numResources, int resources[]);



bool allResources[8] = {FREE, FREE, FREE, FREE, FREE, FREE, FREE, FREE};
pthread_mutex_t mutex;
pthread_cond_t cond;
Thread allThreads[1000];



int main (){
	pthread_cond_init (&cond, NULL);
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
		allThreads[readThreads] = thread;
		pthread_create(&allThreads[readThreads].pthread, NULL, &function, (void *) &allThreads[readThreads]);
		readThreads++;
	}

	for (int i = 0; i < 1000; i++) {
		pthread_join(allThreads[i].pthread, NULL);
	}

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

	return 0;
}



void* function(void* t){
	struct Thread* thread = (struct Thread *) t;
	spend_time(thread->id, NULL, thread->freeTime);
	lockResources(thread->numResources, thread->resources);
	spend_time(thread->id,"C",thread->criticalTime);
	unlockResources(thread->numResources, thread->resources);
	pthread_exit(NULL);
}



// Check if all the resources required by a thread are free
bool allResourcesFree(int numResources, int resources[]) {
	bool isAllFree = true;
	for (int i = 0; i < numResources; i++) {
		int resourceNum = resources[i];
		if (allResources[resourceNum] == BLOCKED) {
			isAllFree = false;
			break;
		}
	}
	return isAllFree;
}



// Check if all the resources required by a thread are free
void lockResources(int numResources, int resources[]) {
	pthread_mutex_lock(&mutex);
	while (!allResourcesFree(numResources, resources)) {
		pthread_cond_wait(&cond, &mutex);
	}
	for (int i = 0; i < numResources; i++) {
		allResources[resources[i]] = BLOCKED;
	}
	pthread_mutex_unlock(&mutex);
}



// Unlock all resources of a thread
void unlockResources(int numResources, int resources[]) {
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < numResources; i++) {
		allResources[resources[i]] = FREE;
	}
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
}