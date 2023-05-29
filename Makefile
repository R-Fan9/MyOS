AS = nasm
ASFLAGS = -f elf64
CC = gcc
CFLAGS = -m64 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
			-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -g -c

ARCH = x86_64

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

dist/$(ARCH)/kernel.iso: $(asm_object_files) $(c_object_files)
	mkdir -p dist/$(ARCH) && \
	ld -m elf_x86_64 -o dist/$(ARCH)/kernel.elf -T targets/$(ARCH)/link.ld $(asm_object_files) $(c_object_files) && \
	cp dist/$(ARCH)/kernel.elf targets/$(ARCH)/iso/boot/kernel.elf && \
	grub-mkrescue /usr/lib/grub/x86_64-efi -o dist/$(ARCH)/kernel.iso targets/$(ARCH)/iso

run: dist/$(ARCH)/kernel.iso
	qemu-system-x86_64 -monitor stdio -cdrom dist/$(ARCH)/kernel.iso

debug: dist/$(ARCH)/kernel.iso
	qemu-system-x86_64 -monitor stdio -cdrom dist/$(ARCH)/kernel.iso -S -s

clean:
	rm -rf build/$(ARCH) dist/$(ARCH) targets/$(ARCH)/iso/boot/kernel.elf