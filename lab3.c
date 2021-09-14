#include <stdio.h>
#include <pthread.h>
#include "string.h"

void *func(void *str) {

    for(char **i = (char**)str; strcmp(*i, "\0") != 0; i++) {
        printf("%s ", *i);
    }
    printf("\n");

	return NULL;
}

int main() {

    pthread_t th1, th2, th3, th4;

    char* str1[5] = {"Hello", "from", "number", "1", "\0"};
    char* str2[5] = {"Hello", "from", "number", "2", "\0"};
    char* str3[5] = {"Hello", "from", "number", "3", "\0"};
    char* str4[5] = {"Hello", "from", "number", "4", "\0"};

    if (pthread_create(&th1, NULL, func, str1) != 0) {
        printf("error while creating thread 1\n");
        return 0;
    }

    if (pthread_create(&th2, NULL, func, str2) != 0) {
        printf("error while creating thread 2\n");
        return 0;
    }

    if (pthread_create(&th3, NULL, func, str3) != 0) {
        printf("error while creating thread 3\n");
        return 0;
    }

    if (pthread_create(&th4, NULL, func, str4) != 0) {
        printf("error while creating thread 4\n");
        return 0;
    }

    pthread_exit(NULL);
    return 0;
}
