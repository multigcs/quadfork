/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "aq.h"
#include "telemetry.h"
#include "util.h"
#include "aq_timer.h"
#include "control.h"
#include "config.h"
#include "radio.h"
#include "imu.h"
#include "gps.h"
#include "nav.h"
#include "compass.h"
#include "motors.h"
#include "rcc.h"
#include "aq_mavlink.h"
#include "nav_ukf.h"
#include "supervisor.h"
#include "analog.h"
#include "comm.h"
#include "alt_ukf.h"
#include "run.h"
#include <CoOS.h>
#include <string.h>
#include <stdio.h>
#include <frsky.h>

static uint16_t loops;
static uint8_t frskyBuf[16];
static uint8_t frskyBufN;
static uint16_t sport_crc;

static void sport_Send (const uint8_t *buf, uint16_t len) {
	commTxBuf_t *txBuf;
	uint8_t *ptr;
	int i;
	txBuf = commGetTxBuf(COMM_STREAM_TYPE_FRSKY, len);
	if (txBuf != 0) {
		ptr = &txBuf->buf;
		for (i = 0; i < len; i++) {
			*ptr++ = *buf++;
		}
		commSendTxBuf(txBuf, len);
	}
	frskyBufN = 0;
	sport_crc = 0;
}

static void sport_addByte (uint8_t byte) {
	if(byte == 0x7E) {
		frskyBuf[frskyBufN++] = SPORT_STUFFING;
		frskyBuf[frskyBufN++] = 0x5E;
	} else if(byte == 0x7D) {
		frskyBuf[frskyBufN++] = SPORT_STUFFING;
		frskyBuf[frskyBufN++] = 0x5D;
	} else {
		frskyBuf[frskyBufN++] = byte;
	}
	sport_crc += byte;
	sport_crc += (sport_crc>>8);
	sport_crc &= 0x00FF;
	sport_crc += (sport_crc>>8);
	sport_crc &= 0x00FF;
}

static void sport_sendUint32(uint16_t dataTypeId, uint32_t data) {
	uint8_t *bytes = (uint8_t*)&dataTypeId;
	sport_addByte(SPORT_SENSOR_DATA_FRAME);
	sport_addByte(bytes[0]);
	sport_addByte(bytes[1]);
	bytes = (uint8_t*)&data;
	sport_addByte(bytes[0]);
	sport_addByte(bytes[1]);
	sport_addByte(bytes[2]);
	sport_addByte(bytes[3]);
	sport_addByte((uint8_t)(0x00FF - sport_crc));
	sport_Send(frskyBuf, frskyBufN);
}

static void sport_sendInt32(uint16_t dataTypeId, int32_t data) {
	uint8_t *bytes = (uint8_t*)&dataTypeId;
	sport_addByte(SPORT_SENSOR_DATA_FRAME);
	sport_addByte(bytes[0]);
	sport_addByte(bytes[1]);
	bytes = (uint8_t*)&data;
	sport_addByte(bytes[0]);
	sport_addByte(bytes[1]);
	sport_addByte(bytes[2]);
	sport_addByte(bytes[3]);
	sport_addByte((uint8_t)(0x00FF - sport_crc));
	sport_Send(frskyBuf, frskyBufN);
}

void frskyDo(void) {
	static uint8_t snum1 = 0;
	static uint8_t snum2 = 0;
	uint32_t latlong;
	if ((loops % 30) == 0) {
		if (snum2 < 2) {
			sport_sendUint32(RPM_ROT_DATA_ID, (uint32_t)(gpsData.numSV * 2));
			snum2++;
		} else if (snum2 < 4) {
			if (gpsData.lat < 0 ) {
				latlong = (uint32_t)(fabsf(gpsData.lat) * 600000) | 0x40000000;
			} else {
				latlong = (uint32_t)(fabsf(gpsData.lat) * 600000);
			}
			sport_sendUint32(GPS_LAT_LON_DATA_ID, latlong);
			snum2++;
		} else if (snum2 < 6) {
			if (gpsData.lon < 0) {
				latlong = (uint32_t)(fabsf(gpsData.lon) * 600000) | 0xC0000000;
			} else {
				latlong = (uint32_t)(fabsf(gpsData.lon) * 600000) | 0x80000000;
			}
			sport_sendUint32(GPS_LAT_LON_DATA_ID, latlong);
			snum2++;
		} else if (snum2 < 8) {
			sport_sendUint32(RPM_ROT_DATA_ID, (uint32_t)(gpsData.numSV * 2));
			snum2++;
		} else if (snum2 < 10) {
			sport_sendUint32(FCS_CURR_DATA_ID, (uint32_t)((analogData.extAmp * 100.0f)));
			snum2++;
		} else if (snum2 < 12) {
			sport_sendUint32(FCS_VOLT_DATA_ID, (uint32_t)((analogData.vIn * 100.0f)));
			snum2++;
		} else if (snum2 < 14) {
			sport_sendInt32(GPS_ALT_DATA_ID, (int32_t)(gpsData.height));
			snum2++;
		} else if (snum2 < 16) {
			sport_sendUint32(FRSKY_ID_TEMP1, (uint32_t)(gpsData.height));
			snum2++;
		} else if (snum2 < 18) {
			sport_sendUint32(GPS_SPEED_DATA_ID, (uint32_t)(gpsData.speed));
			snum2++;
		} else {
			snum2 = 0;
		}
	} else if ((loops % 10) == 0) {
		if (snum1 < 2) {
			sport_sendUint32(HEADING_DATA_ID, (uint32_t)(AQ_YAW * 100.0));
			snum1++;
		} else if (snum1 < 4) {
			sport_sendInt32(ACCX_DATA_ID, (int32_t)(AQ_ROLL * -100.0));
			snum1++;
		} else if (snum1 < 6) {
			sport_sendInt32(ACCY_DATA_ID, (int32_t)(AQ_PITCH * -100.0));
			snum1++;
		} else if (snum1 < 8) {
			sport_sendUint32(ACCZ_DATA_ID, (uint32_t)(AQ_YAW * 100.0));
			snum1++;
		} else if (snum1 < 10) {
			sport_sendInt32(VARIO_ALT_DATA_ID, (int32_t)(gpsData.height));
			snum1++;
		} else if (snum1 < 12) {
			if (navData.mode > NAV_STATUS_MANUAL) {
				sport_sendInt32(FRSKY_ID_FUEL, (int32_t)(0));
			} else if (navData.mode == NAV_STATUS_POSHOLD) {
				sport_sendInt32(FRSKY_ID_FUEL, (int32_t)(16));
			} else if (navData.mode == NAV_STATUS_MISSION) {
				sport_sendInt32(FRSKY_ID_FUEL, (int32_t)(3));
			} else if (navData.mode == NAV_STATUS_ALTHOLD) {
				sport_sendInt32(FRSKY_ID_FUEL, (int32_t)(2));
			} else if (navData.mode == NAV_STATUS_DVH) {
				sport_sendInt32(FRSKY_ID_FUEL, (int32_t)(13));
			} else {
				sport_sendInt32(FRSKY_ID_FUEL, (int32_t)(0));
			}
			snum1++;
		} else if (snum1 < 14) {
			if (supervisorData.state == STATE_DISARMED) {
				sport_sendUint32(FRSKY_ID_TEMP2, (uint32_t)(0));
			} else {
				sport_sendUint32(FRSKY_ID_TEMP2, (uint32_t)(1));
			}
			snum1++;
		} else {
			snum1 = 0;
		}
	}
	loops++;
}

void frskyInit(void) {
	commRegisterTelemFunc(frskyDo);
}


