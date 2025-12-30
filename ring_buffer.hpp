#pragma once
#include "event.hpp"
#include <atomic>
#include <cstddef>

constexpr size_t BUFFER_SIZE = 1024;

class RingBuffer {
public:
    bool push(const char* data, size_t len);
    bool pop(Event& out);

private:
    Event buffer[BUFFER_SIZE];

    alignas(64) std::atomic<uint64_t> claim_seq{0};
char pad1[64];

alignas(64) std::atomic<uint64_t> write_seq{0};
char pad2[64];

alignas(64) std::atomic<uint64_t> read_seq{0};


};
