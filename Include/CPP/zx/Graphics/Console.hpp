// Console.hpp
// Written by TechEverything

/*
Header for Console.cpp
*/

#pragma once

#include "zx/Multiboot2/Tags.hpp"

#include "zx/Graphics/Bitmap.hpp"
#include "zx/Graphics/Color.hpp"
#include "zx/Graphics/Point.hpp"

#include "zx/Assets/Fonts/BitFont.hpp"

namespace Graphics {
    namespace Console {
        void PutCharacter(Point pos, Color col, char character);

        constexpr u32 desired_distancing_x = 9;
        constexpr u32 desired_distancing_y = 10;
        extern u32 x;
        extern u32 y;

        void OutputString(Color col, const char* string);

        namespace Status {
            enum class Type {
                Ok,
                Warn,
                Info,
                Failed
            };

            void Output(Type type, const char* string);
        }
    }
}