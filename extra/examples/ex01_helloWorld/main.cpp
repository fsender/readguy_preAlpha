/**
 * @file main.cpp
 * @author ex01_helloWorld
 * @brief 基础的GXEPD2实例. 同时注入了少许功能性驱动代码.
 * http://www.lilygo.cn/prod_view.aspx?TypeId=50036&Id=1318
 * 项目基于 TTGO T-block Ep-bk (带有背光功能的ESP32电子墨水屏模块)
 * 
 * 注意屏幕所用的 EPD_RESET 其实是接在 GPIO38 上面, 手册写的 GPIO34 是错误的
 * GPIO38 为 SENSOR_CAPN 引脚(可能是供内部使用),
 * 在标准的 ESP32-WROOM 和 WROVER 模组上未引出此 GPIO.
 * 可能影响一部分功能的正常使用. 不过无伤大雅
 * 
 * @version 0.1
 * @date 2022-11-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(\
  GxEPD2_154_D67(/*CS=5*/ 5, /*DC=*/ 19, /*RST=*/ 27, /*BUSY=*/ 38)); // GDEH0154D67
const char HelloWorld[] = "Hello World!";
const char HelloArduino[] = "Hello Arduino!";
const char HelloEpaper[] = "Hello E-Paper!";
void helloWorld_full()
{
  //Serial.println("helloWorld");
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  // full window mode is the initial mode, set it anyway
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.setCursor(x, y);
  display.print(HelloWorld);
  display.display(false); // full update
  //Serial.println("helloWorld done");
}
void helloWorld(int i,int j)
{
  Serial.println("helloWorld");
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setPartialWindow(0,0,32767,32767); //此处的意思是全屏局刷开启
  //display.fillScreen(GxEPD_WHITE);
  display.setCursor(i,j);
  display.print(HelloWorld);
  display.display(true); // full update
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
  pinMode(34,INPUT);
  pinMode(32,INPUT);
  display.init();
  // first update should be full refresh
  helloWorld_full();
  helloWorld(10,10);
  helloWorld(10,30);
  helloWorld(10,50);
  display.fillScreen(GxEPD_WHITE);
  helloWorld(10,70);
  display.fillScreen(GxEPD_WHITE);
  helloWorld(10,90);
  display.fillScreen(GxEPD_WHITE);
  helloWorld(10,140);
  display.fillScreen(GxEPD_WHITE);
  helloWorld(10,160);
  Serial.println("Reading touch sensor...");
}
void loop(){
  Serial.print(digitalRead(32));
  delay(100);
}
