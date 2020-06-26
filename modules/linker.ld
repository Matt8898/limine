OUTPUT_FORMAT(binary)
ENTRY(main)

SECTIONS
{
    . = 0;

    .text : {
        *(.text.main)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
        *(COMMON)
        *(.bss*)
    }
}
