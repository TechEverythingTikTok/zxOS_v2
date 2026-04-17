// Tags.hpp
// Written by TechEverything

/*
Tag parsing and identifiers for Multiboot2
*/


#pragma once

#include "Integers.hpp"

namespace Multiboot2 {
    namespace Tags {
        namespace Types {
            extern const u32 End;
            extern const u32 CLI;
            extern const u32 Bootloader;
            extern const u32 MemoryMap;
            extern const u32 Framebuffer;
        }

        struct __attribute__((packed)) Tag {
            u32 type;
            u32 size;
        };

        namespace CLI {
            struct __attribute__((packed)) Tag {
                u32 type;   // 1
                u32 size;
                char str[];
            };
        }

        namespace Bootloader {
            struct __attribute__((packed)) Tag {
                u32 type;   // 2
                u32 size;
                char str[];
            };
        }

        namespace MemoryMap {
            struct __attribute__((packed)) Entry {
                u64 base_addr;
                u64 len;
                u32 type;
                u32 reserved;
            };

            struct __attribute__((packed)) Tag {
                u32 type;   // 6
                u32 size;
                u32 entry_size;
                u32 entry_versions;
                Entry entries[];
            };
        }

        namespace Framebuffer {
            struct __attribute__((packed)) Tag {
                u32 type;   // 8
                u32 size;
                u64 framebuffer_addr;
                u32 framebuffer_pitch;
                u32 framebuffer_width;
                u32 framebuffer_height;
                u8 framebuffer_bpp;
                u8 framebuffer_type;
                u8 reserved;
                u8 framebuffer_red_field_pos;
                u8 framebuffer_red_mask_size;
                u8 framebuffer_green_field_pos;
                u8 framebuffer_green_mask_size;
                u8 framebuffer_blue_field_pos;
                u8 framebuffer_blue_mask_size;
            };
        }

        void Parse(Multiboot2::Tags::Tag* general_tag);
    }

   namespace Container {
        extern const Multiboot2::Tags::CLI::Tag* cli;
        extern const Multiboot2::Tags::Bootloader::Tag* bootloader;
        extern const Multiboot2::Tags::Framebuffer::Tag* framebuffer;
        extern const Multiboot2::Tags::MemoryMap::Tag* mmap;
    }
}