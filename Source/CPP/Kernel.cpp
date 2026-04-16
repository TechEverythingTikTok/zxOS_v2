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

extern "C" void __attribute__((stdcall)) KernelMain(
    uptr stack_size,
    uptr multiboot2_ident,
    uptr multiboot2_address
) {
    VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Info, "Initializing system\n");

    static char buf[8];
    String::Convert::IntegerToASCII(buf, stack_size);
    VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Info, "Stack size: ");
    VGA::Output(buf, VGA::Color::ProcessColor(VGA::Color::Colors::LightCyan, VGA::Color::Colors::Black));
    VGA::Output("B\n", 0x07);

    if (multiboot2_ident != Multiboot2::Magic) {
        // invalid multiboot2 identifier
        Panic::Common("Invalid Multiboot2 identifier", "The Multiboot2 identifier is invalid");
        return;
    }

    VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Info, "Multiboot2 identifier is valid\n");

    if (multiboot2_address & 7) {
        Panic::Common("Unaligned Multiboot2 address", "The Multiboot2 address is not aligned properly");
        return;
    }

    VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Info, "Multiboot2 address is aligned properly\n");

    multiboot2_address += 8;

    Multiboot2::Tags::Tag* tag = (Multiboot2::Tags::Tag*)(multiboot2_address);

    for (
        ; 
        tag->type != Multiboot2::Tags::Types::End;
        tag = (Multiboot2::Tags::Tag*)((u8*)tag + ((tag->size + 7) & ~7))
    ) {
        Multiboot2::Tags::Parse(tag);
    }

    GDT::DefaultInitialize();

    VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Ok, "Initialized GDT\n");

    IDT::DefaultInitialize();

    VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Ok, "Initialized IDT\n");

    PIC::Remap(0x20, 0x28);

    VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Ok, "Remapped PIC\n");
    
    for (size irq = 0; irq < 16; irq++) {
        PIC::IRQ::SetMask(irq);
    }

    PIC::IRQ::ClearMask(2);

    static char buf2[8];
    u32 target = Memory::BlockAmount * 4;
    String::Convert::IntegerToASCII(buf2, target);
    VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Info, "Heap size: ");
    VGA::Output(buf2, VGA::Color::ProcessColor(VGA::Color::Colors::LightCyan, VGA::Color::Colors::Black));
    VGA::Output("B\n", 0x07);

    Keyboard::Initialize();
    VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Ok, "Initialized keyboard\n");

    asm volatile("sti");

    VGA::Complex::OutputStatusMessage(VGA::Complex::Status::Warn, "Enabled interrupts\n");

    for (;;) {
        asm volatile("hlt");
    }
}