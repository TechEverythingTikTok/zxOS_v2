// Heap.cpp
// Written by TechEverything

/*
Handles heap memory allocation
*/

#pragma once

#include "Integers.hpp"
#include "Functions.hpp"

#include "Panic.hpp"

namespace Memory {
    struct MemoryBlock {
        void* ptr;
        size amount;
        size start;
    };

    function return_type(MemoryBlock) Allocate(size n);
    function return_type(void) Free(MemoryBlock block);
    function return_type(void) Defragment();
}