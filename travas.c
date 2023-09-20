#include <stdio.h>
#define EOL '\n'


typedef struct Thread {
    int id;
    int freeTime;
    int criticalTime;
    int numResources;
    int resources[8];
    } Thread;


int main (int arg, char *argv[]){

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
        allThreads[readThreads] = thread;
        readThreads++;
    }


    for (int i = 0; i < 4 ; i++){
        printf("thread %d:  ", i);
        printf("id: %d  freeTime: %d  criticalTime: %d ", allThreads[i].id, allThreads[i].freeTime, allThreads[i].criticalTime);
            printf(" resources :");
        for (int j = 0; j < allThreads[i].numResources; j++) {
            printf("%d ", allThreads[i].resources[j]);
        }
        printf("\n");
    }


    return 0;
}