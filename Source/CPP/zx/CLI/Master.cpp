// Master.cpp
// Written by TechEverything

/*
Core CLI Input 
*/

#include "Integers.hpp"

#include "zx/Drivers/Keyboard.hpp"

#include "zx/Graphics/Console.hpp"

#include "zx/Memory/String.hpp"

#include "zx/Multiboot2/Tags.hpp"

namespace CLI {
    static char buf[512] = {0};
    size access = 0;

    void Initialize() {
        Graphics::Console::OutputString({255, 255, 255}, "> ");
        access = 0;
        for (size i = 0; i < 512; i++) {
            buf[i] = 0;
        }
    }

    void ProcessCommand() {
        if (buf[0] == '\0') return;
        Graphics::Console::OutputString({0, 0, 0}, "\n");
        if (String::Equals(buf, "help")) {
            Graphics::Console::OutputString(
                {255, 255, 255}, 
                "zxOS v2 (32-bit)\n"
                "help - display Help\n"
                "clear - clear screen"
            );
        } else if (String::Equals(buf, "clear")) {
            Graphics::Console::ClearScreen({0, 0, 0});
            return;
        } else {
            Graphics::Console::OutputString({170, 0, 0}, "Unknown command. Type 'help' for assistance.");
        }
        Graphics::Console::OutputString({0, 0, 0}, "\n");
    }

    void HandleInput(Keyboard::Event event) {
        if (event.release == true) return;
        if (event.type == Keyboard::EventType::Normal) {
            buf[access] = event.ident;
            static char acc[2];
            acc[0] = event.ident;
            acc[1] = '\0';
            Graphics::Console::OutputString({255, 255, 255}, acc);
            access++;
        } else if (event.type == Keyboard::EventType::Special) {
            if (event.ident == Keyboard::SpecialKeyCode::Backspace) {
                if (access <= 0) return;
                if (Graphics::Console::x < Graphics::Console::desired_distancing_x + 8) {
                    Graphics::Console::y -= Graphics::Console::desired_distancing_y;
                    Graphics::Console::x = Multiboot2::Container::framebuffer->framebuffer_width - Graphics::Console::desired_distancing_x - 8;

                    Graphics::Console::PutCharacter({Graphics::Console::x, Graphics::Console::y}, {0, 0, 0}, 31);
                    
                } else {
                    Graphics::Console::x -= Graphics::Console::desired_distancing_x;

                    Graphics::Console::PutCharacter({Graphics::Console::x, Graphics::Console::y}, {0, 0, 0}, 31);
                }
                buf[access] = 0;
                access--;
            } else if (event.ident == Keyboard::SpecialKeyCode::Enter) {
                ProcessCommand();
                Initialize();
            }
        }
    }
}