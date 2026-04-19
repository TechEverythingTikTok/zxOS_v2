// String.hpp
// Written by TechEverything

/*
Header for String.cpp
*/

#pragma once

#include "Integers.hpp"

namespace String {
    void Copy(
        char* dest, const char* src
    );

    bool Equals(
        const char* src1, const char* src2
    );

    void GetFirstWord(
        char* dest, const char* src
    );

    void Reverse(char* dest, const char* src, size len);

    namespace Check {
        bool IsNumber(char c);
    }

    namespace Convert {
        u8 CharToNumber(char c);

        i32 ASCIIToInteger(
            const char* src
        );

        void IntegerToASCII(
            char* dest, i32 src
        );
    }
}