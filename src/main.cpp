#include <Arduino.h>
#include "guy_driver.h"
#include "Fonts/FreeMonoBold9pt7b.h"
const char HelloWorld[] = "Hello World!";
const char HelloArduino[] = "Hello Arduino!";
const char HelloEpaper[] = "Hello E-Paper!";

readguy g;
void helloWorld(int i,int j)
{
  Serial.println("helloWorld");
  g.g()->setFont(&FreeMonoBold9pt7b);
  g.g()->setTextColor(GxEPD_BLACK);
  //display.fillScreen(GxEPD_WHITE);
  g.g()->setCursor(i,j);
  g.g()->print(HelloWorld);
  g.display(true); // full update
  Serial.println("helloWorld done");
}
TaskHandle_t press2reset_h=nullptr;
void press2reset_f(void *pv){
  pinMode(36,INPUT_PULLUP);
  //digitalPinToTouchChannel(32)
  for(;;){
    if(!digitalRead(36)) esp_restart();
    vTaskDelay(10);
  }
}
void setup()
{
  Serial.begin(115200);
  Serial.println("setup");
  xTaskCreate(press2reset_f,"press2reset",1024,nullptr,1,&press2reset_h);
  delay(1000);
  pinMode(32,INPUT);
  g.init();
  // first update should be full refresh
  helloWorld(10,10);
  helloWorld(10,30);
  helloWorld(10,50);
  g.g()->fillScreen(GxEPD_WHITE);
  helloWorld(10,70);
  g.g()->fillScreen(GxEPD_WHITE);
  helloWorld(10,90);
  g.g()->fillScreen(GxEPD_WHITE);
  helloWorld(10,140);
  g.g()->fillScreen(GxEPD_WHITE);
  helloWorld(10,160);
  Serial.println("Reading touch sensor...");
}
void loop(){
  Serial.print(digitalRead(32));
  delay(100);
}
