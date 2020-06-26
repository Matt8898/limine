asm (
    ".section .entry\n\t"

    // Move built in modules where they do not bother
    "mov edi, OFFSET bss_end\n\t"
    "add edi, ecx\n\t"
    "push edi\n\t"
    "push OFFSET bss_end\n\t"
    "push ebx\n\t"
    "mov edi, OFFSET bss_end\n\t"
    "rep movsb\n\t"
    "and edx, 0xff\n\t"
    "push edx\n\t"

    // Zero out .bss
    "xor al, al\n\t"
    "mov edi, OFFSET bss_begin\n\t"
    "mov ecx, OFFSET bss_end\n\t"
    "sub ecx, OFFSET bss_begin\n\t"
    "rep stosb\n\t"

    "call main\n\t"
);

#include <drivers/vga_textmode.h>
#include <lib/real.h>
#include <lib/blib.h>
#include <lib/libc.h>
#include <lib/part.h>
#include <lib/config.h>
#include <lib/e820.h>
#include <lib/memmap.h>
#include <lib/print.h>
#include <lib/module.h>
#include <fs/file.h>
#include <lib/elf.h>
#include <protos/stivale.h>
#include <protos/linux.h>
#include <protos/templeos.h>
#include <protos/chainload.h>
#include <menu.h>

void main(int boot_drive, size_t module_count, void *modules, size_t balloc_base) {
    bump_allocator_base = balloc_base;

    init_vga_textmode();

    print("qloader2\n\n");

    print("Initialising %u built-in modules at %x...\n", module_count, modules);
    init_modules(module_count, modules);

    print("Boot drive: %x\n", boot_drive);

    // Look for config file.
    print("Searching for config file...\n");
    struct part parts[4];
    for (int i = 0; ; i++) {
        if (i == 4) {
            panic("Config file not found.");
        }
        print("Checking partition %d...\n", i);
        int ret = get_part(&parts[i], boot_drive, i);
        if (ret) {
            print("Partition not found.\n");
        } else {
            print("Partition found.\n");
            if (!init_config(boot_drive, i)) {
                print("Config file found and loaded.\n");
                break;
            }
        }
    }

    char *cmdline = menu();

    init_e820();
    init_memmap();

    char proto[32];
    if (!config_get_value(proto, 0, 32, "KERNEL_PROTO")) {
        if (!config_get_value(proto, 0, 32, "PROTOCOL")) {
            panic("PROTOCOL not specified");
        }
    }

    if (!strcmp(proto, "stivale")) {
        stivale_load(cmdline, boot_drive);
    } else if (!strcmp(proto, "linux")) {
        linux_load(cmdline, boot_drive);
    } else if (!strcmp(proto, "templeos")) {
        templeos_load(boot_drive);
    } else if (!strcmp(proto, "chainload")) {
        chainload();
    } else {
        panic("Invalid protocol specified");
    }
}
