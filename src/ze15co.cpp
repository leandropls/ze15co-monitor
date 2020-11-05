/*
  ze15co.cpp - ZE15-CO CO sensor library for Arduino
  Based on MHZ19 library available at https://github.com/crisap94/MHZ19
  
  The MIT License (MIT)

  Copyright © 2020 Leandro Lima

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the “Software”), to 
  deal in the Software without restriction, including without limitation the 
  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
  sell copies of the Software, and to permit persons to whom the Software is 
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in 
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
  DEALINGS IN THE SOFTWARE.

*/

#include "ze15co.hpp"
#include <Arduino.h>

#define WAIT_READ_TIMES 100
#define WAIT_READ_DELAY 10

// public
ZE15CO::ZE15CO(int rx, int tx)
{
	_rx_pin = rx;
	_tx_pin = tx;
}


//protected
void ZE15CO::writeCommand(uint8_t cmd[], uint8_t *response)
{
	SoftwareSerial ze15co_serial(_rx_pin, _tx_pin);
	ze15co_serial.begin(9600);
	ze15co_serial.write(cmd, REQUEST_CNT);
	ze15co_serial.write(ze15co_checksum(cmd));
	ze15co_serial.flush();

	if (response != NULL)
	{
		int i = 0;
		while (ze15co_serial.available() <= 0)
		{
			if (++i > WAIT_READ_TIMES)
			{
				Serial.println("error: can't get ZE15-CO response.");
				return;
			}
			yield();
			delay(WAIT_READ_DELAY);
		}
		ze15co_serial.readBytes(response, ZE15CO::RESPONSE_CNT);
	}
}

//private

float ZE15CO::getMeasurement()
{
	uint8_t buf[ZE15CO::RESPONSE_CNT];
	for (int i = 0; i < ZE15CO::RESPONSE_CNT; i++)
	{
		buf[i] = 0x0;
	}

	writeCommand(getppm, buf);

	if (buf[0] == 0xff && 
        buf[1] == 0x86 && 
        buf[2] == buf[6] &&
        buf[3] == buf[7] &&
        (buf[2] & 0x80) == 0 &&
        ze15co_checksum(buf) == buf[ZE15CO::RESPONSE_CNT - 1])
	{
        return ((float)((buf[2] & 0x1F) * 256 + buf[3])) * 0.1;
	}
    return -1;
}

uint8_t ZE15CO::ze15co_checksum(uint8_t com[])
{
	uint8_t sum = 0x00;
	for (int i = 1; i < ZE15CO::REQUEST_CNT; i++)
	{
		sum += com[i];
	}
	sum = 0xff - sum + 0x01;
	return sum;
}
