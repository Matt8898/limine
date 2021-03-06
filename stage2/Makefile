CC = i386-elf-gcc
LD = i386-elf-gcc
OBJCOPY = i386-elf-objcopy

WERROR = -Werror
CFLAGS = -flto -Os -pipe -Wall -Wextra $(WERROR)

INTERNAL_CFLAGS = \
	-std=gnu11 \
	-fplan9-extensions \
	-ffreestanding \
	-fno-stack-protector \
	-fno-pic \
	-fomit-frame-pointer \
	-Wno-address-of-packed-member \
	-masm=intel \
	-mno-80387 \
	-mno-mmx \
	-mno-3dnow \
	-mno-sse \
	-mno-sse2 \
	-MMD \
	-I. \
	-I..

LDFLAGS = -flto -Os

INTERNAL_LDFLAGS = \
	-lgcc \
	-static-libgcc \
	-nostdlib \
	-no-pie \
	-z max-page-size=0x1000 \
	-static \
	-Tlinker.ld

.PHONY: all clean

C_FILES := $(shell find ./ -type f -name '*.c' | sort)
ASM_FILES := $(shell find ./ -type f -name '*.asm' | sort)
OBJ := $(ASM_FILES:.asm=.o) $(C_FILES:.c=.o)
HEADER_DEPS := $(C_FILES:.c=.d)

all: stage2.bin

stage2.bin: $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) $(INTERNAL_LDFLAGS) -o stage2.elf
	$(OBJCOPY) -O binary stage2.elf stage2.bin

-include $(HEADER_DEPS)

%.o: %.c
	$(CC) $(CFLAGS) $(INTERNAL_CFLAGS) -c $< -o $@

%.o: %.asm
	nasm $< -f elf32 -o $@

clean:
	rm -f stage2.bin stage2.elf $(OBJ) $(HEADER_DEPS)
