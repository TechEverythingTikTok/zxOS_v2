// IDT.cpp
// Written by TechEverything

/*
Handles Interrupt Descriptor Table set up
*/

#include "Integers.hpp"

#include "zx/Interrupts/IDT.hpp"

namespace IDT {
    extern "C" u32 isr_table[];

    struct __attribute__((packed)) Entry {
        u16 offset_1;
        u16 selector;
        u8 zero;
        u8 type_attributes;
        u16 offset_2;
    };

    Entry entries[256];
    IDTR idtr;

    void SetEntry(u16 index, u32 base, u16 selector, u8 flags) {
        // zero
        entries[index].zero = 0;

        // base
        entries[index].offset_1 = base & 0xFFFF;
        entries[index].offset_2 = (base >> 16) & 0xFFFF;

        // selector
        entries[index].selector = selector;

        // flags
        entries[index].type_attributes = flags;
    }

    void DefaultInitialize() {
        for (u16 i = 0; i < 256; i++) {
            if (isr_table[i] != 0) {
                SetEntry(i, isr_table[i], 0x08, 0b10001110);
            } else {
                SetEntry(i, 0, 0, 0);   // null
            }
        }
        idtr.limit = sizeof(entries) - 1;
        idtr.base = (u32)&entries[0];

        asm volatile("lidt %0" : : "m"(idtr));
    }
}