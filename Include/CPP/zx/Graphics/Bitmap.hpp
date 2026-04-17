// Bitmap.hpp
// Written by TechEverything

/*
Header for Bitmap.cpp
*/

#pragma once

#include "zx/Graphics/Point.hpp"
#include "zx/Graphics/Color.hpp"

#include "Integers.hpp"

namespace Graphics {
    namespace Bitmap {
        void Draw(Point pos, Color color, size bitmap_size_y, bool lsb_first, u8* bitmap);
        void DrawScaled(Point pos, Color color, size bitmap_size_y, bool lsb_first, u8 scale, u8* bitmap);
    }
}