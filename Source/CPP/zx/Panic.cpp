// Panic.cpp
// Written by TechEverything

/*
Handles kernel panic
*/

#include "Integers.hpp"

#include "zx/VGA/Color.hpp"
#include "zx/VGA/Output.hpp"

#include "zx/Memory/String.hpp"

#include "zx/Debug.hpp"

#include "zx/Graphics/Console.hpp"

namespace Panic {
    void __attribute__((noreturn)) Common(
        const char* title,
        const char* description
    ) {
        Debug::Output("Kernel Panic\nTitle: ");
        Debug::Output(title);
        Debug::Output("\nDescription: ");
        Debug::Output(description);
        Debug::Output("\n");

        i32 eip;

        asm volatile (
            "call 1f\n"
            "1: pop %0\n"
            : "=r"(eip)
        );

        i32 esp;

        asm volatile (
            "mov %%esp, %0"
            : "=r"(esp)
        );

        if (VGA::vga_draw == true) {
            VGA::Complex::ClearScreen(0);
            static char src[80] = {0};

            for (size i = 0; i < 34; i++) {
                src[i] = ' ';
            }

            src[34] = 'K';
            src[35] = 'e';
            src[36] = 'r';
            src[37] = 'n';
            src[38] = 'e';
            src[39] = 'l';
            src[40] = ' ';
            src[41] = 'P';
            src[42] = 'a';
            src[43] = 'n';
            src[44] = 'i';
            src[45] = 'c';

            VGA::Output(src, VGA::Color::ProcessColor(VGA::Color::Colors::Red, VGA::Color::Colors::Black));
            VGA::Output("\n\n", 0);
            VGA::Output(title, VGA::Color::ProcessColor(VGA::Color::Colors::White, VGA::Color::Colors::Black));
            VGA::Output("\n", 0);
            VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Failed, description);
            VGA::Output("\n", 0);
            VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Info, "This is a kernel panic designed to prevent any damage\nFurther information is below:\n");

            static char buf[8];
            String::Convert::IntegerToASCII(buf, eip);
            VGA::Output("Instruction Pointer (base 10): ", VGA::Color::ProcessColor(VGA::Color::Colors::Gray, VGA::Color::Colors::Black));
            VGA::Output(buf, VGA::Color::ProcessColor(VGA::Color::Colors::LightCyan, VGA::Color::Colors::Black));
            VGA::Output("B\n", 0x07);

            static char buf2[8];
            String::Convert::IntegerToASCII(buf2, esp);
            VGA::Output("Stack Pointer (base 10): ", VGA::Color::ProcessColor(VGA::Color::Colors::Gray, VGA::Color::Colors::Black));
            VGA::Output(buf2, VGA::Color::ProcessColor(VGA::Color::Colors::LightCyan, VGA::Color::Colors::Black));
            VGA::Output("B\n", 0x07);

            VGA::Output("Kernel Code Selector: 0x08\n", VGA::Color::ProcessColor(VGA::Color::Colors::Gray, VGA::Color::Colors::Black));
            VGA::Output("Kernel Data Selector: 0x10\n", VGA::Color::ProcessColor(VGA::Color::Colors::Gray, VGA::Color::Colors::Black));

            VGA::Output("e-mail: itsaxgammer@gmail.com\n", VGA::Color::ProcessColor(VGA::Color::Colors::White, VGA::Color::Colors::Black));

            VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Info, "You can safely reboot now...\n");
        } else {
            Graphics::Console::ClearScreen({0, 0, 0});

            Graphics::Console::OutputString({180, 0, 0}, "Kernel panic\n\n");
            Graphics::Console::OutputString({255, 255, 255}, title);
            Graphics::Console::OutputString({0, 0, 0}, "\n");
            Graphics::Console::Status::Output(Graphics::Console::Status::Type::Failed, description);
            Graphics::Console::OutputString({0, 0, 0}, "\n");
            Graphics::Console::Status::Output(Graphics::Console::Status::Type::Info, "This is a kernel panic designed to prevent any damage\nFurther information is below:\n");

            static char buf[8];
            String::Convert::IntegerToASCII(buf, eip);
            Graphics::Console::OutputString({120, 120, 120}, "Instruction Pointer (base 10): ");
            Graphics::Console::OutputString({0, 170, 170}, buf);
            Graphics::Console::OutputString({120, 120, 120}, "B\n");

            static char buf2[8];
            String::Convert::IntegerToASCII(buf2, esp);
            Graphics::Console::OutputString({120, 120, 120}, "Stack Pointer (base 10): ");
            Graphics::Console::OutputString({0, 170, 170}, buf2);
            Graphics::Console::OutputString({120, 120, 120}, "B\n");

            Graphics::Console::OutputString({120, 120, 120}, "Kernel Code Selector: 0x08\n");
            Graphics::Console::OutputString({120, 120, 120}, "Kernel Data Selector: 0x10\n");

            Graphics::Console::OutputString({255, 255, 255}, "e-mail: itsaxgammer@gmail.com\n");

            Graphics::Console::Status::Output(Graphics::Console::Status::Type::Info, "You can safely reboot now...\n");
        }

        __asm__ volatile("cli; hlt");
        __builtin_unreachable();
    }
}