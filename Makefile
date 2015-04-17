
BOARD_VER        ?= 6
BOARD_REV        ?= 0
DIMU_VER         ?= 0

TARGET           := autoquad
BUILDDIR         := build
OPTLVL           := s # Optimization level, can be [0, 1, 2, 3, s].
TOOLCHAIN_PREFIX := arm-none-eabi
OPENOCDCFG        = /usr/share/openocd/scripts/board/stm32f429discovery.cfg
AS                = $(TOOLCHAIN_PREFIX)-as
AR                = $(TOOLCHAIN_PREFIX)-ar
CC                = $(TOOLCHAIN_PREFIX)-gcc
LD                = $(TOOLCHAIN_PREFIX)-gcc
GDB               = $(TOOLCHAIN_PREFIX)-gdb
SIZE              = $(TOOLCHAIN_PREFIX)-size
OBJCOPY           = $(TOOLCHAIN_PREFIX)-objcopy
EXTLIBS          := Libraries
CMSIS            := $(EXTLIBS)/CMSIS
DSPLIB           := $(CMSIS)/DSP_Lib
STD_PERIPH       := $(EXTLIBS)/STM32F4xx_StdPeriph_Driver


INCLUDE += -I$(STD_PERIPH)/inc
INCLUDE += -I$(CMSIS)/Include
INCLUDE += -I$(CMSIS)/Device/ST/STM32F4xx/Include
INCLUDE += -I$(EXTLIBS)/Mavlink/include/autoquad

INCLUDE += -I$(CURDIR)/src
INCLUDE += -I$(CURDIR)/src/math
INCLUDE += -I$(CURDIR)/src/system
INCLUDE += -I$(CURDIR)/src/coocox
INCLUDE += -I$(CURDIR)/src/targets
INCLUDE += -I$(CURDIR)/src/drivers
INCLUDE += -I$(CURDIR)/src/drivers/can
INCLUDE += -I$(CURDIR)/src/drivers/usb
INCLUDE += -I$(CURDIR)/src/drivers/sensors
INCLUDE += -I$(CURDIR)/src/drivers/protocols

STM_SRC  = 
STM_SRC += $(STD_PERIPH)/src/misc.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_adc.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_can.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_crc.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_dma.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_i2c.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_pwr.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_rcc.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_rng.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_rtc.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_sai.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_spi.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_tim.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_gpio.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_hash.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_exti.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_iwdg.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_ltdc.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_sdio.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_wwdg.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_flash.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_usart.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_syscfg.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_dbgmcu.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_hash_md5.c
STM_SRC += $(STD_PERIPH)/src/stm32f4xx_hash_sha1.c
STM_SRC += $(DSPLIB)/Source/SupportFunctions/arm_fill_f32.c
STM_SRC += $(DSPLIB)/Source/SupportFunctions/arm_copy_f32.c
STM_SRC += $(DSPLIB)/Source/StatisticsFunctions/arm_std_f32.c
STM_SRC += $(DSPLIB)/Source/BasicMathFunctions/arm_scale_f32.c
STM_SRC += $(DSPLIB)/Source/MatrixFunctions/arm_mat_mult_f32.c
STM_SRC += $(DSPLIB)/Source/MatrixFunctions/arm_mat_init_f32.c
STM_SRC += $(DSPLIB)/Source/MatrixFunctions/arm_mat_trans_f32.c
STM_SRC += $(DSPLIB)/Source/MatrixFunctions/arm_mat_inverse_f32.c

ASM_SRC = src/system/startup_stm32f429_439xx.s

SRC += src/system/fpu.c
SRC += src/system/rtc.c
SRC += src/system/rcc.c
SRC += src/system/util.c
SRC += src/system/syscalls+printf.c
SRC += src/system/system_stm32f4xx.c

SRC += src/drivers/ff.c
SRC += src/drivers/spi.c
SRC += src/drivers/i2c.c
SRC += src/drivers/pwm.c
SRC += src/drivers/adc.c
SRC += src/drivers/sdio.c
SRC += src/drivers/flash.c
SRC += src/drivers/esc32.c
SRC += src/drivers/1wire.c
SRC += src/drivers/eeprom.c
SRC += src/drivers/serial.c
SRC += src/drivers/ext_irq.c
SRC += src/drivers/digital.c
SRC += src/drivers/cyrf6936.c
SRC += src/drivers/signaling.c

SRC += src/drivers/usb/usb.c
SRC += src/drivers/usb/usb_bsp.c
SRC += src/drivers/usb/usb_dcd.c
SRC += src/drivers/usb/usbd_req.c
SRC += src/drivers/usb/usb_core.c
SRC += src/drivers/usb/usbd_desc.c
SRC += src/drivers/usb/usbd_core.c
SRC += src/drivers/usb/usbd_ioreq.c
SRC += src/drivers/usb/usb_dcd_int.c
SRC += src/drivers/usb/usbd_msc_bot.c
SRC += src/drivers/usb/usbd_msc_data.c
SRC += src/drivers/usb/usbd_msc_scsi.c
SRC += src/drivers/usb/usbd_storage_msd.c
SRC += src/drivers/usb/usbd_cdc_msc_core.c

SRC += src/drivers/can/can.c
SRC += src/drivers/can/canOSD.c
SRC += src/drivers/can/canUart.c
SRC += src/drivers/can/canCalib.c
SRC += src/drivers/can/canSensors.c

SRC += src/drivers/sensors/ms5611.c
SRC += src/drivers/sensors/hmc5983.c
SRC += src/drivers/sensors/mpu6000.c
SRC += src/drivers/sensors/max21100.c

