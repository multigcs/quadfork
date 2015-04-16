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

/*
    This module handles the coordination of multiple I2C clients which may be
    sharing a I2C bus.  Once a transfer request has been queued, the caller
    will be notified via a flag or callback at their option when complete.
    Requests are handled on a FIFO basis.  It is up to the client(s) to make sure
    that they do not call i2cTransaction() concurrently as it is not thread safe.
    This can easily be handled if clients all submit transaction requests via
    data-ready ISR's of the same priority and preemption level.

    The Ethernet periphreal's interrupt lines are stolen for the I2C scheduler.
    If Ethernet is one day needed, this must be changed.
*/

#include "i2c.h"
#include "aq_timer.h"
#include "util.h"
#include "digital.h"

i2cStruct_t i2cData[3];

#ifdef I2C_I2C1_CLOCK
void i2c1Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	if (!i2cData[0].initialized) {
		// I2C interface
		RCC_APB2PeriphClockCmd(I2C_I2C1_CLOCK, ENABLE);

		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

		// I2C SCL / SDA pin configuration
		GPIO_InitStructure.GPIO_Pin = I2C_I2C1_SCL_PIN;
		GPIO_Init(I2C_I2C1_SCL_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = I2C_I2C1_SDA_PIN;
		GPIO_Init(I2C_I2C1_SDA_PORT, &GPIO_InitStructure);

		// Connect I2C pins to Alternate Function
		GPIO_PinAFConfig(I2C_I2C1_SCL_PORT, I2C_I2C1_SCL_SOURCE, I2C_I2C1_AF);
		GPIO_PinAFConfig(I2C_I2C1_SDA_PORT, I2C_I2C1_SDA_SOURCE, I2C_I2C1_AF);

		// configure I2C1
		I2C_InitStructure.I2C_ClockSpeed = 100000; // 100kHz
		I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; // I2C mode
		I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; // 50% duty cycle --> standard
		I2C_InitStructure.I2C_OwnAddress1 = 0x00; // own address, not relevant in master mode
		I2C_InitStructure.I2C_Ack = I2C_Ack_Disable; // disable acknowledge when reading (can be changed later on)
		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
		I2C_Init(I2C1, &I2C_InitStructure); // init I2C1
		// enable I2C1
		I2C_Cmd(I2C1, ENABLE);

		i2cData[0].initialized = 1;

	}
}
#endif

void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction) {
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2Cx, address, direction);
	if(direction == I2C_Direction_Transmitter) {
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	} else if(direction == I2C_Direction_Receiver) {
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
}

void I2C_write(I2C_TypeDef* I2Cx, uint8_t data) {
	I2C_SendData(I2Cx, data);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

uint8_t I2C_read_ack(I2C_TypeDef* I2Cx) {
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	uint8_t data = I2C_ReceiveData(I2Cx);
	return data;
}

uint8_t I2C_read_nack(I2C_TypeDef* I2Cx) {
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	I2C_GenerateSTOP(I2Cx, ENABLE);
	while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	uint8_t data = I2C_ReceiveData(I2Cx);
	return data;
}

void I2C_stop(I2C_TypeDef* I2Cx) {
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

