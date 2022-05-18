
#ifndef LINKED_H
#define LINKED_H 1

struct linked_list
{
    struct node(*head_node_ptr);                       // 8 bytes
    int cap;                                           // 4 bytes
    int size;                                          // 4 bytes
    int (*push_item)(struct linked_list(*), int);      // 8 bytes
    int (*slip_item)(struct linked_list(*), int, int); // 8 bytes
    int (*pop_item)(struct linked_list(*));            // 8 bytes
    int (*remove_item)(struct linked_list(*), int);    // 8 bytes
    int (*get)(struct linked_list(*), int);            // 8 bytes
    int (*find)(struct linked_list(*), int);           // 8 bytes
};

struct node
{
    int data;                        // 4 bytes
    char index;                      // 1 byte
    struct node(*previous_node_ptr); // 8 bytes
    struct node(*next_node_ptr);     // 8 bytes
};

extern struct linked_list *
init_linked_list_heap(int);

extern struct linked_list *init_linked_list_stack(struct linked_list(*), int);

// member functions
extern int push_item(struct linked_list(*), int);
extern int slip_item(struct linked_list(*), int, int);

extern int pop_item(struct linked_list(*));
extern int remove_item(struct linked_list(*), int);

extern int get(struct linked_list(*), int);

extern int find(struct linked_list(*), int);

extern void print_list(struct linked_list(*));

// internal
static void reorder_from(struct linked_list(*), int, int);
static struct node *recycle(struct linked_list(*));

#endif