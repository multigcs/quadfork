

STMTYPE        = STM32F429_439xx
RCC_OPT        = -DHSE_VALUE=25000000 -DPLL_M=25
LINKER_SCRIPT  = src/system/STM32F405_FLASH.ld
ASM_SRC        = src/system/startup_stm32f429_439xx.s

