// Basic.hpp
// Written by TechEverything

/*
Header for Basic.cpp
*/

#pragma once

#include "zx/Graphics/Point.hpp"
#include "zx/Graphics/Color.hpp"

namespace Graphics {
    namespace Basic {
        struct Snapshot {
            Point size;
            u8* region;
        };
        void DrawPixel(Point pos, Color color);
        void DrawRect(Point pos, Point _size, Color color);
        void ClearScreen(Color color);
        void ScrollScreen(u32 height);
    }
}