#include "ring_buffer.h"
#include "app_log.h"
// this is function use check ring buffer full.
uint8_t ring_buffer_is_full(ring_buffer_types *queue)
{
  if ((queue->rear + 1) % MAX_SIZE_RING_BUFFER == queue->front) {
    return TRUE;
  }
  return FALSE;
}

// this is function use check ring buffer empty.
uint8_t ring_buffer_is_empty(ring_buffer_types *queue)
{
  if (queue->front == queue->rear) {
    return TRUE;
  }
  return FALSE;
}

// this is function use push data in ring buffer.
status_ring_buffer ring_buffer_push_data(ring_buffer_types *queue, uint8_t *data)
{

  if (!ring_buffer_is_full(queue)) {
    //app_log("queue->rear[%d] = %x\n", queue->rear, queue->data[queue->rear]);
    for (int i = 0; i < SIZE_OF_DATA; i++) {
      queue->data[queue->rear][i] = data[i];
    }
    queue->rear = (queue->rear + 1) % MAX_SIZE_RING_BUFFER ;
    return RING_BUFFER_OK;
  } else {
    app_log("Ring buffer full\n");
  }
  return RING_BUFFER_ERROR;
}
// this is function use pop data out ring buffer.
status_ring_buffer ring_buffer_pop_data(ring_buffer_types *queue, uint8_t *data_out)
{
  if (!ring_buffer_is_empty(queue)) {
    for (int i = 0; i < SIZE_OF_DATA; i++) {
      data_out[i] = queue->data[queue->front][i];
    }
    queue->front = (queue->front + 1) % MAX_SIZE_RING_BUFFER;
    return RING_BUFFER_OK;
  } else {
    app_log("Ring buffer empty\n");
    return RING_BUFFER_ERROR;
  }
}
