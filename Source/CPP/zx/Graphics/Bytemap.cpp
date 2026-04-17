// Bytemap.cpp
// Written by TechEverything

/*
Bytemap (1 byte = 1 pixel)
*/

#include "zx/Graphics/Color.hpp"
#include "zx/Graphics/Point.hpp"
#include "zx/Graphics/Basic.hpp"

#include "Integers.hpp"

namespace Graphics {
    namespace Bytemap {
        void Draw(Point pos, Color color, Point _size, u8* bytemap) {
            for (size col = 0; col < _size.x; col++) {
                for (size row = 0; row < _size.y; row++) {
                    u8 pixel = bytemap[row * _size.x + col];

                    if (pixel) {
                        Basic::DrawPixel({pos.x + col, pos.y + row}, color);
                    }
                }
            }
        }

        void DrawScaled(Point pos, Color color, Point _size, u8 scale, u8* bytemap) {
            for (size row = 0; row < _size.y; row++) {
                for (size col = 0; col < _size.x; col++) {
                    u8 pixel = bytemap[row * _size.x + col];
                
                    if (pixel) {
                        Basic::DrawRect(
                            {pos.x + col * scale, pos.y + row * scale},
                            {scale, scale},
                            color
                        );
                    }
                }
            }
        }
    }
}