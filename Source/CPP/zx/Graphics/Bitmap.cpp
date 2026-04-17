// Bitmap.cpp
// Written by TechEverything

/*
Bitmap (1 byte = 8 pixels)
*/

#include "zx/Graphics/Color.hpp"
#include "zx/Graphics/Point.hpp"
#include "zx/Graphics/Basic.hpp"

namespace Graphics {
    namespace Bitmap {
        void Draw(Point pos, Color color, size bitmap_size_y, bool lsb_first, u8* bitmap) {
            for (size row = 0; row < bitmap_size_y; row++) {
                u8 line = bitmap[row];
                for (size col = 0; col < 8; col++) {
                    if (lsb_first) {
                        if (line & (1 << col)) {
                            Basic::DrawPixel({pos.x + col, pos.y + row}, color);
                        }
                    } else {
                        if (line & (1 << (7 - col))) {
                            Basic::DrawPixel({pos.x + col, pos.y + row}, color);
                        }
                    }

                }
            }
        }

        void DrawScaled(Point pos, Color color, size bitmap_size_y, bool lsb_first, u8 scale, u8* bitmap) {
            for (size row = 0; row < bitmap_size_y; row++) {
                u8 line = bitmap[row];
                for (size col = 0; col < 8; col++) {
                    if (lsb_first) {
                        if (line & (1 << col)) {
                            Basic::DrawRect({pos.x + (scale * col), pos.y + (scale * row)}, {scale, scale}, color);
                        }
                    } else {
                        if (line & (1 << (7 - col))) {
                            Basic::DrawRect({pos.x + (scale * col), pos.y + (scale * row)}, {scale, scale}, color);
                        }
                    }

                }
            }
        }
    }
}