SRC += src/drivers/sensors/l3gd20.c

SRC += src/drivers/protocols/ppm.c
SRC += src/drivers/protocols/dsm.c
SRC += src/drivers/protocols/sbus.c
SRC += src/drivers/protocols/ublox.c
SRC += src/drivers/protocols/frsky.c
SRC += src/drivers/protocols/grhott.c
SRC += src/drivers/protocols/mlinkrx.c
SRC += src/drivers/protocols/spektrum.c
SRC += src/drivers/protocols/aq_mavlink.c
SRC += src/drivers/protocols/telemetry.c

SRC += src/coocox/mm.c
SRC += src/coocox/sem.c
SRC += src/coocox/core.c
SRC += src/coocox/flag.c
SRC += src/coocox/time.c
SRC += src/coocox/task.c
SRC += src/coocox/port.c
SRC += src/coocox/arch.c
SRC += src/coocox/mbox.c
SRC += src/coocox/mutex.c
SRC += src/coocox/timer.c
SRC += src/coocox/queue.c
SRC += src/coocox/event.c
SRC += src/coocox/utility.c
SRC += src/coocox/serviceReq.c
SRC += src/coocox/kernelHeap.c

SRC += src/math/srcdkf.c
SRC += src/math/alt_ukf.c
SRC += src/math/nav_ukf.c
SRC += src/math/algebra.c
SRC += src/math/rotations.c

SRC += src/imu.c
SRC += src/run.c
SRC += src/nav.c
SRC += src/gps.c
SRC += src/pid.c
SRC += src/main.c
SRC += src/comm.c
SRC += src/calib.c
SRC += src/filer.c
SRC += src/d_imu.c
SRC += src/radio.c
SRC += src/gimbal.c
SRC += src/config.c
SRC += src/analog.c
SRC += src/motors.c
SRC += src/logger.c
SRC += src/control.c
SRC += src/command.c
SRC += src/compass.c
SRC += src/aq_init.c
SRC += src/aq_timer.c
SRC += src/supervisor.c
SRC += src/getbuildnum.c


CDEFS  = -DSTM32F429_439xx
CDEFS += -DUSE_STDPERIPH_DRIVER -D__FPU_USED -D__FPU_PRESENT=1 -D__SIZEOF_WCHAR_T=4 -D__ARM_ARCH_7EM__
CDEFS += -D__FPU_PRESENT=1 -DARM_MATH_CM4 -D__THUMB -DNESTED_INTERRUPTS -DCTL_TASKING
CDEFS += -DBOARD_TYPE=$(BOARD_TYPE) -DBOARD_VERSION=$(BOARD_VER) -DBOARD_REVISION=$(BOARD_REV) -DDIMU_VERSION=$(DIMU_VER)
CDEFS += -D__sqrtf=sqrtf

#CDEFS += -DHSE_VALUE=8000000 -DPLL_M=8
#LINKER_SCRIPT     = src/system/STM32F429_FLASH.ld
CDEFS += -DHSE_VALUE=25000000 -DPLL_M=25
LINKER_SCRIPT     = src/system/STM32F405_FLASH.ld

MCUFLAGS  = -mcpu=cortex-m4 -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard
MCUFLAGS += -fsingle-precision-constant -Wall -finline-functions
MCUFLAGS += -Wdouble-promotion -std=c99 -fno-dwarf2-cfi-asm  -mno-thumb-interwork
MCUFLAGS += -ffunction-sections -fdata-sections -fno-common -fmessage-length=0


COMMONFLAGS = -O$(OPTLVL) -g -Wall
CFLAGS      = $(COMMONFLAGS) $(MCUFLAGS) $(INCLUDE) $(CDEFS)
LDLIBS      = -lm -lc -lg
LDFLAGS     = $(COMMONFLAGS) $(MCUFLAGS) -fno-exceptions -ffunction-sections -fdata-sections -nostartfiles -Wl,-T,$(LINKER_SCRIPT)
LDFLAGS    += -Wl,--gc-sections -Wl,--Map=$(BUILDDIR)/$(TARGET).map
OBJ         = $(STM_SRC:%.c=$(BUILDDIR)/%.o) $(SRC:%.c=$(BUILDDIR)/%.o) $(ASM_SRC:%.s=$(BUILDDIR)/%.o)

all: $(OBJ)
	$(CC) -o $(BUILDDIR)/$(TARGET).elf $(LDFLAGS) $(OBJ) $(LDLIBS)
	$(OBJCOPY) -O ihex   $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/$(TARGET).hex
	$(OBJCOPY) -O binary $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/$(TARGET).bin

clean:
	rm -f $(OBJ)
	rm -f $(BUILDDIR)/$(TARGET).elf
	rm -f $(BUILDDIR)/$(TARGET).hex
	rm -f $(BUILDDIR)/$(TARGET).bin
	rm -f $(BUILDDIR)/$(TARGET).map

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^

$(BUILDDIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^

format:
	astyle --lineend=linux --max-code-length=140 --add-brackets --indent-switches --indent=tab --indent=force-tab --indent-col1-comments --style=google $(SRC) | grep -v "^Unchanged " || true

debug: $(BUILDDIR)/$(TARGET).elf
	@echo "### openocd -f $(OPENOCDCFG) ###"
	arm-none-eabi-gdb --command=$(CURDIR)/src/system/gdb.init $(BUILDDIR)/$(TARGET).elf

flash: $(BUILDDIR)/$(TARGET).hex
	stm32flash -w $(BUILDDIR)/$(TARGET).hex -v -g 0x0 -b 115200 /dev/ttyUSB0

