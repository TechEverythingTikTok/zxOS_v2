// Output.hpp
// Written by TechEverything

/*
Header for Output.cpp
*/

#include "zx/VGA/Info.hpp"
#include "Integers.hpp"

#pragma once

namespace VGA {
    void Output(const char* src, u8 color);

    extern const bool vga_draw;
    extern bool in_panic;

    namespace Complex {
        enum class Status {
            Ok,
            Warn,
            Failed,
            Info
        };

        void OutputStatusMessage(
            Status status, const char* message
        );

        void ClearScreen(u8 color);
    }
}