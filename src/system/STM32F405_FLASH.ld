/*
*****************************************************************************
**
**  File        : stm32_flash.ld
**
**  Abstract    : Linker script for STM32F405 Device with
**                2048KByte FLASH, 128KByte RAM, 64KByte CCM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Environment : Atollic TrueSTUDIO(R)
**
**  Distribution: The file is distributed “as is,” without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

ENTRY(Reset_Handler)

_estack = 0x20020000;    /* end of RAM */
_Min_Heap_Size = 0;
_Min_Stack_Size = 0x400;

MEMORY {
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
  CCM (xrw)  : ORIGIN = 0x10000000, LENGTH = 64K
  RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS {
  .isr_vector : {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >FLASH

  .text : {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.rodata)
    *(.rodata*)
    *(.glue_7)
    *(.glue_7t)
	*(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
    _etext = .;
    _exit = .;
  } >FLASH

  .ARM.extab : {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } >FLASH

  .preinit_array : {
      PROVIDE_HIDDEN (__preinit_array_start = .);
      KEEP (*(.preinit_array*))
      PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH

  .init_array : {
     PROVIDE_HIDDEN (__init_array_start = .);
     KEEP (*(SORT(.init_array.*)))
     KEEP (*(.init_array*))
     PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH

 .fini_array : {
     PROVIDE_HIDDEN (__fini_array_start = .);
     KEEP (*(.fini_array*))
     KEEP (*(SORT(.fini_array.*)))
     PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  _sidata = .;
  .data : AT (_sidata) {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } >RAM

  . = ALIGN(4);
  .bss : {
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } >RAM

  .ccm : {
    _sccm = .;
    __ccm_start__ = _sccm;
    . = ALIGN(4);
    *(.ccm)
    *(.ccm.*)
    . = ALIGN(4);
    _eccm = .;
    __ccm_end__ = _eccm;
  } >CCM

  ._user_heap_stack : {
    . = ALIGN(4);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(4);
  } >RAM

  /DISCARD/ : {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : {
    *(.ARM.attributes)
  }
}
