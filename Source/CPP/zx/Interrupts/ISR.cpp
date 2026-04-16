// ISR.cpp
// Written by TechEverything

/*
Handles Interrupt Service Routine set up
*/

#include "Integers.hpp"

#include "zx/Interrupts/PIC.hpp"

#include "zx/VGA/Output.hpp"

#include "zx/Drivers/Keyboard.hpp"

struct Registers {
    u32 gs; 
    u32 fs; 
    u32 es; 
    u32 ds;
    u32 edi;
    u32 esi;
    u32 ebp;
    u32 esp;
    u32 ebx;
    u32 edx;
    u32 ecx;
    u32 eax;
    u32 int_no;
    u32 err_code;
    u32 eflags;
    u32 cs;
    u32 eip;
};

extern "C" void ISRHandler(Registers* regs) {
    if (regs->int_no >= 32) {
        u8 irq = regs->int_no - 32;
        if (irq == 1) {
            Keyboard::EventHandle();
        }
        PIC::SendEOI(regs->int_no - 32);
    } else {

    }
    return;
}