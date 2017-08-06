#include "U8glib.h"
#include <stdio.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 

void pageTimeDay();     // Page-1
void pageTemperature(); // Page-2
void pageHumidity();    // Page-3
void pageInfo();        // Page-4

const int pageCount = 4;
int p;
void (*pages[pageCount])() = { pageTimeDay, pageTemperature, pageHumidity, pageInfo };
int duration [pageCount] = { 1000, 1000, 1000, 3000 };


void setup() {  
  u8g.setFont(u8g_font_unifont);
  u8g.setColorIndex(1);
  p = 0;
  Serial.begin(9600);
}

void loop() {  
  u8g.firstPage();
  do {  
    (*pages[p])();
  } while( u8g.nextPage() );
  delay(duration[p]);
  p = p+1;
  if (p == pageCount)
    p=0;
}
  


void pageTimeDay() {
  Serial.write("Time Day");
  Serial.println();
  u8g.drawStr( 0, 15, "July 15");
  return 0; 
}
void pageTemperature(){
  Serial.write("Temperature");
  Serial.println();
  u8g.drawStr( 0, 15, "27 degrees"); 
  return 0;
}
void pageHumidity() {
  Serial.write("Humidity");
  Serial.println();
  u8g.drawStr( 0, 15, "%65"); 
  return 0;
}
void pageInfo(){
  Serial.write("pageInfo");
  Serial.println();
  u8g.drawStr( 0, 15, "Cuneyt"); 
  u8g.drawStr( 0, 30, "Aliustaoglu"); 
  return 0;
}
