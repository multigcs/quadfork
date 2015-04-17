

STMTYPE        = STM32F429_439xx
RCC_OPT        = -DHSE_VALUE=8000000 -DPLL_M=8
LINKER_SCRIPT  = src/system/STM32F429_FLASH.ld
ASM_SRC        = src/system/startup_stm32f429_439xx.s

