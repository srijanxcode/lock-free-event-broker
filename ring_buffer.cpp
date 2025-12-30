#include "ring_buffer.hpp"
#include <cstring>

bool RingBuffer::push(const char* data, size_t len) {
    // 1. Atomically claim a unique slot
    uint64_t seq = claim_seq.fetch_add(1, std::memory_order_relaxed);

    // 2. Wait if buffer is full (backpressure)
    while (seq - read_seq.load(std::memory_order_acquire) >= BUFFER_SIZE) {
        // spin
    }

    // 3. Write into claimed slot
    size_t idx = seq % BUFFER_SIZE;
    buffer[idx].seq = seq;
    memcpy(buffer[idx].payload, data, len);

    // 4. Publish in order (wait for previous producer)
    while (write_seq.load(std::memory_order_acquire) != seq) {
        // spin
    }

    write_seq.store(seq + 1, std::memory_order_release);
    return true;
}

bool RingBuffer::pop(Event& out) {
    uint64_t seq = read_seq.load(std::memory_order_relaxed);

    if (seq >= write_seq.load(std::memory_order_acquire))
        return false;

    size_t idx = seq % BUFFER_SIZE;
    out = buffer[idx];

    read_seq.store(seq + 1, std::memory_order_release);
    return true;
}
