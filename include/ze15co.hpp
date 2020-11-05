/*
  ze15co.hpp - ZE15-CO CO sensor library for Arduino
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

#ifndef _ZE15CO
#define _ZE15CO

#include "Arduino.h"
#include "SoftwareSerial.h"


class ZE15CO
{
  public:
	ZE15CO(int rx, int tx);
	float getMeasurement();

  protected:
	void writeCommand(uint8_t com[], uint8_t response[]);

  private:
	uint8_t ze15co_checksum(uint8_t com[]);

	static const int REQUEST_CNT = 8;
	static const int RESPONSE_CNT = 9;

	// serial command
	uint8_t getppm[REQUEST_CNT] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	// Serial Pins
	int _rx_pin = -1;
	int _tx_pin = -1;
};

#endif
