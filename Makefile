AS = nasm
ASFLAGS = -f elf32
CC = gcc
CFLAGS = -I src/impl/$(ARCH) -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
			-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -g -c

ARCH = x86/32

asm_source_files := $(shell find src/impl/$(ARCH) -name *.asm)
asm_object_files := $(patsubst src/impl/$(ARCH)/%.asm, build/$(ARCH)/%.o, $(asm_source_files))

c_source_files := $(shell find src/impl/$(ARCH) -name *.c)
c_object_files := $(patsubst src/impl/$(ARCH)/%.c, build/$(ARCH)/%.o, $(c_source_files))

$(asm_object_files): build/$(ARCH)/%.o : src/impl/$(ARCH)/%.asm
	mkdir -p $(dir $@) && \
	$(AS) $(ASFLAGS) $(patsubst build/$(ARCH)/%.o, src/impl/$(ARCH)/%.asm, $@) -o $@

$(c_object_files): build/$(ARCH)/%.o : src/impl/$(ARCH)/%.c
	mkdir -p $(dir $@) && \
	$(CC) $(CFLAGS) $(patsubst build/$(ARCH)/%.o, src/impl/$(ARCH)/%.c, $@) -o $@

dist/$(ARCH)/kernel.elf: $(asm_object_files) $(c_object_files)
	mkdir -p dist/$(ARCH) && \
	ld -m elf_i386 -o dist/$(ARCH)/kernel.elf -T targets/$(ARCH)/link.ld $(asm_object_files) $(c_object_files)

run: dist/$(ARCH)/kernel.elf
	qemu-system-i386 -monitor stdio -kernel dist/$(ARCH)/kernel.elf

debug: dist/$(ARCH)/kernel.elf
	qemu-system-i386 -monitor stdio -kernel dist/$(ARCH)/kernel.elf -S -s

clean:
	rm -rf build/$(ARCH) dist/$(ARCH) targets/$(ARCH)/iso/boot/kernel.elf

dist/$(ARCH)/kernel.iso: $(asm_object_files) $(c_object_files)
	mkdir -p dist/$(ARCH) && \
	ld -m elf_i386 -o dist/$(ARCH)/kernel.elf -T targets/$(ARCH)/link.ld $(asm_object_files) $(c_object_files) && \
	cp dist/$(ARCH)/kernel.elf targets/$(ARCH)/iso/boot/kernel.elf && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/$(ARCH)/kernel.iso targets/$(ARCH)/iso

run-iso: dist/$(ARCH)/kernel.iso
	qemu-system-i386 -monitor stdio -cdrom dist/$(ARCH)/kernel.iso

debug-iso: dist/$(ARCH)/kernel.iso
	qemu-system-i386 -monitor stdio -cdrom dist/$(ARCH)/kernel.iso -S -s
