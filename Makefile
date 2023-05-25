AS = nasm
ASFLAGS = -f elf64

ARCH = x86_64

DISTDIR = dist/$(ARCH)

asm_source_files := $(shell find src/impl/$(ARCH) -name *.asm)
asm_object_files := $(patsubst src/impl/$(ARCH)/%.asm, build/$(ARCH)/%.o, $(asm_source_files))

$(asm_object_files): build/$(ARCH)/%.o : src/impl/$(ARCH)/%.asm
	mkdir -p $(dir $@) && \
	$(AS) $(ASFLAGS) $(patsubst build/$(ARCH)/%.o, src/impl/$(ARCH)/%.asm, $@) -o $@

$(DISTDIR)/kernel.iso: $(asm_object_files)
	mkdir -p $(DISTDIR) && \
	ld -n -o $(DISTDIR)/kernel.bin -T targets/$(ARCH)/linker.ld $(asm_object_files) && \
	cp $(DISTDIR)/kernel.bin targets/$(ARCH)/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o $(DISTDIR)/kernel.iso targets/$(ARCH)/iso

run: $(DISTDIR)/kernel.iso
	qemu-system-x86_64 -cdrom $(DISTDIR)/kernel.iso

clean:
	rm -rf $(asm_object_files) $(DISTDIR)/*