// Heap.cpp
// Written by TechEverything

/*
Handles heap memory allocation
*/

#include "Integers.hpp"
#include "Functions.hpp"
#include "Panic.hpp"

#include "zx/Memory/Heap.hpp"

u32 Space[256] = {0}; // 256 * 4 = 1KB
bool Used[256] = {false};

namespace Memory {
    function return_type(MemoryBlock) Allocate(size n) {
        // get 4-byte aligned 
        size amount = (n + 3) / 4;

        for (size i = 0; i < 256 - amount; i++) {
            bool can_use = true;
            for (size j = 0; j < amount; j++) {
                if (Used[j + i] == true) {
                    can_use = false;
                    break;
                }
            }

            if (can_use) {
                for (size j = 0; j < amount; j++) {
                    Used[i + j] = true;
                }
                MemoryBlock block;
                block.ptr = &Space[i];
                block.amount = amount;
                block.start = i;
                return block;
            }
        }

        Panic::Common("Out of memory", "The system has run out of memory.\nThe heap allocator can not find any free memory to write to.");
        MemoryBlock block;
        block.ptr = nullptr;
        block.amount = amount;
        block.start = 0;
        return block;
    }

    function return_type(void) Free(MemoryBlock block) {
        if (block.ptr == nullptr) return;
        if (block.start + block.amount > 256) {
            Panic::Common("Heap corruption", "Invalid free detected.");
            return;
        }
        for (size i = 0; i < block.amount; i++) {
            Used[block.start + i] = false;
        }
    }

    function return_type(void) Defragment() {
        // just mark everything clear lol
        for (size i = 0; i < 256; i++) {
            Used[i] = false;
        }
    }
}