/*
    This file is part of AutoQuad.

    AutoQuad is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    AutoQuad is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with AutoQuad.  If not, see <http://www.gnu.org/licenses/>.

    Copyright © 2011-2014  Bill Nesbitt
*/

#ifndef _mpu6000_h
#define _mpu6000_h

#include "spi.h"
#include "util.h"

#define MPU6000_SPI_REG_BAUD	    SPI_BaudRatePrescaler_64	// initial setup only
#define MPU6000_SPI_RUN_BAUD	    SPI_BaudRatePrescaler_4	// 10.5 MHz

#define MPU6000_READ_BIT	    (0x01<<7)
#define MPU6000_WRITE_BIT	    (0x00<<7)

#define MPU6000_BYTES		    15
#define MPU6000_SLOT_SIZE	    ((MPU6000_BYTES+sizeof(int)-1) / sizeof(int) * sizeof(int))

#ifdef USE_QUATOS
#define MPU6000_SLOTS	    80						    // 100Hz bandwidth
#define MPU6000_DRATE_SLOTS	    (MPU6000_SLOTS * 100.0f * DIMU_INNER_DT * 2.0f) // variable
#else
#define MPU6000_SLOTS	    80						    // 100Hz bandwidth
#define MPU6000_DRATE_SLOTS	    40						    // 200Hz
#endif

#define BITS_DLPF_CFG_256HZ         0x00
#define BITS_DLPF_CFG_188HZ         0x01
#define BITS_DLPF_CFG_98HZ          0x02
#define BITS_DLPF_CFG_42HZ          0x03
#define BITS_DLPF_CFG_20HZ          0x04
#define BITS_DLPF_CFG_10HZ          0x05
#define BITS_DLPF_CFG_5HZ           0x06
#define BITS_DLPF_CFG_2100HZ_NOLPF  0x07


typedef struct {
	utilFilter_t tempFilter;
	spiClient_t *spi;
	volatile uint32_t spiFlag;
	volatile uint8_t rxBuf[MPU6000_SLOT_SIZE*MPU6000_SLOTS];
	volatile uint8_t slot;
	float rawTemp;
	float rawAcc[3];
	float rawGyo[3];
	float dRateRawGyo[3];
	float gyoOffset[3];
	volatile float acc[3];
	volatile float temp;
	volatile float gyo[3];
	volatile float dRateGyo[3];
	volatile uint32_t lastUpdate;
	float accSign[3];
	float gyoSign[3];
	uint8_t readReg;
	uint8_t enabled;
} mpu6000Struct_t;

extern mpu6000Struct_t mpu6000Data;

extern void mpu6000PreInit(void);
extern void mpu6000Init(void);
extern void mpu6600InitialBias(void);
extern void mpu6000Decode(void);
extern void mpu6000DrateDecode(void);
extern void mpu6000Enable(void);
extern void mpu6000Disable(void);

#endif
