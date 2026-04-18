// Utilities.hpp
// Written by TechEverything

/*
Header for Utilities.cpp
*/

#pragma once

#include "Integers.hpp"

namespace Memory {
    void Move(
        void* dest,
        const void* src,
        size n
    );

    void Copy(
        void* dest,
        void* src,
        size n
    );

    void Set(
        void* dest,
        u32 value,
        size n
    );
}