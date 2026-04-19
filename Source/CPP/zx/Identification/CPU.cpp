// CPU.cpp
// Written by TechEverything

/*
CPU Identification
*/

#include <cpuid.h>

#include "Integers.hpp"

#include "zx/Identification/CPU.hpp"

#include "zx/Memory/Heap.hpp"

namespace Identification {
    namespace CPU {
        Memory::MemoryBlock GetID(Request req) {
            Memory::MemoryBlock block = Memory::Allocate(16);

            u32 eax, ebx, ecx, edx;

            if (__get_cpuid(0, &eax, &ebx, &ecx, &edx)) {
                Memory::MemoryBlock block = Memory::Allocate(13);
                char* str = (char*)block.ptr;
            
                ((u32*)str)[0] = ebx;
                ((u32*)str)[1] = edx;
                ((u32*)str)[2] = ecx;
            
                str[12] = '\0';
                return block;
            } else {
                Memory::Free(block);
                return {nullptr, 0, 0};
            }
        }

        Memory::MemoryBlock GetBrandString() {
            u32 eax, ebx, ecx, edx;

            // Check if extended functions are supported
            if (!__get_cpuid(0x80000000, &eax, &ebx, &ecx, &edx)) {
                return {nullptr, 0, 0};
            }
        
            if (eax < 0x80000004) {
                return {nullptr, 0, 0};
            }
        
            Memory::MemoryBlock block = Memory::Allocate(49); // 48 + null terminator
            char* str = (char*)block.ptr;
        
            u32* ptr = (u32*)str;
        
            __get_cpuid(0x80000002, &ptr[0], &ptr[1], &ptr[2], &ptr[3]);
            __get_cpuid(0x80000003, &ptr[4], &ptr[5], &ptr[6], &ptr[7]);
            __get_cpuid(0x80000004, &ptr[8], &ptr[9], &ptr[10], &ptr[11]);
        
            str[48] = '\0';
        
            return block;
        }
    }
}