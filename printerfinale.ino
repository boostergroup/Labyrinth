const int buttons[] = {14,15,16,17,18,19,20};     // array of all button pins


#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"


  #include "booster2.h"
  #include "booster3.h"



#define TX_PIN 5     // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 6     // Arduino receive   GREEN WIRE   labeled TX on printer
SoftwareSerial mySerial(RX_PIN, TX_PIN);    // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial, 2);   // Pass addr to printer constructor



void setup() {
 
 Serial.begin(9600);

 mySerial.begin(19200);
 printer.begin(120)    // Init printer (same regardless of serial type)

 pinMode(14, INPUT);
 pinMode(15, INPUT);
 pinMode(19, INPUT);

}

void loop() {
 
 for(int i = 14; i < 20; ++i) {
 if(digitalRead(i) == HIGH) {
 
 delay(20);   // software de-bounce improves accuracy       
 
 
 if(digitalRead(i) == HIGH) {   // check switches
 
 Serial.print ("input");
 Serial.println(i);


if(digitalRead(14) == HIGH){

printer.printBitmap(384, 640, booster2);
}

else if(digitalRead(15) == HIGH){

printer.printBitmap(384, 640, booster3);
}

else if(digitalRead(19) == HIGH){

printer.feed(1);
printer.justify('C');
printer.setSize('M');
printer.println(F("FOLGE DEINEM WEG"));
printer.feed(2);
printer.println(F("- - - - - - - - - - - - - - - -"));
printer.feed(2);
printer.setSize('L');
printer.println(F("FINDE DEINEN WEG"));

}


 delay(120);
 }
 }
 }
}
