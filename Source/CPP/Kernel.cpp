// Kernel.cpp
// Written by TechEverything

/*
Handles initialization, connects all other code together
*/

#include "Integers.hpp"

#include "zx/Multiboot2/Magic.hpp"
#include "zx/Multiboot2/Tags.hpp"

#include "zx/VGA/Color.hpp"
#include "zx/VGA/Output.hpp"

#include "zx/Permissions/GDT.hpp"

#include "zx/Interrupts/IDT.hpp"
#include "zx/Interrupts/PIC.hpp"

#include "zx/Panic.hpp"
#include "zx/Debug.hpp"

#include "zx/Memory/Heap.hpp"
#include "zx/Memory/String.hpp"

#include "zx/Drivers/Keyboard.hpp"

#include "zx/Graphics/Basic.hpp"
#include "zx/Graphics/Bitmap.hpp"
#include "zx/Graphics/Console.hpp"

#include "zx/Assets/Fonts/BitFont.hpp"

extern "C" void __attribute__((stdcall)) KernelMain(
    uptr stack_size,
    uptr multiboot2_ident,
    uptr multiboot2_address
) {
    // VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Info, "Initializing system\n");
    Debug::Output("Initializing system\n");

    static char _stack_size[12];
    String::Convert::IntegerToASCII(_stack_size, stack_size);

    //VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Info, "Stack size: ");
    //VGA::Output(buf, VGA::Color::ProcessColor(VGA::Color::Colors::LightCyan, VGA::Color::Colors::Black));
    //VGA::Output("B\n", 0x07);

    if (multiboot2_ident != Multiboot2::Magic) {
        // invalid multiboot2 identifier
        Panic::Common("Invalid Multiboot2 identifier", "The Multiboot2 identifier is invalid");
        return;
    }

    //VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Info, "Multiboot2 identifier is valid\n");

    if (multiboot2_address & 7) {
        Panic::Common("Unaligned Multiboot2 address", "The Multiboot2 address is not aligned properly");
        return;
    }

    //VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Info, "Multiboot2 address is aligned properly\n");

    multiboot2_address += 8;

    Multiboot2::Tags::Tag* tag = (Multiboot2::Tags::Tag*)(multiboot2_address);

    for (
        ; 
        tag->type != Multiboot2::Tags::Types::End;
        tag = (Multiboot2::Tags::Tag*)((u8*)tag + ((tag->size + 7) & ~7))
    ) {
        Multiboot2::Tags::Parse(tag);
    }  

    Graphics::Console::Status::Output(Graphics::Console::Status::Type::Info, "Initializing system\n");

    Graphics::Console::Status::Output(Graphics::Console::Status::Type::Info, "Stack size: ");
    Graphics::Console::OutputString({0, 170, 170}, _stack_size);
    Graphics::Console::OutputString({0, 0, 0}, "B\n");

    GDT::DefaultInitialize();

    //VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Ok, "Initialized GDT\n");
    Graphics::Console::Status::Output(Graphics::Console::Status::Type::Ok, "Initialized GDT\n");

    IDT::DefaultInitialize();

    //VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Ok, "Initialized IDT\n");
    Graphics::Console::Status::Output(Graphics::Console::Status::Type::Ok, "Initialized IDT\n");

    PIC::Remap(0x20, 0x28);

    //VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Ok, "Remapped PIC\n");
    Graphics::Console::Status::Output(Graphics::Console::Status::Type::Ok, "Remapped PIC\n");
    
    for (size irq = 0; irq < 16; irq++) {
        PIC::IRQ::SetMask(irq);
    }

    PIC::IRQ::ClearMask(2);

    static char _heap_size[12];
    u32 target = Memory::BlockAmount * 4;
    String::Convert::IntegerToASCII(_heap_size, target);
    //VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Info, "Heap size: ");
    //VGA::Output(buf2, VGA::Color::ProcessColor(VGA::Color::Colors::LightCyan, VGA::Color::Colors::Black));
    //VGA::Output("B\n", 0x07);
    Graphics::Console::Status::Output(Graphics::Console::Status::Type::Info, "Heap size: ");
    Graphics::Console::OutputString({0, 170, 170}, _heap_size);
    Graphics::Console::OutputString({0, 0, 0}, "B\n");

    Keyboard::Initialize();
    //VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Ok, "Initialized keyboard\n");
    Graphics::Console::Status::Output(Graphics::Console::Status::Type::Ok, "Initialized keyboard\n");

    asm volatile("sti");

    //VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Warn, "Enabled interrupts\n");
    Graphics::Console::Status::Output(Graphics::Console::Status::Type::Ok, "Enabled interrupts\n");

    Graphics::Console::OutputString({255, 255, 255}, "PRESS ANY KEY TO CONTINUE\n");

    while (!Keyboard::first_press) {
        asm volatile("hlt");
    }

    Graphics::Basic::ClearScreen({0, 0, 0});

    for (;;) {
        asm volatile("hlt");
    }
}