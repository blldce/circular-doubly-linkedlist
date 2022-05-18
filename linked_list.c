#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

#define nullptr (void(*))0

struct linked_list *init_linked_list_heap(int cap)
{
    struct linked_list(*linked_list_ptr) = (struct linked_list(*))malloc(sizeof(struct linked_list));
    linked_list_ptr->size = 0;
    linked_list_ptr->cap = cap;
    linked_list_ptr->head_node_ptr = nullptr;
    linked_list_ptr->push_item = push_item;
    linked_list_ptr->slip_item = slip_item;
    linked_list_ptr->pop_item = pop_item;
    linked_list_ptr->remove_item = remove_item;
    linked_list_ptr->find = find;
    linked_list_ptr->get = get;
    return linked_list_ptr;
}

struct linked_list *init_linked_list_stack(struct linked_list(*linked_list_ptr), int cap)
{
    linked_list_ptr->size = 0;
    linked_list_ptr->cap = cap;
    linked_list_ptr->head_node_ptr = nullptr;
    return linked_list_ptr;
}

// member function impls

int push_item(struct linked_list(*linked_list_ptr), int data)
{
    struct node(*node_ptr);
    if (linked_list_ptr->head_node_ptr == nullptr) // first push?
    {
        node_ptr = (struct node(*))malloc(sizeof(struct node));
        node_ptr->index++;
        node_ptr->previous_node_ptr = nullptr;
        node_ptr->next_node_ptr = nullptr;
    }
    else
    { // nope
        if (linked_list_ptr->size >= linked_list_ptr->cap)
        {
            node_ptr = recycle(linked_list_ptr);
            if (node_ptr == nullptr)
                return 0;
        }
        else
        {
            node_ptr = (struct node(*))malloc(sizeof(struct node));
            node_ptr->index = linked_list_ptr->head_node_ptr->index + 1;
        }
        node_ptr->previous_node_ptr = linked_list_ptr->head_node_ptr;
        node_ptr->next_node_ptr = nullptr;
        linked_list_ptr->head_node_ptr->next_node_ptr = node_ptr;
    }
    node_ptr->data = data;
    linked_list_ptr->head_node_ptr = node_ptr;
    linked_list_ptr->size++;
    return node_ptr->index;
}

static struct node *recycle(struct linked_list(*linked_list_ptr))
{
    struct node(*curr_node_ptr) = linked_list_ptr->head_node_ptr;
    while (curr_node_ptr->index > 0)
    {
        if (curr_node_ptr->index == 1)
        {
            linked_list_ptr->size = 0;
            return curr_node_ptr;
        }
        curr_node_ptr = curr_node_ptr->previous_node_ptr;
    }
    return nullptr;
}

int slip_item(struct linked_list(*linked_list_ptr), int index, int data)
{
    if (index > linked_list_ptr->cap || index < 1) // yes size, not cap
        return 0;

    struct node(*curr_node_ptr) = linked_list_ptr->head_node_ptr;
    int count = 0;
    while (count < linked_list_ptr->cap)
    {
        if (curr_node_ptr->index == index)
        {
            struct node(*sandwich_node_ptr);

            if (linked_list_ptr->size >= linked_list_ptr->cap)
            {
                sandwich_node_ptr = recycle(linked_list_ptr);
                if (sandwich_node_ptr == nullptr)
                    return 0;
                sandwich_node_ptr->previous_node_ptr = nullptr;
                sandwich_node_ptr->next_node_ptr = curr_node_ptr;
            }
            else
            {
                reorder_from(linked_list_ptr, curr_node_ptr->index - 1, 1);
                sandwich_node_ptr = (struct node(*))malloc(sizeof(struct node));
                sandwich_node_ptr->previous_node_ptr = curr_node_ptr->previous_node_ptr;
                sandwich_node_ptr->next_node_ptr = curr_node_ptr;
                curr_node_ptr->previous_node_ptr->next_node_ptr = sandwich_node_ptr;
                sandwich_node_ptr->index = index;
            }

            sandwich_node_ptr->data = data;

            curr_node_ptr->previous_node_ptr = sandwich_node_ptr;

            linked_list_ptr->size++;

            return 1;
        }
        curr_node_ptr = curr_node_ptr->previous_node_ptr;
        count++;
    }

    return 0;
}

