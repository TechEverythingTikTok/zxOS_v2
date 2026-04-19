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

    struct PanicMessage {
        const char* title;
        const char* description;
    };

    constexpr PanicMessage exceptions[] = {
        // 0
        {
            "Divide Error (#DE)",
            "Division by zero or overflow.\nCheck divisors and integer bounds."
        },
    
        // 1
        {
            "Debug Exception (#DB)",
            "Unexpected debug trap.\nDid you leave a breakpoint or enable single-step?"
        },
    
        // 2
        {
            "Non Maskable Interrupt (NMI)",
            "Hardware reported a critical condition.\nPossible hardware failure or watchdog trigger."
        },
    
        // 3
        {
            "Breakpoint (#BP)",
            "Breakpoint hit.\nINT3 instruction executed."
        },
    
        // 4
        {
            "Overflow (#OF)",
            "INTO instruction triggered overflow.\nCheck arithmetic bounds."
        },
    
        // 5
        {
            "BOUND Range Exceeded (#BR)",
            "Array bounds check failed.\nIndex out of valid range."
        },
    
        // 6
        {
            "Invalid Opcode (#UD)",
            "CPU tried to execute an invalid instruction.\nPossible bad function pointer or corrupted code."
        },
    
        // 7
        {
            "Device Not Available (#NM)",
            "FPU not available.\nDid you forget to initialize or switch FPU state?"
        },
    
        // 8
        {
            "Double Fault (#DF)",
            "Exception occurred while handling another exception.\nSystem state is corrupted."
        },
    
        // 9
        {
            "Coprocessor Segment Overrun",
            "Obsolete exception.\nShould never happen on modern CPUs."
        },
    
        // 10
        {
            "Invalid TSS (#TS)",
            "Task State Segment is invalid.\nCheck GDT/TSS configuration."
        },
    
        // 11
        {
            "Segment Not Present (#NP)",
            "Tried to load a missing segment.\nCheck GDT entries."
        },
    
        // 12
        {
            "Stack Fault (#SS)",
            "Stack access failed.\nStack overflow or invalid SS."
        },
    
        // 13
        {
            "General Protection Fault (#GP)",
            "Protection violation.\nBad pointer, invalid segment, or privilege issue."
        },
    
        // 14
        {
            "Page Fault (#PF)",
            "Invalid memory access.\nCheck pointer validity and paging structures."
        },
    
        // 15
        {
            "Reserved Exception",
            "CPU reserved exception triggered.\nThis should not happen."
        },
    
        // 16
        {
            "Floating Point Error (#MF)",
            "x87 FPU error.\nInvalid floating-point operation."
        },
    
        // 17
        {
            "Alignment Check (#AC)",
            "Unaligned memory access.\nCheck structure packing or pointer alignment."
        },
    
        // 18
        {
            "Machine Check (#MC)",
            "Hardware failure detected.\nCheck CPU/RAM stability."
        },
    
        // 19
        {
            "SIMD Floating Point Exception (#XM)",
            "SSE floating-point error.\nInvalid SIMD operation."
        },
    
        // 20
        {
            "Virtualization Exception (#VE)",
            "EPT or virtualization violation.\nOnly relevant if using virtualization."
        },
    
        // 21
        {
            "Control Protection Exception (#CP)",
            "Control-flow violation (CET).\nCorrupted return address or missing ENDBRANCH."
        }
    };

    void __attribute__((noreturn)) Interrupt(
        u32 int_no,
        u32 err_code
    ) {
        const char* title = exceptions[int_no].title;
        const char* description = exceptions[int_no].description;

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