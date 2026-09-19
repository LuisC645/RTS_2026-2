#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 10
#define NUM_ITEMS   20

/* --------------------------------------------------
 * Buffer circular compartido
 * -------------------------------------------------- */

typedef struct {

    int data[BUFFER_SIZE];

    int in;       // próxima posición de escritura
    int out;      // próxima posición de lectura
    int count;    // número de elementos actuales

    pthread_mutex_t mutex;

    pthread_cond_t not_full;
    pthread_cond_t not_empty;

} buffer_t;


/* Buffer compartido */

buffer_t buffer;


/* --------------------------------------------------
 * Inicialización del buffer
 * -------------------------------------------------- */

void buffer_init(buffer_t *b)
{
    b->in = 0;
    b->out = 0;
    b->count = 0;

    pthread_mutex_init(&b->mutex, NULL);

    pthread_cond_init(&b->not_full, NULL);
    pthread_cond_init(&b->not_empty, NULL);
}


/* --------------------------------------------------
 * Productor
 * -------------------------------------------------- */

void *producer()
{
    for (int i = 0; i < NUM_ITEMS; i++) {

        pthread_mutex_lock(&buffer.mutex);

        /*
         * Si el buffer está lleno,
         * el productor debe esperar.
         */

        while (buffer.count == BUFFER_SIZE) {

            printf("Producer: buffer FULL\n");

            pthread_cond_wait(
                &buffer.not_full,
                &buffer.mutex
            );
        }

        /*
         * Producir un elemento.
         */

        buffer.data[buffer.in] = i;

        buffer.in =
            (buffer.in + 1) % BUFFER_SIZE;

        buffer.count++;

        printf(
            "Producer: produced %d "
            "(buffer = %d)\n",
            i,
            buffer.count
        );

        /*
         * Avisar al consumidor que
         * hay un elemento disponible.
         */

        pthread_cond_signal(
            &buffer.not_empty
        );

        pthread_mutex_unlock(&buffer.mutex);
    }

    return NULL;
}


/* --------------------------------------------------
 * Consumidor
 * -------------------------------------------------- */

void *consumer()
{
    for (int i = 0; i < NUM_ITEMS; i++) {

        pthread_mutex_lock(&buffer.mutex);

        /*
         * Si el buffer está vacío,
         * el consumidor debe esperar.
         */

        while (buffer.count == 0) {

            printf("Consumer: buffer EMPTY\n");

            pthread_cond_wait(
                &buffer.not_empty,
                &buffer.mutex
            );
        }

        /*
         * Consumir un elemento.
         */

        int value = buffer.data[buffer.out];

        buffer.out =
            (buffer.out + 1) % BUFFER_SIZE;

        buffer.count--;

        printf(
            "Consumer: consumed %d "
            "(buffer = %d)\n",
            value,
            buffer.count
        );

        /*
         * Avisar al productor que
         * hay espacio disponible.
         */

        pthread_cond_signal(
            &buffer.not_full
        );

        pthread_mutex_unlock(&buffer.mutex);
    }

    return NULL;
}


/* --------------------------------------------------
 * Programa principal
 * -------------------------------------------------- */

int main(void)
{
    pthread_t producer_thread;
    pthread_t consumer_thread;

    buffer_init(&buffer);

    /*
     * Crear productor.
     */

    pthread_create(
        &producer_thread,
        NULL,
        producer,
        NULL
    );

    /*
     * Crear consumidor.
     */

    pthread_create(
        &consumer_thread,
        NULL,
        consumer,
        NULL
    );

    /*
     * Esperar a que terminen.
     */

    pthread_join(
        producer_thread,
        NULL
    );

    pthread_join(
        consumer_thread,
        NULL
    );

    /*
     * Liberar recursos.
     */

    pthread_mutex_destroy(&buffer.mutex);

    pthread_cond_destroy(&buffer.not_full);
    pthread_cond_destroy(&buffer.not_empty);

    printf("\nSimulation finished.\n");

    return 0;
}
