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

#include "aq.h"
#include "radio.h"
#include "sbus.h"
#include "aq_mavlink.h"
#include "imu.h"
#include "util.h"
#include "aq_timer.h"
#include <string.h>

sbusStruct_t sbusData __attribute__((section(".ccm")));

int sbusDecode(radioInstance_t *r) {
	if (sbusData.u.rawBuf[22] & 0b0100) {
		r->errorCount++;
		return -1;
	} else {
		r->channels[0] = 1024 - sbusData.u.channels.channel1;
		r->channels[1] = 1024 - sbusData.u.channels.channel2;
		r->channels[2] = 1696 - sbusData.u.channels.channel3;
		r->channels[3] = 1024 - sbusData.u.channels.channel4;
		r->channels[4] = 1024 - sbusData.u.channels.channel5;
		r->channels[5] = 1024 - sbusData.u.channels.channel6;
		r->channels[6] = 1024 - sbusData.u.channels.channel7;
		r->channels[7] = 1024 - sbusData.u.channels.channel8;
		r->channels[8] = 1024 - sbusData.u.channels.channel9;
		r->channels[9] = 1024 - sbusData.u.channels.channel10;
		r->channels[10] = 1024 - sbusData.u.channels.channel11;
		r->channels[11] = 1024 - sbusData.u.channels.channel12;
		r->channels[12] = 1024 - sbusData.u.channels.channel13;
		r->channels[13] = 1024 - sbusData.u.channels.channel14;
		r->channels[14] = 1024 - sbusData.u.channels.channel15;
		r->channels[15] = 1024 - sbusData.u.channels.channel16;
		r->channels[16] = sbusData.u.rawBuf[22] & 0b0001 ? 800 : -800;
		r->channels[17] = sbusData.u.rawBuf[22] & 0b0010 ? 800 : -800;

		return 1;
	}
}

int sbusCharIn(radioInstance_t *r, uint8_t c) {

	// force top of frame if its been more than 3ms since last input
	// this is a safeguard in case we started receiving bytes in the middle of a frame
	// shortest Sbus frame = 7ms - 3ms for data = 4ms minimum gap
	if (timerMicros() - sbusData.lastCharReceived > 3000) {
		sbusData.state = SBUS_WAIT_SYNC;
	}

	sbusData.lastCharReceived = timerMicros();

	switch (sbusData.state) {
		case SBUS_WAIT_SYNC:
			if (c == SBUS_START_CHAR) {
				sbusData.state = SBUS_WAIT_DATA;
				sbusData.dataCount = 0;
				sbusData.validFrame = 0;
			}
			break;

		case SBUS_WAIT_DATA:
			sbusData.u.rawBuf[sbusData.dataCount++] = c;
			if (sbusData.dataCount == 23) {
				sbusData.state = SBUS_WAIT_END;
			}
			// make sure at least one channel value byte is > 0
			// all zeros means something is wrong with Rx (observed in the wild)
			if (c) {
				sbusData.validFrame = 1;
			}
			break;

		case SBUS_WAIT_END:
			sbusData.state = SBUS_WAIT_SYNC;
			if ((c & SBUS_END_CHAR) == 0 && sbusData.validFrame) {
				return sbusDecode(r);
			}
			break;
	}

	return 0;
}

void sbusInit(radioInstance_t *r, USART_TypeDef *uart) {
	memset((void *)&sbusData, 0, sizeof(sbusData));

	r->serialPort = serialOpen(uart, SBUS_BAUD, USART_HardwareFlowControl_None, SBUS_RXBUF_SIZE, 0);
	serialChangeParity(r->serialPort, USART_Parity_Even);
	serialChangeStopBits(r->serialPort, USART_StopBits_2);
}
