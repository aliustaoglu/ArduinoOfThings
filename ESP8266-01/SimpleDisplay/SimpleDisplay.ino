/* ************************************
 @author Cüneyt Aliustaoğlu
 Simple display with ESP8266-01 using
* *************************************/

// Libraries
#include <Wire.h>
#include "SSD1306.h"

SSD1306 display(0x3c, 0, 2); //0x3C OLED address, GPIO0-> tx, GPIO2-> rx

void setup()
{
  Wire.pins(0, 2);
  Wire.begin(0, 2);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}

void loop()
{
  display.clear();
  
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  
  display.drawString(0, 10, "Hello World");
  display.drawString(0, 24, "Cuneyt Aliustaoglu");
 
  display.display();
  delay(10);
}