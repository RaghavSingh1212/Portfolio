#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// Forward declarations of the structs to allow for type references
typedef struct NodeObj *Node;
typedef struct ListObj *List;

// Function prototypes
Node create_node(char *file, void *rwl);
void node_free(Node *n);

List newList();
void freeList(List *L);
int length(List L);
int curr_index(List L);
char *getfile(List L);
void *getlock(List L);
void clear(List L);

void moveFront(List L);
void moveNext(List L);
void append(List L, char *filename, void *rwl);
void deleteFront(List L);

#endif // LINKEDLIST_H
