AS = nasm
ASFLAGS = -f elf64

ARCH = x86_64

asm_source_files := $(shell find src/impl/$(ARCH) -name *.asm)
asm_object_files := $(patsubst src/impl/$(ARCH)/%.asm, build/$(ARCH)/%.o, $(asm_source_files))

$(asm_object_files): build/$(ARCH)/%.o : src/impl/$(ARCH)/%.asm
	mkdir -p $(dir $@) && \
	$(AS) $(ASFLAGS) $(patsubst build/$(ARCH)/%.o, src/impl/$(ARCH)/%.asm, $@) -o $@

dist/$(ARCH)/kernel.iso: $(asm_object_files)
	mkdir -p dist/$(ARCH) && \
	ld -n -o dist/$(ARCH)/kernel.bin -T targets/$(ARCH)/linker.ld $(asm_object_files) && \
	cp dist/$(ARCH)/kernel.bin targets/$(ARCH)/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/$(ARCH)/kernel.iso targets/$(ARCH)/iso

run: dist/$(ARCH)/kernel.iso
	qemu-system-x86_64 -cdrom dist/$(ARCH)/kernel.iso

clean:
	rm -rf build/ dist/ targets/$(ARCH)/iso/boot/kernel.bin