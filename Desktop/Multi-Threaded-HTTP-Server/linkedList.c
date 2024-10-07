#include "linkedList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef struct NodeObj NodeObj;

typedef NodeObj *Node;

struct NodeObj {
    char file[90];
    void *rwlock;
    Node next;
    Node prev;
};

Node create_node(char *file, void *rwl) {
    Node node = calloc(1, sizeof(NodeObj));
    strncpy(node->file, file, 89);

    node->file[89] = '\0';
    node->rwlock = rwl;
    node->next = NULL;
    return node;
}

void node_free(Node *n) {
    if (n != NULL && *n != NULL) {
        free(*n);
        free((*n)->rwlock);
        *n = NULL;
    }
}

typedef struct ListObj {
    Node front;
    Node back;
    Node cursor;
    int index;
    int length;
} ListObj;

typedef struct ListObj *List;

List newList() {
    List l = calloc(1, sizeof(ListObj));
    l->front = NULL;
    l->back = NULL;
    l->cursor = NULL;
    l->index = -1;
    l->length = 0;
    return l;
}

void freeList(List *L) {
    if (L != NULL && *L != NULL) {
        clear(*L);
        free(*L);
        *L = NULL;
    }
}

int length(List L) {
    return L->length;
}

int curr_index(List L) {
    return L->index;
}

char *getfile(List L) {
    return L->cursor->file;
}

void *getlock(List L) {
    return L->cursor->rwlock;
}

void clear(List L) {

    while (length(L) > 0) {
        deleteFront(L);
    }
    node_free(&(L)->cursor);
    L->index = -1;
    L->length = 0;
}

void moveFront(List L) {
    if (L->length > 0) {
        L->cursor = L->front;
        L->index = 0;
    }
}

void moveNext(List L) {
    if (L->cursor != NULL) {
        L->cursor = L->cursor->next;
        L->index = L->index + 1;
        if (L->cursor == NULL) {
            L->index = -1;
        }
    }
}

void append(List L, char *filename, void *rwl) {
    Node n = create_node(filename, rwl);
    if (L->length == 0) {
        L->front = n;
        L->back = n;
    } else {
        n->prev = L->back;
        L->back->next = n;
        L->back = n;
    }
    L->length++;
}

void deleteFront(List L) {
    if (L == NULL) {
        fprintf(stderr, "list is null\n");
        exit(1);
    }
    if (L->length <= 0) {
        fprintf(stderr, "list is empty\n");
        exit(1);
    }
    if (L->length == 1) {
        node_free(&(L->front));
        L->front = NULL;
        L->cursor = NULL;
        L->back = NULL;
        L->index = -1;
        L->length = 0;
    } else {
        if (L->cursor == L->front) {
            L->cursor = NULL;
            L->index = -1;
        }

        L->front = L->front->next;
        node_free(&(L->front->prev));
        L->front->prev = NULL;
        L->length -= 1;
        if (L->index != -1) {
            L->index -= 1;
        }
    }
}
