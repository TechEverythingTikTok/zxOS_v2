// Keyboard.cpp
// Written by TechEverything

/*
Driver for keyboard (PS-2)
*/

#include "Integers.hpp"

#include "zx/Interrupts/PIC.hpp"

#include "zx/Drivers/Keyboard.hpp"

#include "zx/Ports/IO.hpp"

#include "zx/Panic.hpp"

#include "zx/CLI/Master.hpp"

namespace Keyboard {
    bool first_press = false;
    // oh my days c++, no designated initializers
    u8 normal_lowercase[256] = {0};
    u8 normal_uppercase[256] = {0};
    u8 keypad[256] = {0};
    u8 special[256] = {0};

    bool is_shift_pressed = false;

    void Initialize() {
        normal_lowercase[0x02] = '1'; 
        normal_lowercase[0x03] = '2'; 
        normal_lowercase[0x04] = '3'; 
        normal_lowercase[0x05] = '4'; 
        normal_lowercase[0x06] = '5'; 
        normal_lowercase[0x07] = '6'; 
        normal_lowercase[0x08] = '7'; 
        normal_lowercase[0x09] = '8'; 
        normal_lowercase[0x0A] = '9'; 
        normal_lowercase[0x0B] = '0';
        normal_lowercase[0x0C] = '-'; 
        normal_lowercase[0x0D] = '='; 
        normal_lowercase[0x10] = 'q'; 
        normal_lowercase[0x11] = 'w'; 
        normal_lowercase[0x12] = 'e'; 
        normal_lowercase[0x13] = 'r'; 
        normal_lowercase[0x14] = 't'; 
        normal_lowercase[0x15] = 'y'; 
        normal_lowercase[0x16] = 'u'; 
        normal_lowercase[0x17] = 'i';
        normal_lowercase[0x18] = 'o'; 
        normal_lowercase[0x19] = 'p'; 
        normal_lowercase[0x1A] = '['; 
        normal_lowercase[0x1B] = ']'; 
        normal_lowercase[0x1E] = 'a'; 
        normal_lowercase[0x1F] = 's'; 
        normal_lowercase[0x20] = 'd'; 
        normal_lowercase[0x21] = 'f'; 
        normal_lowercase[0x22] = 'g'; 
        normal_lowercase[0x23] = 'h';
        normal_lowercase[0x24] = 'j'; 
        normal_lowercase[0x25] = 'k'; 
        normal_lowercase[0x26] = 'l'; 
        normal_lowercase[0x27] = ';'; 
        normal_lowercase[0x28] = '\'';
        normal_lowercase[0x29] = '`'; 
        normal_lowercase[0x2B] = '\\';
        normal_lowercase[0x2C] = 'z'; 
        normal_lowercase[0x2D] = 'x'; 
        normal_lowercase[0x2E] = 'c';
        normal_lowercase[0x2F] = 'v'; 
        normal_lowercase[0x30] = 'b'; 
        normal_lowercase[0x31] = 'n'; 
        normal_lowercase[0x32] = 'm'; 
        normal_lowercase[0x33] = ','; 
        normal_lowercase[0x34] = '.'; 
        normal_lowercase[0x35] = '/'; 
        normal_lowercase[0x39] = ' ';

        normal_uppercase[0x02] = '!'; 
        normal_uppercase[0x03] = '@'; 
        normal_uppercase[0x04] = '#'; 
        normal_uppercase[0x05] = '$'; 
        normal_uppercase[0x06] = '%'; 
        normal_uppercase[0x07] = '^'; 
        normal_uppercase[0x08] = '&'; 
        normal_uppercase[0x09] = '*'; 
        normal_uppercase[0x0A] = '('; 
        normal_uppercase[0x0B] = ')';
        normal_uppercase[0x0C] = '_'; 
        normal_uppercase[0x0D] = '+'; 
        normal_uppercase[0x10] = 'Q'; 
        normal_uppercase[0x11] = 'W'; 
        normal_uppercase[0x12] = 'E'; 
        normal_uppercase[0x13] = 'R'; 
        normal_uppercase[0x14] = 'T'; 
        normal_uppercase[0x15] = 'Y'; 
        normal_uppercase[0x16] = 'U'; 
        normal_uppercase[0x17] = 'I';
        normal_uppercase[0x18] = 'O'; 
        normal_uppercase[0x19] = 'P'; 
        normal_uppercase[0x1A] = '{'; 
        normal_uppercase[0x1B] = '}'; 
        normal_uppercase[0x1E] = 'A'; 
        normal_uppercase[0x1F] = 'S'; 
        normal_uppercase[0x20] = 'D'; 
        normal_uppercase[0x21] = 'F'; 
        normal_uppercase[0x22] = 'G'; 
        normal_uppercase[0x23] = 'H';
        normal_uppercase[0x24] = 'J'; 
        normal_uppercase[0x25] = 'K'; 
        normal_uppercase[0x26] = 'L'; 
        normal_uppercase[0x27] = ':'; 
        normal_uppercase[0x28] = '"'; 
        normal_uppercase[0x29] = '~'; 
        normal_uppercase[0x2B] = '|'; 
        normal_uppercase[0x2C] = 'Z'; 
        normal_uppercase[0x2D] = 'X'; 
        normal_uppercase[0x2E] = 'C';
        normal_uppercase[0x2F] = 'V'; 
        normal_uppercase[0x30] = 'B'; 
        normal_uppercase[0x31] = 'N'; 
        normal_uppercase[0x32] = 'M'; 
        normal_uppercase[0x33] = '<'; 
        normal_uppercase[0x34] = '>'; 
        normal_uppercase[0x35] = '?'; 
        normal_uppercase[0x39] = ' ';

        keypad[0x37] = '*'; 
        keypad[0x47] = '7'; 
        keypad[0x48] = '8'; 
        keypad[0x49] = '9'; 
        keypad[0x4A] = '-'; 
        keypad[0x4B] = '4'; 
        keypad[0x4C] = '5'; 
        keypad[0x4D] = '6'; 
        keypad[0x4E] = '+'; 
        keypad[0x4F] = '1';
        keypad[0x50] = '2'; 
        keypad[0x51] = '3'; 
        keypad[0x52] = '0'; 
        keypad[0x53] = '.';

        special[0x01] = SpecialKeyCode::Escape;
        special[0x0E] = SpecialKeyCode::Backspace;
        special[0x0F] = SpecialKeyCode::Tab;
        special[0x1C] = SpecialKeyCode::Enter;
        special[0x1D] = SpecialKeyCode::LeftCtrl;
        special[0x2A] = SpecialKeyCode::LeftShift;
        special[0x36] = SpecialKeyCode::RightShift;
        special[0x38] = SpecialKeyCode::LeftAlt;
        special[0x3A] = SpecialKeyCode::CapsLock;
        special[0x3B] = SpecialKeyCode::F1;
        special[0x3C] = SpecialKeyCode::F2;
        special[0x3D] = SpecialKeyCode::F3;
        special[0x3E] = SpecialKeyCode::F4;
        special[0x3F] = SpecialKeyCode::F5;
        special[0x40] = SpecialKeyCode::F6;
        special[0x41] = SpecialKeyCode::F7;
        special[0x42] = SpecialKeyCode::F8;
        special[0x43] = SpecialKeyCode::F9;
        special[0x44] = SpecialKeyCode::F10;
        special[0x45] = SpecialKeyCode::NumLock;
        special[0x46] = SpecialKeyCode::ScrollLock;
        special[0x57] = SpecialKeyCode::F11;
        special[0x58] = SpecialKeyCode::F12;

        PIC::IRQ::ClearMask(1);
        return;
    }

