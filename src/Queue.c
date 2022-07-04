#include <stdlib.h>
#include <stdio.h>

#include "Queue.h"

struct node
{
    char *data;
    struct node *next;
};

static struct node *new_node(char *data)
{
    struct node *n;
    n = (struct node *)malloc(sizeof(struct node));
    n->data = data;
    n->next = NULL;
    return n;
}

struct queue *queue_new()
{
    struct queue *q;
    q = (struct queue *)malloc(sizeof(struct queue));
    q->head = NULL;
    q->tail = NULL;
    q->length = 0;
    return q;
}

int queue_push(struct queue *q, char *data)
{
    if (q != NULL)
    {
        struct node *n = new_node(data);
        if (q->head == NULL)
        {
            q->head = n;
            q->tail = n;
        }
        else
        {
            q->tail->next = n;
            q->tail = q->tail->next;
        }
        q->length++;
        return 0;
    }

    return -1;
}

char *queue_peek_head(struct queue *q)
{
    if (q != NULL && q->head != NULL)
        return q->head->data;

    return NULL;
}

void queue_pop_free_head(struct queue *q)
{
    if (q != NULL)
    {
        struct node *tem;
        if (q->head != NULL)
        {
            tem = q->head;
            q->head = q->head->next;
            if (tem->data != NULL)
                free(tem->data);
            free(tem);
            q->length--;
        }
    }
}

char *queue_pop_head(struct queue *q)
{
    if (q != NULL)
    {
        struct node *tem;
        void *head_data = NULL;
        if (q->head != NULL)
        {
            tem = q->head;
            q->head = q->head->next;
            head_data = tem->data;
            free(tem);
            q->length--;
        }
        return head_data;
    }

    return NULL;
}

void queue_free_all(struct queue *q)
{
    while (q->head != NULL)
    {
        queue_pop_free_head(q);
    }
    free(q);
}

unsigned int queue_get_length(struct queue *q)
{
    if (q != NULL)
        return q->length;

    return 0;
}
