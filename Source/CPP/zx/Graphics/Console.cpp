// Console.cpp
// Written by TechEverything

/*
Console for Framebuffer mode
*/

#include "zx/Multiboot2/Tags.hpp"

#include "zx/Graphics/Bitmap.hpp"
#include "zx/Graphics/Color.hpp"
#include "zx/Graphics/Point.hpp"
#include "zx/Graphics/Console.hpp"
#include "zx/Graphics/Basic.hpp"

#include "zx/Assets/Fonts/BitFont.hpp"

namespace Graphics {
    namespace Console {
        void PutCharacter(Point pos, Color col, char character) {
            Bitmap::Draw(pos, col, Assets::Fonts::BitFont::Height, true, Assets::Fonts::BitFont::Font[character]);
        }

        u32 x = desired_distancing_x;
        u32 y = desired_distancing_y;

        void OutputString(Color col, const char* string) {
            while (*string) {
                if (*string == '\n') {
                    x = desired_distancing_x;
                    y += desired_distancing_y;

                    if (y >= Multiboot2::Container::framebuffer->framebuffer_height) {
                        // shit
                        // ill implement this later yeahhh
                    }
                    string++;
                    continue;
                }
                PutCharacter({x, y}, col, *string);
                x += desired_distancing_x;
                if (x + desired_distancing_x >= Multiboot2::Container::framebuffer->framebuffer_width) {
                    x = desired_distancing_x;
                    y += desired_distancing_y;

                    if (y >= Multiboot2::Container::framebuffer->framebuffer_height) {

                    }
                }
                string++;
            }
        }
        
        void ClearScreen(Color color) {
            Graphics::Basic::ClearScreen(color);
            x = desired_distancing_x;
            y = desired_distancing_y;
        }

        namespace Status {
            void Output(Type type, const char* string) {
                switch (type) {
                    case Type::Ok:
                        OutputString({255, 255, 255}, "[");
                        OutputString({0, 180, 30}, "  OK  ");
                        OutputString({255, 255, 255}, "] ");
                        OutputString({255, 255, 255}, string);
                        break;
                    case Type::Info:
                        OutputString({255, 255, 255}, "[");
                        OutputString({150, 150, 150}, " INFO ");
                        OutputString({255, 255, 255}, "] ");
                        OutputString({255, 255, 255}, string);
                        break;
                    case Type::Warn:
                        OutputString({255, 255, 255}, "[");
                        OutputString({180, 180, 0}, " WARN ");
                        OutputString({255, 255, 255}, "] ");
                        OutputString({255, 255, 255}, string);
                        break;
                    case Type::Failed:
                        OutputString({255, 255, 255}, "[");
                        OutputString({180, 0, 0}, "FAILED");
                        OutputString({255, 255, 255}, "] ");
                        OutputString({255, 255, 255}, string);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}