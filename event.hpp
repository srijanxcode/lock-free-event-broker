#pragma once
#include <cstdint>

struct Event {
    uint64_t seq;
    char payload[256];
};
