// Keyboard.hpp
// Written by TechEverything

/*
Driver for keyboard (PS-2)
*/

#pragma once

#include "Integers.hpp"

namespace Keyboard {
    extern bool first_press;

    namespace SpecialKeyCode {
        constexpr char Escape = 'e';
        constexpr char Backspace = 'b';
        constexpr char Tab = 't';
        constexpr char Enter = 'E';
        constexpr char LeftCtrl = 'c';
        constexpr char LeftShift = 's';
        constexpr char RightShift = 'S';
        constexpr char LeftAlt = 'a';
        constexpr char CapsLock = 'q';
        constexpr char F1 = '1';
        constexpr char F2 = '2';
        constexpr char F3 = '3';
        constexpr char F4 = '4';
        constexpr char F5 = '5';
        constexpr char F6 = '6';
        constexpr char F7 = '7';
        constexpr char F8 = '8';
        constexpr char F9 = '9';
        constexpr char F10 = ')';
        constexpr char F11 = '!';
        constexpr char F12 = '@';
        constexpr char NumLock = 'n';
        constexpr char ScrollLock = 'x';
    }

    extern u8 normal_lowercase[256];
    extern u8 normal_uppercase[256];
    extern u8 keypad[256];
    extern u8 special[256];

    extern bool is_shift_pressed;

    enum EventType {
        Normal,
        Keypad,
        Special
    };

    struct Event {
        bool release;
        EventType type;
        char ident;
    };

    void Initialize();
    void EventHandle();
}