/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)
 
/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
    /* Set the starting address of the kernel to 2MB. This is safer than 1MB due to UEFI compatibility. */
    . = 2M;

    /* First put the multiboot header, as it is required to be placed very early in the image. 
       After that, place the .text section. */
    .text BLOCK(4K) : ALIGN(4K)
    {
        *( .multiboot )   /* Multiboot header for the bootloader */
        *( .text )        /* Actual code of the kernel */
    }

    /* Read-only data section */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *( .rodata )      /* Read-only initialized data */
    }

    /* Read-write initialized data section */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *( .data )        /* Initialized data */
    }

    /* Read-write uninitialized data (BSS) and stack section */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *( .bss )         /* Uninitialized data */
        *( COMMON )       /* Common symbols (such as uninitialized global variables) */
    }

    /* Optionally, any other sections can be added below if required by the compiler. */
}
