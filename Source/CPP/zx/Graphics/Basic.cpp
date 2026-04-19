// Basic.cpp
// Written by TechEverything

/*
Basic functions (pixel drawing, rect drawing)
*/

#include "Integers.hpp"

#include "zx/Multiboot2/Tags.hpp"

#include "zx/Graphics/Color.hpp"
#include "zx/Graphics/Point.hpp"
#include "zx/Graphics/Basic.hpp"

#include "zx/Memory/Utilities.hpp"

namespace Graphics {
    namespace Basic {
        void DrawPixel(Point pos, Color color) {
            // oh no
            u8* framebuffer_addr = (u8*)(uptr)Multiboot2::Container::framebuffer->framebuffer_addr;
            u32 pitch = Multiboot2::Container::framebuffer->framebuffer_pitch;

            u32 bpp = Multiboot2::Container::framebuffer->framebuffer_bpp;
            u32 bytes_per_pixel = bpp / 8;

            u32 _pos = pos.y * pitch + pos.x * bytes_per_pixel;

            switch (bytes_per_pixel) {
                case 4: {
                    framebuffer_addr[_pos] = color.b;
                    framebuffer_addr[_pos + 1] = color.g;
                    framebuffer_addr[_pos + 2] = color.r;
                    break;
                }

                case 3: {
                    const Multiboot2::Tags::Framebuffer::Tag* fb = Multiboot2::Container::framebuffer;

                    u32 pixel = 0;

                    pixel |= (color.r >> (8 - fb->framebuffer_red_mask_size))
                        << fb->framebuffer_red_field_pos;

                    pixel |= (color.g >> (8 - fb->framebuffer_green_mask_size))
                        << fb->framebuffer_green_field_pos;

                    pixel |= (color.b >> (8 - fb->framebuffer_blue_mask_size))
                        << fb->framebuffer_blue_field_pos;

                    *(u32*)(framebuffer_addr + _pos) = pixel;
                    break;
                }

                default:
                    break;
            }
        }

        void DrawRect(Point pos, Point _size, Color color) {
            for (size y_lim = 0; y_lim < _size.y; y_lim++) {
                for (size x_lim = 0; x_lim < _size.x; x_lim++) {
                    DrawPixel({pos.x + x_lim, pos.y + y_lim}, color);
                }
            }
        }

        void ClearScreen(Color color) {
            for (size y = 0; y < Multiboot2::Container::framebuffer->framebuffer_height; y++) {
                for (size x = 0; x < Multiboot2::Container::framebuffer->framebuffer_width; x++) {
                    DrawPixel({x, y}, color);
                }
            }
        }

        void ScrollScreen(u32 height) {
            u8* framebuffer_addr = (u8*)(uptr)Multiboot2::Container::framebuffer->framebuffer_addr;
            u32 pitch = Multiboot2::Container::framebuffer->framebuffer_pitch;
            u32 _height = Multiboot2::Container::framebuffer->framebuffer_height;

            u32 bpp = Multiboot2::Container::framebuffer->framebuffer_bpp;
            u32 bytes_per_pixel = bpp / 8;

            u32 _min_pos = 0;  // x is 0 anyway so whatever
            u32 _begin_pos = height * pitch;

            u32 _amount_to_copy = pitch * (_height - height);

            Memory::Move(framebuffer_addr + _min_pos, framebuffer_addr + _begin_pos, _amount_to_copy);
            Memory::Set(framebuffer_addr + _amount_to_copy, 0, _begin_pos);
        }
    }
}