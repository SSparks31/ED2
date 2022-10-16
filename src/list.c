#include "list.h"

#include <stdio.h>
#include <stdlib.h>

struct list_pos {
    List_elem elem;

    List_pos next;
    List_pos prev;
};

struct list {
    List_pos first;
    List_pos last;

    int size;
};

List list_create() {
    List list = malloc(sizeof(struct list));

    if (!list) {
        return NULL;
    }

    list->first = NULL;
    list->last = NULL;
    list->size = 0;

    return list;
}

int list_get_size(List list) {
    if (!list) {
        return -1;
    }

    return list->size;
}

List_elem list_get_elem(List list, List_pos pos) {
    if (!list || !pos) {
        return NULL;
    }

    return pos->elem;
}

List_pos list_append(List list, List_elem elem) {
    if (!list || !elem) {
        return NULL;
    }

    List_pos new_pos = malloc(sizeof(struct list_pos));
    if (!new_pos) {
        return NULL;
    }


    new_pos->elem = elem;
    new_pos->prev = list_get_last(list);
    new_pos->next = NULL;

    List_pos first = list_get_first(list);
    if (!first) {
        list->first = new_pos;
    }

    List_pos last = list_get_last(list);
    if (last) {
        last->next = new_pos;
    }

    list->last = new_pos;

    ++list->size;

    return new_pos;
}

List_pos list_insert_before(List list, List_pos pos, List_elem elem) {
    if (!list || !pos || !elem) {
        return NULL;
    }

    List_pos new_pos = malloc(sizeof(struct list_pos));
    if (!new_pos) {
        return NULL;
    }

    new_pos->elem = elem;
    new_pos->next = pos;

    List_pos pos_prev = list_get_prev(list, pos);

    if (pos_prev) {
        pos_prev->next = new_pos;
    } else {
        list->first = new_pos;
    }

    pos->prev = new_pos;
    new_pos->prev = pos_prev;

    ++list->size;
    
    return new_pos;
}

List_pos list_insert_after(List list, List_pos pos, List_elem elem) {
    if (!list || !pos || !elem) {
        return NULL;
    }

    List_pos new_pos = malloc(sizeof(struct list_pos));
    if (!new_pos) {
        return NULL;
    }

    new_pos->elem = elem;
    new_pos->prev = pos;

    List_pos pos_next = list_get_next(list, pos);

    if (pos_next) {
        pos_next->prev = new_pos;
    } else {
        list->last = new_pos;
    }

    pos->next = new_pos;
    new_pos->next = pos_next;

    ++list->size;
    
    return new_pos;
}

void list_remove(List list, List_pos pos) {
    if (!list || !pos) {
        return;
    }

    List_pos next = list_get_next(list, pos);
    List_pos prev = list_get_prev(list, pos);

    if (next) next->prev = prev;
    if (prev) prev->next = next;

    List_pos first = list_get_first(list);
    List_pos last = list_get_last(list);

    if (pos == first) {
        list->first = next;
    }
    if (pos == last) {
        list->last = prev;
    }

    free(pos);

    --list->size;
}

List_pos list_get_first(List list) {
    if (!list) {
        return NULL;
    }

    return list->first;
}

List_pos list_get_next(List list, List_pos pos) {
    if (!list || !pos) {
        return NULL;
    }

    return pos->next;
}

List_pos list_get_prev(List list, List_pos pos) {
    if (!list || !pos) {
        return NULL;
    }

    return pos->prev;
}

List_pos list_get_last(List list) {
    if (!list) {
        return NULL;
    }

    return list->last;
}

void list_destroy(List* list) {
    if (!list || !*list) {
        return;
    }

    if (list_get_size(*list) != 0) {
        printf("\n------------\n");
        printf("ATENCAO:\nO desalocamento de elementos da lista nao e realizado automaticamente; caso seja necessario, modifique seu programa para realizar a remocao e desalocamento.\n");
    }

    List_pos cur;
    while ((cur = list_get_last(*list))) {
        list_remove(*list, cur);
    }

    free(*list);
    *list = NULL;
}