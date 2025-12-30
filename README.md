# Lock-Free Event Broker (C++)

## Overview
This project implements a **high-performance, in-memory event broker** in C++
using **lock-free ring buffers**. It is designed to demonstrate how
**low-latency systems** handle concurrent message passing with predictable
performance.

The goal of this project is **systems-level understanding**, not to build a
full-featured distributed messaging platform.

---

## Key Features
- Lock-free **SPSC** (Single Producer, Single Consumer) ring buffer
- Lock-free **MPSC** (Multiple Producers, Single Consumer) ring buffer
- Fixed-size, pre-allocated memory (no runtime heap allocation)
- Busy-spin synchronization for low latency
- Real **TCP ingestion layer** (Windows)
- Benchmarked and performance-analyzed

---

## Architecture


Core components:
- Circular buffer with sequence numbers
- Atomic slot claiming for producers
- Ordered publishing to preserve correctness
- Single consumer for deterministic ordering

---

## Design Decisions

### Fixed-Size Ring Buffer
All memory is allocated at startup. This avoids allocator overhead, reduces
latency jitter, and improves cache locality.

### Lock-Free Synchronization
The system relies on `std::atomic` instead of mutexes:
- No thread blocking
- No kernel context switches
- Predictable latency behavior

### Busy-Spin Waiting
Threads spin instead of sleeping:
- Trades CPU usage for lower wake-up latency
- Common in low-latency systems such as trading engines

---

## Implemented Modes

### SPSC (Single Producer, Single Consumer)
- One producer thread
- One consumer thread
- Minimal synchronization
- Highest throughput

### MPSC (Multiple Producers, Single Consumer)
- Multiple producer threads
- Atomic slot claiming
- Ordered publishing
- Higher contention, lower throughput (expected)

---

## Benchmarks

Benchmarks were run locally on a Windows laptop using MSYS2 (UCRT64).

| Mode | Producers | Consumers | Throughput |
|------|----------|-----------|------------|
| SPSC | 1 | 1 | ~30 million messages/sec |
| MPSC | 2 | 1 | ~10 million messages/sec |

> Throughput decreases as producer count increases due to contention on shared
> atomic counters. This behavior is expected in MPSC designs.

---

## Performance Analysis
- SPSC benefits from minimal contention and achieves very high throughput.
- MPSC introduces contention due to atomic slot claiming and ordered publishing.
- Lock-free does **not** mean contention-free.
- Latency remains predictable even as throughput decreases.

---

## TCP Ingestion
A lightweight TCP server runs in a dedicated thread:
- Listens on port `9090`
- Accepts external client connections
- Pushes incoming data directly into the ring buffer

This validates integration of the lock-free core with real network I/O.

---

## Trade-offs & Limitations
- In-memory only (no persistence)
- Single-node design
- No fault tolerance
- Optimized for latency, not durability or distribution

This project is **not** intended to replace systems like Kafka or Redis.

---

## Build & Run

### Requirements
- C++20 compatible compiler
- CMake ≥ 3.16
- MSYS2 (UCRT64) on Windows

### Build
```bash
mkdir build
cd build
cmake ..
cmake --build .
