/*
 * ring_buffer.h
 *
 *  Created on: 18 thg 8, 2023
 *      Author: ADMIN
 */

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_


#include <stdint.h>
#include <stdlib.h>

#define MAX_SIZE_BUFFER 10
#define TRUE 1
#define FALSE 0
#define SIZE_OF_DATA 9
enum { MAX_SIZE_RING_BUFFER = 8};
// this is status of ring buffer.
typedef enum {
    RING_BUFFER_ERROR,
    RING_BUFFER_OK
}status_ring_buffer;

// this is type data ring buffer.
typedef struct {
    uint8_t front;//phần tử đầu tiên trong ring buffer
    uint8_t rear;//phần tử cuối cùng trong ring buffer
    uint8_t data[MAX_SIZE_RING_BUFFER][SIZE_OF_DATA];

}ring_buffer_types;

// this is function use check ring buffer empty.
uint8_t ring_buffer_is_full(ring_buffer_types *queue);

// this is function use check ring buffer full.
uint8_t ring_buffer_is_full(ring_buffer_types *queue);

// this is function use push data in ring buffer.
status_ring_buffer ring_buffer_push_data(ring_buffer_types *queue, uint8_t *data);

// this is function use pop data out ring buffer.
status_ring_buffer ring_buffer_pop_data(ring_buffer_types *queue, uint8_t *data_out);

// this is function use show data.
void ring_buffer_show_data(ring_buffer_types *queue);


#endif /* RING_BUFFER_H_ */
