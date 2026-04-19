// Panic.cpp
// Written by TechEverything

/*
Header for Panic.cpp
*/

#pragma once

#include "Integers.hpp"

#include "zx/VGA/Color.hpp"
#include "zx/VGA/Output.hpp"

namespace Panic {
    void __attribute__((noreturn)) Common(
        const char* title,
        const char* description
    );

    void __attribute__((noreturn)) Interrupt(
        u32 int_no,
        u32 err_code
    );
}