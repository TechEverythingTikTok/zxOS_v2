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

#include "zx/Identification/CPU.hpp"

#include "zx/Interrupts/IDT.hpp"

#include "zx/Ports/IO.hpp"

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
        static char firstword[512] = {0};
        Graphics::Console::OutputString({0, 0, 0}, "\n");
        String::GetFirstWord(firstword, buf);

        if (String::Equals(firstword, "help")) {
            Graphics::Console::OutputString(
                {255, 255, 255}, 
                "zxOS v2 (32-bit)\n"
                "help - display help\n"
                "clear - clear screen\n"
                "echo <message> - output string\n"
                "sysfetch - get system information\n"
                "reboot - reboot system\n"
                "beep <frequency> - output a beep with a select frequency"
            );
        } else if (String::Equals(firstword, "clear")) {
            Graphics::Console::ClearScreen({0, 0, 0});
            return;
        } else if (String::Equals(firstword, "echo")) {
            if (access < 5) {
                Graphics::Console::OutputString({255, 255, 255}, "Usage: echo <message>");
                return;
            }
            static char msg[512] = {0};
            String::Copy(msg, buf + 5);
            Graphics::Console::OutputString({255, 255, 255}, msg);
        } else if (String::Equals(firstword, "sysfetch")) {
            Graphics::Console::OutputString(
                {255, 255, 255},
                "__________\n"
                "         /      \\       /\n"
                "        /        \\     /\n"
                "       /          \\   /\n"
                "      /            \\ /\n"
                "     /              /\n"
                "    /              / \\\n"
                "   /              /   \\\n"
                "  /              /     \\\n"
                " /              /       \\\n"
                "__________     /         \\\n"
            );
            Graphics::Console::OutputString({255, 255, 255}, "zxOS v2 (32 bit)\n");
            Graphics::Console::OutputString({255, 255, 255}, "CPU: ");
            Memory::MemoryBlock block = Identification::CPU::GetBrandString();
            Graphics::Console::OutputString({255, 255, 255}, (char*)block.ptr);
        } else if (String::Equals(firstword, "reboot")) {
            IDT::IDTR zero = {0, 0};
            asm volatile("lidt %0" : : "m"(zero));
            asm volatile("int $3");
        } else if (String::Equals(firstword, "beep")) {
            if (access < 5) {
                Graphics::Console::OutputString({255, 255, 255}, "Usage: beep <frequency>");
            }
            static char freq[24] = {0};
            String::Copy(freq, buf + 5);
            i32 freq_num = String::Convert::ASCIIToInteger(freq);
            IO::Methods::Beep(freq_num);
            for (size i = 0; i < 10000000; i++) {
                IO::Methods::Hiccup();
            }
            IO::Methods::NoBeep();
            return;
        } else {
            Graphics::Console::OutputString({170, 0, 0}, "Unknown command. Type 'help' for assistance.");
        }
        Graphics::Console::OutputString({0, 0, 0}, "\n");
    }

    void HandleInput(Keyboard::Event event) {
        if (event.release == true) return;
        if (event.type == Keyboard::EventType::Normal) {
            if (access >= 511) {
                IO::Methods::Beep(1000);
                for (size i = 0; i < 10000000; i++) {
                    IO::Methods::Hiccup();
                }
                IO::Methods::NoBeep();
                return;
            }
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