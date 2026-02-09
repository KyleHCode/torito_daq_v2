#include "ringbuffer.h"

RingBuffer::RingBuffer() : head(0), tail(0), count(0), overrun_count(0) {
    // Constructor - initialize members
}

bool RingBuffer::push(const SampleFrame *frame) {
    if (count >= RING_BUFFER_SIZE) {  // Direct access, no rb->
        overrun_count++;
        return false;
    }
    frames[head] = *frame;
    head = (head + 1) % RING_BUFFER_SIZE;
    count++;
    return true;
}

bool RingBuffer::pop(SampleFrame *frame) {
    if (count == 0) {
        return false;
    }
    *frame = frames[tail];
    tail = (tail + 1) % RING_BUFFER_SIZE;
    count--;
    return true;
}

uint32_t RingBuffer::get_overrun_count() const {
    return overrun_count;
}

void RingBuffer::reset_overrun_count() {
    overrun_count = 0;
}