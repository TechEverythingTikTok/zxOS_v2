// IO.hpp
// Written by TechEverything

/*
Port I/O for x86
*/

#pragma once

#include "Integers.hpp"

namespace IO {
    namespace Input {
        static inline u8 Byte(u16 port) {
            u8 ret;
            __asm__ volatile ( "inb %w1, %b0"
                    : "=a"(ret)
                    : "Nd"(port)
                    : "memory");
            return ret;
        }

        static inline u16 Word(u16 port) {
            u16 ret;
            __asm__ volatile ( "inw %w1, %w0"
                    : "=a"(ret)
                    : "Nd"(port)
                    : "memory");
            return ret;
        }

        static inline u32 DoubleWord(u16 port) {
            u32 ret;
            __asm__ volatile ( "inl %w1, %0"
                    : "=a"(ret)
                    : "Nd"(port)
                    : "memory");
            return ret;
        }
    }

    namespace Output {
        static inline void Byte(u16 port, u8 val) {
            __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
        }

        static inline void Word(u16 port, u16 val) {
            __asm__ volatile ( "outb %w0, %w1" : : "a"(val), "Nd"(port) : "memory");
        }

        static inline void DoubleWord(u16 port, u32 val) {
            __asm__ volatile ( "outb %0, %w1" : : "a"(val), "Nd"(port) : "memory");
        }
    }

    namespace Methods {
        static inline void Hiccup(void) {
            Output::Byte(0x80, 0);
        }

        static inline void Beep(u32 freq) {
            u32 divisor = 1193180 / freq;

            // Set PIT channel 2
            Output::Byte(0x43, 0xB6);
            Output::Byte(0x42, (u8)(divisor & 0xFF));
            Output::Byte(0x42, (u8)((divisor >> 8) & 0xFF));

            // Enable speaker
            u8 tmp = Input::Byte(0x61);
            if (tmp != (tmp | 3)) {
                Output::Byte(0x61, tmp | 3);
            }
        }

        static inline void NoBeep() {
            u8 tmp = Input::Byte(0x61) & 0xFC;
            Output::Byte(0x61, tmp);
        }
    }
}