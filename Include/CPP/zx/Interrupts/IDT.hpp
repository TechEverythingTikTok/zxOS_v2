// IDT.hpp
// Written by TechEverything

/*
Header for IDT.cpp
*/

#pragma once

#include "Integers.hpp"

namespace IDT {
    struct __attribute__((packed)) IDTR {
        u16 limit;
        u32 base;
    };
    void DefaultInitialize();
}