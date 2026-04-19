// CPU.hpp
// Written by TechEverything

/*
Header for CPU.cpp
*/

#include <cpuid.h>

#include "zx/Memory/Heap.hpp"

namespace Identification {
    namespace CPU {
        enum class Request {
            GetVendorString,
            GetFeatures,
            GetTLB,
            GetSerial
        };
        Memory::MemoryBlock GetID(Request req);
        Memory::MemoryBlock GetBrandString();
    }
}