__stack_size = 1024;

MEMORY {
    rom(rx) : org = 0x00000000, len = 32k
    ram(rwx) : org = 0x10000000, len = 4k
}

SECTIONS {
    .bss : {
        __bss_start = .;
        *(.bss .bss.* .gnu.linkonce.b.*)
        __bss_end = .;
    } > ram

    .text : {
        KEEP(*(.vectors))

        *(.rodata .rodata.* .gnu.linkonce.r.*)

        __modules_start = .;
        KEEP(*(modules))
        __modules_end = .;
    } > rom

    .data : {
        __data_init_start = LOADADDR(.data);
        __data_start = .;
        *(.data .data.* .gnu.linkonce.d.*)
        __data_end = .;
    } > ram AT > rom

    .stack : {
        . += __stack_size;
        __stack_end = .;
    } > ram
}
