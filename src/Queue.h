#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
extern "C"
{
#endif
    // structure for holding the queue
    struct queue
    {
        struct node *head;
        struct node *tail;

        unsigned int length;
    };

    struct queue *queue_new();

    int queue_push(struct queue *q, char *data);

    char *queue_peek_head(struct queue *q);

    void queue_pop_free_head(struct queue *q);

    char *queue_pop_head(struct queue *q);

    void queue_free_all(struct queue *q);

    unsigned int queue_get_length(struct queue *q);

#ifdef __cplusplus
}
#endif

#endif