#ifndef LIB_QUEUE_H
#define LIB_QUEUE_H

/* Modulo de implementação do TAD Fila Estática Circular */

typedef struct queue* Queue;
typedef void* Queue_elem;

/* Cria e retorna uma fila estática com no máximo `max_lenght` elementos */
Queue queue_create(int max_length);

/* Retorna 1 caso `queue` esteja vazia; caso contrário, retorna 0 */
int queue_is_empty(Queue queue);

/* Insere `elem` no final de `queue` */
void queue_insert(Queue queue, Queue_elem elem);

/* Remove o primeiro elemento de `queue` */
Queue_elem queue_remove(Queue queue);

/* Destroi a fila apontada por `queue` */
void queue_destroy(Queue* queue);

#endif