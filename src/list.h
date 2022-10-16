#ifndef LIB_LIST_H
#define LIB_LIST_H

/* Modulo de implementação do TAD Lista dinâmica duplamente encadeada */

typedef struct list* List;
typedef struct list_pos* List_pos;
typedef void* List_elem;

/* Cria e retorna uma lista */
List list_create();

/* Retorna o tamanho atual de `list` */
int list_get_size(List list);

/* Retorna o elemento armazenado na posição `pos` */
List_elem list_get_elem(List list, List_pos pos);

/* Insere `elem` no final de `list` */
List_pos list_append(List list, List_elem elem);

/* Insere `elem` antes da posição `pos` */
List_pos list_insert_before(List list, List_pos pos, List_elem elem);

/* Insere `elem` depois da posição `pos` */
List_pos list_insert_after(List list, List_pos pos, List_elem elem);

/* Remove a posição `pos` da lista */
void list_remove(List list, List_pos pos);

/* Retorna a primeira posição de `list` */
List_pos list_get_first(List list);

/* Retorna a posição depois de `pos` */
List_pos list_get_next(List list, List_pos pos);

/* Retorna a posição antes de `pos` */
List_pos list_get_prev(List list, List_pos pos);

/* Retorna a última posição de `pos` */
List_pos list_get_last(List list);

/* Destroi a lista apontada por `list` */
void list_destroy(List* list);

#endif