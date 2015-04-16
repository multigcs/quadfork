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

#ifndef _i2c_h
#define _i2c_h

#include "digital.h"

typedef struct {
	uint8_t initialized;
} i2cStruct_t;

extern void i2c1Init(void);
extern void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
extern void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
extern uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
extern uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);
extern void I2C_stop(I2C_TypeDef* I2Cx);


#endif

