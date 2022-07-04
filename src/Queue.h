#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * Structure for holding the queue.
     * It should be used with the queue_new() constructor
     * @param head stores the poiter to the first element
     * @param tail stores the pointer to the last element
     * @param length stores the actual length of the queue
     */
    struct queue
    {
        struct node *head;
        struct node *tail;

        unsigned int length;
    };

    /**
     * Returns a pointer to newly created queue
     */
    struct queue *queue_new();

    /**
     * Ads new data to the tail
     * @param q queue created with queue_new()
     * @param data the data to be added
     */
    int queue_push(struct queue *q, char *data);

    /**
     * Returns (without freeing) a pointer to data in the first element.
     * @param q queue created with queue_new()
     */
    char *queue_peek_head(struct queue *q);

    /**
     * Destroys the first element of queue and calls free() on the data.
     * @param q queue created with queue_new()
     */
    void queue_pop_free_head(struct queue *q);

    /**
     * Destroys the first element of queue and returns the data.
     * In this case the queue is not responsible for freeing the data
     * @param q queue created with queue_new()
     */
    char *queue_pop_head(struct queue *q);

    /**
     * Destroys the queue and calls free() on every element.
     * @param q queue created with queue_new()
     */
    void queue_free_all(struct queue *q);

    /**
     * Returns the actual number of stored elements.
     * @param q queue created with queue_new()
     */
    unsigned int queue_get_length(struct queue *q);

#ifdef __cplusplus
}
#endif

#endif