// Bytemap.hpp
// Written by TechEverything

/*
Header for Bytemap.cpp
*/

#pragma once

#include "zx/Graphics/Color.hpp"
#include "zx/Graphics/Point.hpp"
#include "zx/Graphics/Basic.hpp"

#include "Integers.hpp"

namespace Graphics {
    namespace Bytemap {
        void Draw(Point pos, Color color, Point _size, u8* bytemap);
        void DrawScaled(Point pos, Color color, Point _size, u8 scale, u8* bytemap) ;
    }
}