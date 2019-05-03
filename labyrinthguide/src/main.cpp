/*
 * 
 * This detects advertising messages of BLE devices and compares it with stored MAC addresses. 
 * If one matches and is inside a certain RSSI value, it lights up the LEDs one after another.

   Based on Neil Kolban's example file: https://github.com/nkolban/ESP32_BLE_Arduino
   And Andreas Spiess' Proximity Sensor  https://github.com/SensorsIot/Bluetooth-BLE-on-Arduino-IDE

   Copyright <2019> <Tim Fuchs>
   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
 */

#include "Arduino.h"
#include "BLEDevice.h"
#include <WiFi.h>


static BLEAddress *pServerAddress;


#define LED_COUNT 6
int leds[LED_COUNT] = {14, 27, 26, 25, 33, 32,};

BLEScan* pBLEScan;
BLEClient*  pClient;
bool deviceClose = false;

String knownAddresses[] = {"30:ae:a4:dd:9c:ae", "30:ae:a4:dd:a8:4e"};
std::vector<String> foundAdresses; 

#define TOPIC "outTopic"  // Change for your own topic
#define PAYLOAD "1"    // change for your own payload

unsigned long entry;


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //Serial.print("BLE Advertised Device found: ");
      //Serial.println(advertisedDevice.toString().c_str());
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());

      bool known = false;
      for (int i = 0; i < (sizeof(knownAddresses) / sizeof(knownAddresses[0])); i++) {
        if (strcmp(pServerAddress->toString().c_str(), knownAddresses[i].c_str()) == 0) known = true;
        if (known) {
          //Serial.print("Device found: ");
          //Serial.println(advertisedDevice.getRSSI());
          if (advertisedDevice.getRSSI() > -60) {
            if(std::find(foundAdresses.begin(), foundAdresses.end(), pServerAddress->toString().c_str()) == foundAdresses.end()) {
              foundAdresses.push_back(pServerAddress->toString().c_str());
              Serial.println(pServerAddress->toString().c_str());
            }
          }
          advertisedDevice.getScan()->stop();
        }
      }
    }
}; // MyAdvertisedDeviceCallbacks


void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");

  BLEDevice::init("");

  pClient  = BLEDevice::createClient();
  Serial.println(" - Created client");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
}

void loop() {

  pinMode(leds[foundAdresses.size()+1], OUTPUT);
  Serial.println();
  //Serial.println("BLE Scan restarted.....");
  //deviceClose = false;
  BLEScanResults scanResults = pBLEScan->start(1);

  for(int i = 0; i < foundAdresses.size(); i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], HIGH);
  }

}