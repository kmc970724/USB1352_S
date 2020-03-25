#include <Queue/queue.h>

void queue_init(queue* queue)
{
    queue->front = 0;
    queue->end = 0;
    queue->size = 0;
}

uint8_t queue_empty(queue* queue)
{
    if (queue->size == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

uint8_t queue_full(queue* queue)
{
    if (queue->size == MAX_QUEUE_SIZE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void queue_insert(queue* queue, sub1ghz_spi_frame* data)
{
    if (queue_full(queue))
    {
        return;
    }
    else
    {
        memcpy(&queue->buffer[queue->end], data, sizeof(sub1ghz_spi_frame));
        queue->size += 1;
        queue->end += 1;

        if (queue->end >= MAX_QUEUE_SIZE)
        {
            queue->end = queue->end % MAX_QUEUE_SIZE;
        }
    }
}

void queue_remove(queue* queue)
{
    if (queue_empty(queue))
    {
        return;
    }
    else
    {
        queue->front += 1;
        if (queue->front >= MAX_QUEUE_SIZE)
        {
            queue->front = queue->front % MAX_QUEUE_SIZE;
        }
        queue->size -= 1;
    }
}
