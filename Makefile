NASM = nasm -f elf32 -IInclude/Assembly
GPP = g++ -m32 -c \
	-ffreestanding -fno-rtti -fno-exceptions -fno-stack-protector -fno-pie \
	-IInclude/CPP -Wall -Wextra
LD = ld -m elf_i386 -nostdlib --nmagic -o ISO/boot/kernel.elf -T linker.ld
GRUB = grub-mkrescue -o zxos.iso

ASM_SOURCE = Source/Assembly
BUILD_ASM_SOURCE = Build/Assembly

CPP_SOURCE = Source/CPP
BUILD_CPP_SOURCE = Build/CPP

create:
# assembly
	mkdir -p $(BUILD_ASM_SOURCE)
	$(NASM) $(ASM_SOURCE)/multiboot2/multiboot2.asm -o $(BUILD_ASM_SOURCE)/multiboot2.o
	$(NASM) $(ASM_SOURCE)/setup/setup.asm -o $(BUILD_ASM_SOURCE)/setup.o
	$(NASM) $(ASM_SOURCE)/isr/isr.asm -o $(BUILD_ASM_SOURCE)/isr.o

# c++
	mkdir -p $(BUILD_CPP_SOURCE)
	$(GPP) $(CPP_SOURCE)/Kernel.cpp -o $(BUILD_CPP_SOURCE)/kernel.o

	$(GPP) $(CPP_SOURCE)/zx/VGA/Color.cpp -o $(BUILD_CPP_SOURCE)/vga_color.o
	$(GPP) $(CPP_SOURCE)/zx/VGA/Output.cpp -o $(BUILD_CPP_SOURCE)/vga_output.o

	$(GPP) $(CPP_SOURCE)/zx/Memory/Utilities.cpp -o $(BUILD_CPP_SOURCE)/memory_utilities.o
	$(GPP) $(CPP_SOURCE)/zx/Memory/String.cpp -o $(BUILD_CPP_SOURCE)/memory_string.o
	$(GPP) $(CPP_SOURCE)/zx/Memory/Heap.cpp -o $(BUILD_CPP_SOURCE)/memory_heap.o

	$(GPP) $(CPP_SOURCE)/zx/Multiboot2/Tags.cpp -o $(BUILD_CPP_SOURCE)/multiboot2_tags.o

	$(GPP) $(CPP_SOURCE)/zx/Permissions/GDT.cpp -o $(BUILD_CPP_SOURCE)/permissions_gdt.o

	$(GPP) $(CPP_SOURCE)/zx/Interrupts/IDT.cpp -o $(BUILD_CPP_SOURCE)/interrupts_idt.o
	$(GPP) $(CPP_SOURCE)/zx/Interrupts/ISR.cpp -o $(BUILD_CPP_SOURCE)/interrupts_isr.o
	$(GPP) $(CPP_SOURCE)/zx/Interrupts/PIC.cpp -o $(BUILD_CPP_SOURCE)/interrupts_pic.o

	$(GPP) $(CPP_SOURCE)/zx/Drivers/Keyboard.cpp -o $(BUILD_CPP_SOURCE)/drivers_keyboard.o

	$(GPP) $(CPP_SOURCE)/zx/Graphics/Basic.cpp -o $(BUILD_CPP_SOURCE)/graphics_basic.o
	$(GPP) $(CPP_SOURCE)/zx/Graphics/Bitmap.cpp -o $(BUILD_CPP_SOURCE)/graphics_bitmap.o
	$(GPP) $(CPP_SOURCE)/zx/Graphics/Bytemap.cpp -o $(BUILD_CPP_SOURCE)/graphics_bytemap.o
	$(GPP) $(CPP_SOURCE)/zx/Graphics/Console.cpp -o $(BUILD_CPP_SOURCE)/graphics_console.o

	$(GPP) $(CPP_SOURCE)/zx/Assets/Fonts/BitFont.cpp -o $(BUILD_CPP_SOURCE)/assets_fonts_bitfont.o
	$(GPP) $(CPP_SOURCE)/zx/Assets/Cursors/ByteCursor.cpp -o $(BUILD_CPP_SOURCE)/assets_cursors_bytecursor.o

	$(GPP) $(CPP_SOURCE)/zx/CLI/Master.cpp -o $(BUILD_CPP_SOURCE)/cli_master.o

	$(GPP) $(CPP_SOURCE)/zx/Panic.cpp -o $(BUILD_CPP_SOURCE)/panic.o
	$(GPP) $(CPP_SOURCE)/zx/Debug.cpp -o $(BUILD_CPP_SOURCE)/debug.o

# link
	$(LD) $(BUILD_ASM_SOURCE)/multiboot2.o $(BUILD_ASM_SOURCE)/setup.o \
		$(BUILD_CPP_SOURCE)/panic.o $(BUILD_CPP_SOURCE)/debug.o \
		$(BUILD_CPP_SOURCE)/memory_utilities.o $(BUILD_CPP_SOURCE)/memory_string.o $(BUILD_CPP_SOURCE)/memory_heap.o \
		$(BUILD_CPP_SOURCE)/assets_fonts_bitfont.o $(BUILD_CPP_SOURCE)/assets_cursors_bytecursor.o \
		$(BUILD_CPP_SOURCE)/multiboot2_tags.o \
		$(BUILD_ASM_SOURCE)/isr.o \
		$(BUILD_CPP_SOURCE)/interrupts_isr.o $(BUILD_CPP_SOURCE)/interrupts_idt.o \
		$(BUILD_CPP_SOURCE)/interrupts_pic.o \
		$(BUILD_CPP_SOURCE)/drivers_keyboard.o \
		$(BUILD_CPP_SOURCE)/cli_master.o \
		$(BUILD_CPP_SOURCE)/vga_color.o $(BUILD_CPP_SOURCE)/vga_output.o \
		$(BUILD_CPP_SOURCE)/graphics_basic.o \
		$(BUILD_CPP_SOURCE)/graphics_bytemap.o $(BUILD_CPP_SOURCE)/graphics_bitmap.o $(BUILD_CPP_SOURCE)/graphics_console.o \
		$(BUILD_CPP_SOURCE)/permissions_gdt.o \
		$(BUILD_CPP_SOURCE)/kernel.o

# grub
	$(GRUB) ISO

clean:
	rm -r Build
	rm -f zxos.iso