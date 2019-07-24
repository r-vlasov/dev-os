ENTRY(start)
phys = 0x00100000;
SECTIONS
{
    . = phys;
    ._mbHeader : AT ( ADDR( ._mbHeader ) ) {
        *(._mbHeader)
    }

    .text : AT ( ADDR( .text ) ) {
	    code = .;
	    *(.text)
	    . = ALIGN(4096);
    }

    .data : AT ( ADDR( .text ) + ( data - code ) ) {
        data = .;
        *(.data)
        . = ALIGN(4096);
    }

    .bss : AT ( ADDR( .text) + ( bss - code ) ) {
        bss = .;
        *(.bss)
        . = ALIGN(4096);
    }

    kernel_end = .;
}
