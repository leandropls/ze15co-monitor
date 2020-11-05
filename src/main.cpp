/*
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
#include <Arduino.h>
#include <Process.h>
#include "ze15co.hpp"

#define RX_PIN 10                                          // Rx pin which the ZE15-CO Tx pin is attached to
#define TX_PIN 11                                          // Tx pin which the ZE15-CO Rx pin is attached to


const String kInfluxBucketName = "";
const String kInfluxOrgId = "";
const String kInfluxToken = "";

const String kInfluxEndPoint = "https://us-west-2-1.aws.cloud2.influxdata.com/api/v2/write?org=" + kInfluxOrgId + "&bucket=" + kInfluxBucketName + "&precision=s";
const String kAuthHeader = "Authorization: Token " + kInfluxToken;

ZE15CO ze15co(RX_PIN, TX_PIN);

unsigned long getDataTimer = 0;
void saveData(float co);


void setup()
{
    Bridge.begin();
    Serial.begin(9600);                                      // Device to serial monitor feedback
}


void loop()
{
    if (millis() - getDataTimer >= 5000)
    {
        static float co = 0;

        co = ze15co.getMeasurement();
        Serial.println("conditions co=" + String(co));
        saveData(co);

        getDataTimer = millis();
    }
}

/**
 * Fetch data from given url
 * 
 * @param co current co ppm
 * 
 * This function writes the command's output to buffer only
 * up to it's maximum size, closing it with a \0 character.
 * 
 * Arduino Yun's bridge should be already initialized before calling this.
 */
void saveData(float co)
{
    Process p;

    p.begin("curl");
    p.addParameter(kInfluxEndPoint);
    p.addParameter("--header");
    p.addParameter(kAuthHeader);
    p.addParameter("--data-raw");
    p.addParameter("conditions co=" + String(co));
    p.run();
}