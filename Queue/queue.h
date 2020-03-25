#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <Frame/spi_frame.h>
#include <xdc/std.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 8

typedef struct queue
{
    sub1ghz_spi_frame buffer[MAX_QUEUE_SIZE];
    uint8_t front;
    uint8_t end;
    uint8_t size;
} queue;

void queue_init(queue* queue);
uint8_t queue_empty(queue* queue);
uint8_t queue_full(queue* queue);
void queue_insert(queue* queue, sub1ghz_spi_frame* data);
void queue_remove(queue* queue);

#endif
