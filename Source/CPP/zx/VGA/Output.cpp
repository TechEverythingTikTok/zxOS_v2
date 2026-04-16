// Output.cpp
// Written by TechEverything

/*
0xB8000 VGA output
*/

#include "Integers.hpp"

#include "zx/VGA/Info.hpp"
#include "zx/VGA/Output.hpp"
#include "zx/VGA/Color.hpp"

#include "zx/Memory/Utilities.hpp"

#include "zx/Ports/IO.hpp"

#include "zx/Panic.hpp"

#include "zx/Multiboot2/Tags.hpp"

namespace VGA {

    const bool vga_draw = true;
    bool in_panic = false;

    volatile char* Video = (volatile char*)(VGA::Address);
    u32 x = 0;
    u32 y = 0;

    void PanicIfDrawNotAllowed() {
        if (in_panic == true) return;
        if (vga_draw == false) {
            in_panic = true;
            Panic::Common(
                "VGA Output is deprecated",
                "VGA::___ is deprecated. Please replace it with Graphics::___"
            );
        }
    }

    void UpdateCursor(u8 _x, u8 _y) {
        u16 pos = _y * VGA::Width + _x;

        IO::Output::Byte(0x3D4, 0x0F);
        IO::Output::Byte(0x3D5, (u8)(pos & 0xFF));
        IO::Output::Byte(0x3D4, 0x0E);
        IO::Output::Byte(0x3D5, (u8)((pos >> 8) & 0xFF));
    }

    void Scroll() {
        char* addr_target = (char*)VGA::Address;
        char* addr_start = (char*)(VGA::Address + (2 * (1 * VGA::Width + 0)));

        Memory::Move(addr_target, addr_start, 
            160 * (VGA::Height - 1));
            
        for (size x = 0; x < VGA::Width; x++) {
            Video[2 * (24 * VGA::Width + x)] = ' ';
            Video[2 * (24 * VGA::Width + x) + 1] = 0x0F;
        }
    }

    void Output(const char* src, u8 color) {
        PanicIfDrawNotAllowed();
        while (*src) {
            if (*src == '\n') {
                y++;
                x = 0;
                src++;

                if (y >= VGA::Height) {
                    Scroll();
                    y = VGA::Height - 1;
                }
                continue;
            }

            Video[2 * (y * VGA::Width + x)] = *src;
            Video[2 * (y * VGA::Width + x) + 1] = color;

            x++;
            if (x >= VGA::Width) {
                x = 0;
                y++;
                if (y >= VGA::Height) {
                    Scroll();
                    y = VGA::Height - 1;
                }
            }

            src++;
        }
        UpdateCursor(x, y);
    }

    namespace Complex {
        void OutputStatusMessage(
            Status status, const char* message
        ) {
            PanicIfDrawNotAllowed();
            switch (status) {
                case Status::Ok:
                    Output("[", Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    Output("  OK  ", Color::ProcessColor(Color::Colors::Green, Color::Colors::Black));
                    Output("] ", Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    Output(message, Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    break;
                case Status::Warn:
                    Output("[", Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    Output(" WARN ", Color::ProcessColor(Color::Colors::Yellow, Color::Colors::Black));
                    Output("] ", Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    Output(message, Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    break;
                case Status::Failed:
                    Output("[", Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    Output("FAILED", Color::ProcessColor(Color::Colors::LightRed, Color::Colors::Black));
                    Output("] ", Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    Output(message, Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    break;
                case Status::Info:
                    Output("[", Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    Output(" INFO ", Color::ProcessColor(Color::Colors::Gray, Color::Colors::Black));
                    Output("] ", Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    Output(message, Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    break;
                default:
                    Output("[", Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    Output(" UNKN ", Color::ProcessColor(Color::Colors::Gray, Color::Colors::Black));
                    Output("] ", Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    Output(message, Color::ProcessColor(Color::Colors::White, Color::Colors::Black));
                    break;
            }
        }

        void ClearScreen(u8 color) {
            for (size _y = 0; _y < VGA::Height; _y++) {
                for (size _x = 0; _x < VGA::Width; _x++) {
                    Video[2 * (_y * VGA::Width + _x)] = ' ';
                    Video[2 * (_y * VGA::Width + _x) + 1] = color;
                }
            }
            y = 0;
            x = 0;
            UpdateCursor(x, y);
        }
    }
}