    void EventHandle() {
        first_press = true;
        u8 Scancode = IO::Input::Byte(0x60);

        Event event;
        char c;

        if (Scancode & 0x80) {
            event.release = true;
            Scancode = Scancode & 0x7F; // clear last bit

            if (normal_lowercase[Scancode]) {
                event.type = EventType::Normal;
                if (is_shift_pressed) {
                    c = normal_uppercase[Scancode];
                } else {
                    c = normal_lowercase[Scancode];
                }

            } else if (keypad[Scancode]) {
                event.type = EventType::Keypad;
                c = keypad[Scancode];

            } else if (special[Scancode]) {
                event.type = EventType::Special;
                c = special[Scancode];
                if (c == SpecialKeyCode::LeftShift) {
                    is_shift_pressed = false;
                }

            } else {
                Panic::Common(
                    "Unknown scancode",
                    "An unknown scancode has been sent by a PS/2 device."
                );
            }

        } else {
            event.release = false;

            if (normal_lowercase[Scancode]) {
                event.type = EventType::Normal;
                if (is_shift_pressed) {
                    c = normal_uppercase[Scancode];
                } else {
                    c = normal_lowercase[Scancode];
                }

            } else if (keypad[Scancode]) {
                event.type = EventType::Keypad;
                c = keypad[Scancode];

            } else if (special[Scancode]) {
                event.type = EventType::Special;
                c = special[Scancode];
                if (c == SpecialKeyCode::LeftShift) {
                    is_shift_pressed = true;
                }

            } else {
                Panic::Common(
                    "Unknown scancode",
                    "An unknown scancode has been sent by a PS/2 device."
                );
            }
        }

        event.ident = c;
        
        CLI::HandleInput(event);
    }
}