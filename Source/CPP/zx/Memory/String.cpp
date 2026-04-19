// String.cpp
// Written by TechEverything

/*
String utilities for zxOS
*/

#include "Integers.hpp"

namespace String {
    void Copy(
        char* dest, const char* src
    ) {
        while (*src) {
            *dest = *src;
            dest++;
            src++;
        }

        *dest = '\0';
    }

    bool Equals(
        const char* src1, const char* src2
    ) {
        while (*src1 && *src2) {
            if (*src1 != *src2) return false;
            src1++;
            src2++;
        }

        return *src1 == *src2;
    }

    void GetFirstWord(
        char* dest, const char* src
    ) {
        while (*src && *src != ' ') {
            *dest = *src;
            dest++;
            src++;
        }

        *dest = '\0';
    }

    void Reverse(char* dest, const char* src, size len) {
        if (len == 0) {
            dest[0] = '\0';
            return;
        }

        for (size i = 0; i < len; i++) {
            dest[i] = src[len - i - 1];
        }

        dest[len] = '\0';
    }

    namespace Check {
        bool IsNumber(char c) {
            return (c >= '0') && (c <= '9');
        }
    }

    namespace Convert {
        u8 CharToNumber(char c) {
            return c - '0';
        }

        i32 ASCIIToInteger(
            const char* src
        ) {
            i32 number = 0;
            bool negative = false;

            while (*src == ' ') {
                src++;
            }

            if (*src == '-' || *src == '+') {
                negative = (*src == '-');
                src++;
            }

            while (*src && Check::IsNumber(*src)) {
                number = number * 10 + CharToNumber(*src);
                src++;
            }

            if (negative) number *= -1;

            return number;
        }

        void IntegerToASCII(
            char* dest, i32 src
        ) {
            if (src == 0) {
                dest[0] = '0';
                dest[1] = '\0';
                return;
            }
        
            bool negative = false;
            if (src < 0) {
                negative = true;
                src = -src;
            }
        
            char* start = dest;
        
            while (src != 0) {
                *dest++ = (src % 10) + '0';
                src /= 10;
            }
        
            if (negative) {
                *dest++ = '-';
            }
        
            *dest = '\0';
        
            size len = dest - start;
            for (size i = 0; i < len / 2; i++) {
                char tmp = start[i];
                start[i] = start[len - i - 1];
                start[len - i - 1] = tmp;
            }
        }
    }
}