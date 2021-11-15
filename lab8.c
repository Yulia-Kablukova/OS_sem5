#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define num_steps 2000

struct args {
    double sum;
    int id;
};

void calculate(struct args* a) {

    a->sum = 0;

    for (int i = a->id*num_steps; i < a->id*num_steps + num_steps; i++) {
        a->sum += 1.0/(i*4.0 + 1.0);
        a->sum -= 1.0/(i*4.0 + 3.0);
    }
}

int main(int argc, char **argv) {

    int threads_number;

    if (argc == 2) {
        threads_number = atoi(argv[1]);
    } else {
        printf("Incorrect number of arguments\n");
        return 0;
    }

    if (threads_number < 1) {
        printf("Incorrect number of threads\n");
        return 0;
    }

    pthread_t th[threads_number];
    struct args a[threads_number];

    for (int i = 0; i < threads_number; i++) {
        a[i].id = i;
        if (pthread_create(&(th[i]), NULL, calculate, &a[i]) != 0) {
            printf("error while creating a thread %d\n", i);
            pthread_exit(NULL);
        }
    }

    double pi = 0;

    for (int i = 0; i < threads_number; i++) {
        pthread_join(th[i], NULL);
        pi += a[i].sum;
    }
    pi = pi * 4.0;

    printf("pi done - %.15g \n", pi);

    pthread_exit(NULL);
}
