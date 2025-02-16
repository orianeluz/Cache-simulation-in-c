# Cache Simulation in C

This repository contains a C program that simulates the behavior of a cache using the Least Frequently Used (LFU) cache replacement policy. The program models the cache operations, including reading and writing bytes, simulating how data is stored and replaced in the cache.

## Features

- **Cache Structure**: The cache is represented by sets, with each set containing multiple cache lines. Each cache line holds a block of data, a frequency counter, and a tag.
- **LFU Cache Replacement**: When a cache set is full, the line with the least frequency of access is replaced. If two lines have the same frequency, the first encountered line is replaced.
- **Write-Through Cache**: Writes to the cache are also written to the main memory.

## Functions

- `initialize_cache`: Initializes the cache based on user-defined parameters.
- `read_byte`: Simulates reading a byte from the cache, updates cache lines based on the LFU policy, and returns the byte.
- `write_byte`: Simulates writing a byte to the cache and memory.
- `print_cache`: Prints the current state of the cache.

## How It Works

The program simulates a simple cache system with the following steps:

1. **Input**:
   - The program asks for the size of the data (simulated RAM) and the actual data values.
   - The user inputs cache parameters: `s` (number of sets), `t` (tag length), `b` (block size), and `E` (number of lines per set).
   
2. **Cache Operations**:
   - The program simulates memory accesses by calling `read_byte` and `write_byte` functions.
   - Cache lines are updated according to the LFU replacement policy.

3. **Output**:
   - The final cache state is printed, showing the validity, frequency, tag, and data blocks of each cache line.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/orianeluz/Cache-simulation-in-c.git
   
2.Compile the code:
   ```bash
     gcc cache_simulation.c -o cache_simulation

