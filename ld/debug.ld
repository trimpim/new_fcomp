

/*
 * libraries
 */
GROUP (
  libstdc++.a
)


/*
 * variables
 */
__top_RamLoc8 = 0x10000000 +    /* RAM base address */
                0x2000 ;        /* 8K bytes */

/*
 * Memory regions
 */
MEMORY
{
  MFlash128 (rx)  : ORIGIN =        0x0, LENGTH = 0x20000     /* 128K bytes (alias Flash) */
  RamLoc8 (rwx)   : ORIGIN = 0x10000000, LENGTH = 0x2000      /*   8K bytes (alias RAM)   */
  Pheriphery (rw) : ORIGIN = 0x50000000, LENGTH = 0x1000000   /* xxxK bytes (alias Periphery configuration block */ 
}


/*
 * Sections to link
 */
SECTIONS
{
  .text : ALIGN(4)
  {
    __vectors_start__ = ABSOLUTE(.);
    KEEP(*(.isr_vector_list)) . = ALIGN(4);
    KEEP(*(.static_data)) . = ALIGN(4);

    *(.isr_functions*)

  } > MFlash128

  .text : ALIGN(4)
  {
    *(.isr_functions*)
    *(.text*)
    *(.rodata .rodata.* .constdata .constdata.*) . = ALIGN(4);

    KEEP(*(.init));
    . = ALIGN(4);
    KEEP(*(.fini));

  } > MFlash128

  .uninit_RESERVED : ALIGN(4)
  {
      KEEP(*(.bss.$RESERVED*));
      . = ALIGN(4) ;
      _end_uninit_RESERVED = .;
  } > RamLoc8

  /* pheriphery configuration block */
  .gpio_port1 0x50010000 :
  {
    KEEP(*(.gpio_port1))
  } > Pheriphery
  .gpio_port2 0x50020000 :
  {
    KEEP(*(.gpio_port2))
  } > Pheriphery

  PROVIDE(_vStackTop = __top_RamLoc8);
}