int remove_item(struct linked_list(*linked_list_ptr), int index)
{
    if (index > linked_list_ptr->cap || index < 1)
        return 0;

    struct node(*curr_node_ptr) = linked_list_ptr->head_node_ptr;
    int count = 0;
    while (count < linked_list_ptr->cap)
    {
        if (curr_node_ptr->index == index)
        {
            linked_list_ptr->size--;
            reorder_from(linked_list_ptr, curr_node_ptr->index, 0);

            if (curr_node_ptr->index == linked_list_ptr->cap) // head
            {
                curr_node_ptr->previous_node_ptr->next_node_ptr = curr_node_ptr->next_node_ptr;
                linked_list_ptr->head_node_ptr = curr_node_ptr->previous_node_ptr;
                return 1;
            }

            if (curr_node_ptr->index != 1) // not tie
                curr_node_ptr->previous_node_ptr->next_node_ptr = curr_node_ptr->next_node_ptr;
            curr_node_ptr->next_node_ptr->previous_node_ptr = curr_node_ptr->previous_node_ptr;
            free(curr_node_ptr);

            return 1;
        }
        curr_node_ptr = curr_node_ptr->previous_node_ptr;
        count++;
    }

    return 0;
}

int pop_item(struct linked_list(*linked_list_ptr))
{
    if (linked_list_ptr->head_node_ptr == nullptr)
        return 0;

    int pop_val = linked_list_ptr->head_node_ptr->data;
    struct node(*temp_node_ptr) = linked_list_ptr->head_node_ptr->previous_node_ptr;
    free(linked_list_ptr->head_node_ptr);
    temp_node_ptr->next_node_ptr = nullptr;
    linked_list_ptr->head_node_ptr = temp_node_ptr;
    linked_list_ptr->size--;
    return pop_val;
}

int get(struct linked_list(*linked_list_ptr), int index)
{
    if (index > linked_list_ptr->cap || index < 0)
        return 0;

    struct node(*curr_node_ptr) = linked_list_ptr->head_node_ptr;
    int count = 0;
    while (count < linked_list_ptr->cap)
    {
        if (curr_node_ptr->index == index)
            return curr_node_ptr->data;

        curr_node_ptr = curr_node_ptr->previous_node_ptr;
        if (curr_node_ptr == nullptr)
            return 0; // // cap not filled yet!
        count++;
    }
    return 0;
}

int find(struct linked_list(*linked_list_ptr), int data)
{
    struct node(*curr_node_ptr) = linked_list_ptr->head_node_ptr;
    int count = 0;
    while (count < linked_list_ptr->cap)
    {
        if (curr_node_ptr->data == data)
            return curr_node_ptr->index;

        curr_node_ptr = curr_node_ptr->previous_node_ptr;
        if (curr_node_ptr == nullptr)
            return 0; // cap not filled yet!
        count++;
    }
    return 0;
}

void print_list(struct linked_list(*linked_list_ptr))
{
    struct node(*curr_node_ptr) = linked_list_ptr->head_node_ptr;
    int count = 0;
    while (count < linked_list_ptr->cap)
    {
        printf("Node : Index : %d -- Node Value : %d  -- Node Address : %p \n", curr_node_ptr->index, curr_node_ptr->data, curr_node_ptr);
        curr_node_ptr = curr_node_ptr->previous_node_ptr;
        if (curr_node_ptr == nullptr)
            return; // cap not filled yet!
        count++;
    }
}

static void reorder_from(struct linked_list(*linked_list_ptr), int index_to, int is_inc)
{
    struct node(*curr_node_ptr) = linked_list_ptr->head_node_ptr;
    int resize_index = curr_node_ptr->index;
    while (resize_index > index_to)
    {
        curr_node_ptr->index = is_inc ? curr_node_ptr->index + 1 : curr_node_ptr->index - 1;
        curr_node_ptr = curr_node_ptr->previous_node_ptr;
        resize_index--;
    }
}
