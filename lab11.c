#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutexattr_t attr;
pthread_mutex_t m[3];

void destroyMutex(int index) {
    for (int i = 0; i < index; ++i) {
        pthread_mutex_destroy(&m[i]);
    }
}

void stop(char *errorMsg) {
    destroyMutex(3);
    perror(errorMsg);
    exit(EXIT_FAILURE);
}

void lockMutex(int index) {
    if (pthread_mutex_lock(&m[index]))
        stop("Error lock mutex");
}

void unlockMutex(int index) {
    if (pthread_mutex_unlock(&m[index]))
        stop("Error unlock mutex");
}

void initMutexes() {
    pthread_mutexattr_init(&attr);
    if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK)) {
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; ++i) {
        if (pthread_mutex_init(&m[i], &attr)) {
            destroyMutex(i);
            exit(EXIT_FAILURE);
        }
    }
}

void *childFunc() {
    int n = 0;
    
    lockMutex(1);
    
    for (int i = 0; i < 10; ++i) {
        lockMutex(n);
        printf("child line %d\n", i);
        unlockMutex((n + 1) % 3);
        n = (n + 2) % 3;
    }
    
    unlockMutex(0);
    
    return NULL;
}

int main() {
    pthread_t th;

    initMutexes();

    lockMutex(0);
    lockMutex(2);

    if (pthread_create(&th, NULL, childFunc, NULL)) {
        printf("error while creating a thread\n");
        pthread_exit(NULL);
    }

    if (sleep(1)) {
        stop("Sleep was interrupted");
        pthread_exit(NULL);
    }

    int n = 0;

    for (int i = 0; i < 10; ++i) {
        printf("parent line %d\n", i);
        unlockMutex(n);
        lockMutex((n + 1) % 3);
        n = (n + 2) % 3;
    }

    unlockMutex(1);
    unlockMutex(2);

    pthread_join(th, NULL);

    destroyMutex(3);
    pthread_exit(EXIT_SUCCESS);
}
