// Utilities.cpp
// Written by TechEverything

/*
Memory utilities such as:
Memory::Move
Memory::Copy
*/

#include "Integers.hpp"

namespace Memory {
    void Move(
        void* dest,
        const void* src,
        size n
    ) {
        char* cdest = (char*)(dest);
        char* csrc = (char*)(src);

        if (dest <= src) {
            while (n--) {
                *cdest = *csrc;
                cdest++;
                csrc++;
            }
        } else {
            cdest += n - 1;
            csrc += n - 1;
            while (n--) {
                *cdest = *csrc;
                cdest--;
                csrc--;
            }
        }
    }

    void Copy(
        void* dest,
        void* src,
        size n
    ) {
        char* cdest = (char*)dest;
        char* csrc = (char*)src;

        while (n--) {
            *cdest = *csrc;
            cdest++;
            csrc++;
        }
    }

    void Set(
        void* dest,
        u32 value,
        size n
    ) {
        char* cdest = (char*)dest;

        while (n--) {
            *cdest = value;
            cdest++;
        }
    }
}