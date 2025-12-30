#include "ring_buffer.hpp"
#include <thread>
#include <iostream>
#include <chrono>
#include <cstdio>
void tcp_server();

RingBuffer rb;

void producer(int id) {
    char msg[256];
    std::snprintf(msg, sizeof(msg), "producer-%d", id);

    for (int i = 0; i < 500'000; ++i) {
        while (!rb.push(msg, sizeof(msg))) {}
    }
}

void consumer() {
    Event e;
    uint64_t expected = 0;

    while (expected < 1'000'000) {
        if (rb.pop(e)) {
            if (e.seq != expected) {
                std::cerr << "SEQUENCE ERROR\n";
                std::exit(1);
            }
            expected++;
        }
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::thread p1(producer, 1);
    std::thread p2(producer, 2);
    std::thread net(tcp_server);
    std::thread c(consumer);

    p1.join();
    p2.join();
    net.join(); // runs forever
    c.join();

    auto end = std::chrono::high_resolution_clock::now();
    double sec = std::chrono::duration<double>(end - start).count();

    std::cout << "Time: " << sec << " sec\n";
    std::cout << "Throughput: "
              << (1'000'000 / sec)
              << " messages/sec\n";
}

