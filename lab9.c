#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#define num_steps 20000000

int CTRL_C_FLAG = 0;

struct args {
    double sum;
    int id;
    int threads_number;
    int tasks_count;
};

void calculate(struct args* a) {
    while (!CTRL_C_FLAG) {
        for (int i = a->id*num_steps; i < a->id*num_steps + num_steps; i++) {
            if (i % 1000000 == 0 && CTRL_C_FLAG) {
                pthread_exit(NULL);
            }
            a->sum += 1.0/(i*4.0 + 1.0);
            a->sum -= 1.0/(i*4.0 + 3.0);
        }
        a->id += a->threads_number;
    }
}

void finish(struct args* a) {
    for (int j = 0; j < a->tasks_count; j++) {
        for (int i = a->id*num_steps; i < a->id*num_steps + num_steps; i++) {
            if (i % 1000000 == 0 && CTRL_C_FLAG) {
                pthread_exit(NULL);
            }
            a->sum += 1.0/(i*4.0 + 1.0);
            a->sum -= 1.0/(i*4.0 + 3.0);
        }
        a->id += a->threads_number;
    }
}

void my_handler(sig_atomic_t s){
    CTRL_C_FLAG = 1;
}

int main(int argc, char **argv) {

    signal (SIGINT, my_handler);

    int threads_number;

    if (argc == 2) {
        threads_number = atoi(argv[1]);
    } else {
        printf("Incorrect number of threads\n");
        return 0;
    }

    if (threads_number < 1) {
        printf("Incorrect number of threads\n");
        return 0;
    }

    pthread_t th[threads_number];
    struct args a[threads_number];
    double pi = 0;
    int id = 0;

    for (int i = 0; i < threads_number; i++) {
        a[i].sum = 0;
        a[i].threads_number = threads_number;
    }

    for (int i = 0; i < threads_number; i++) {
        a[i].id = id++;
        if (pthread_create(&(th[i]), NULL, (void *(*)(void *)) calculate, &a[i]) != 0) {
            printf("error while creating a thread %d\n", i);
            pthread_exit(NULL);
        }
    }

    for (int i = 0; i < threads_number; i++) {
        pthread_join(th[i], NULL);
    }

    int max_tasks_count = a[0].id % threads_number;

    for (int i = 1; i < threads_number; i++) {
        if (max_tasks_count < (a[i].id % threads_number - i)) {
            max_tasks_count = a[i].id % threads_number - i;
        }
    }

    int wait[threads_number];

    for (int i = 0; i < threads_number; i++) {
        wait[i] = 0;
        if (max_tasks_count > (a[i].id % threads_number - i)) {
            wait[i] = 1;
            a->tasks_count = max_tasks_count - a[i].id % threads_number + i;
            if (pthread_create(&(th[i]), NULL, (void *(*)(void *)) finish, &a[i]) != 0) {
                printf("error while creating a thread %d\n", i);
                pthread_exit(NULL);
            }
        }
    }

    for (int i = 0; i < threads_number; i++) {
        if (wait[i]) {
            pthread_join(th[i], NULL);
        }
        pi += a[i].sum;
    }


    pi = pi * 4.0;

    printf("\npi done - %.15g \n", pi);

    pthread_exit(NULL);
}
