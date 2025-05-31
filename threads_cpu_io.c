#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

// Função para cálculo de π usando a série de Leibniz
double calcular_pi(long iteracoes) {
    double pi = 0.0;
    for (long i = 0; i < iteracoes; i++) {
        double termo = (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
        pi += termo;
    }
    return pi * 4;
}

// Thread CPU-bound
void* cpu_thread(void* arg) {
    long id = (long)arg;
    printf("Thread CPU-bound %ld iniciada (PID: %d)\n", id, getpid());

    double resultado = calcular_pi(100000000); // 100 milhões de iterações
    printf("Thread CPU-bound %ld terminou (π ≈ %f)\n", id, resultado);

    return NULL;
}

// Thread I/O-bound
void* io_thread(void* arg) {
    long id = (long)arg;
    printf("Thread I/O-bound %ld iniciada (PID: %d)\n", id, getpid());
    sleep(2);
    printf("Thread I/O-bound %ld terminou\n", id);
    return NULL;
}

int main() {
    pid_t pid;
    pthread_t cpu1, cpu2, cpu3;
    pthread_t io1, io2, io3;

    pid = fork();

    if (pid == 0) {
        // Processo filho
        printf("\nProcesso filho (PID: %d)\n", getpid());

        // Criar threads CPU-bound
        pthread_create(&cpu1, NULL, cpu_thread, (void*)1);
        pthread_create(&cpu2, NULL, cpu_thread, (void*)2);
        pthread_create(&cpu3, NULL, cpu_thread, (void*)3);

        // Criar threads I/O-bound
        pthread_create(&io1, NULL, io_thread, (void*)1);
        pthread_create(&io2, NULL, io_thread, (void*)2);
        pthread_create(&io3, NULL, io_thread, (void*)3);

        // Aguardar término das threads
        pthread_join(cpu1, NULL);
        pthread_join(cpu2, NULL);
        pthread_join(cpu3, NULL);
        pthread_join(io1, NULL);
        pthread_join(io2, NULL);
        pthread_join(io3, NULL) ;

    } else if (pid > 0) {
        // Processo pai
        printf("Processo pai (PID: %d)\n", getpid());
        wait(NULL);
    }

    return 0;
}
