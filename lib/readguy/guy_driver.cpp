#include "guy_driver.h"
#define INDEV_VERSION_NO_WIFI 1 //debug专用, 必须define, 删掉就每次开机必重新配置

template <class T>
T readguy::t_init(T t, bool initial){ // 参数 initial 默认为true.
  t->init(0,initial);
  t->setFullWindow();
  t->fillScreen(GxEPD_WHITE); //将来会改成我的logo的
  t->display(true);
  t->setPartialWindow(0,0,32767,32767); //此处的意思是全屏局刷开启
  return t;
}
//template <class T>
//void readguy::t_display(T t){
//  t->display(true);
//}

readguy::readguy(){
  READGUY_cali = 0; // config_data[0] 的初始值为0
}
uint8_t readguy::init(){
  //Serial.printf("sizeof (readguy) = %d\n",sizeof(readguy));
  if(READGUY_cali>=2) //已经初始化过了一次了, 为了防止里面一些volatile的东西出现问题....还是退出吧
    return 0;
  //char config_data[18];
  nvsData.begin(projname);  //初始化NVS
#if defined(INDEV_VERSION_NO_WIFI)
  if(!nvsData.isKey(tagname)){  //如果NVS没有录入数据, 需要打开WiFiAP模式初始化录入引脚数据
#endif
    //开启WiFi和服务器, 然后网页获取数据
    //以下代码仅供测试
    ap_setup();
    server_setup();
    for(uint32_t i=UINT32_MAX;millis()<i;){
      if(server_loop()){
        if(i==UINT32_MAX) i=millis()+500;
      }
      yield();
    }
    //delay(300); //等待网页加载完再关掉WiFi. (有没有用存疑)
    sv.stop();
    WiFi.mode(WIFI_OFF);
#if defined(INDEV_VERSION_NO_WIFI)
  }
  else{ //看来NVS有数据, //从NVS加载数据, 哪怕前面的数据刚刚写入, 还没读取
    nvsData.getBytes(tagname,config_data,sizeof(config_data));
    for(int i=0;i<sizeof(config_data);i++){
      Serial.printf_P(PSTR("data %d: %d\n"),i,config_data[i]);
    }
    setEpdDriver();  //初始化屏幕
    setMoreDriver();
  }
#endif
  nvsData.end(); //用完NVS记得关闭, 生内存
  Serial.println(F("init done."));
  READGUY_cali=2;
  return sd_ok;
}
void readguy::setEpdDriver(){
  Serial.println("setEpdDriver fx");
  for(int i=0;i<sizeof(config_data);i++){
    Serial.print(config_data[i]);
    Serial.print(' ');
  }
  //Serial.println("setEpdDriver fx");
  switch (READGUY_epd_type){
    case 0:
      guy_dev_154 = new GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT>
        (GxEPD2_154(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_154);
    break;
    case 1:
      guy_dev_154_D67 = new GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>
        (GxEPD2_154_D67(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_154_D67);
    break;
    case 2:
      guy_dev_154_T8 = new GxEPD2_BW<GxEPD2_154_T8, GxEPD2_154_T8::HEIGHT>
        (GxEPD2_154_T8(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_154_T8);
    break;
    case 3:
      guy_dev_154_M09 = new GxEPD2_BW<GxEPD2_154_M09, GxEPD2_154_M09::HEIGHT>
        (GxEPD2_154_M09(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_154_M09);
    break;
    case 4:
      guy_dev_213 = new GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT>
        (GxEPD2_213(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_213);
    break;
    case 5:
      guy_dev_GxEPD2_213_B72 = new GxEPD2_BW<GxEPD2_213_B72, GxEPD2_213_B72::HEIGHT>
        (GxEPD2_213_B72(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_213_B72);
    break;
    case 6:
      guy_dev_GxEPD2_213_B73 = new GxEPD2_BW<GxEPD2_213_B73, GxEPD2_213_B73::HEIGHT>
        (GxEPD2_213_B73(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_213_B73);
    break;
    case 7:
      guy_dev_GxEPD2_213_B74 = new GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT>
        (GxEPD2_213_B74(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_213_B74);
    break;
    case 8:
      guy_dev_GxEPD2_213_flex = new GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex::HEIGHT>
        (GxEPD2_213_flex(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_213_flex);
    break;
    case 9:
      guy_dev_GxEPD2_213_M21 = new GxEPD2_BW<GxEPD2_213_M21, GxEPD2_213_M21::HEIGHT>
        (GxEPD2_213_M21(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_213_M21);
    break;
    case 10:
      guy_dev_GxEPD2_213_T5D = new GxEPD2_BW<GxEPD2_213_T5D, GxEPD2_213_T5D::HEIGHT>
        (GxEPD2_213_T5D(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_213_T5D);
    break;
    case 11:
      guy_dev_GxEPD2_260 = new GxEPD2_BW<GxEPD2_260, GxEPD2_260::HEIGHT>
        (GxEPD2_260(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_260);
    break;
    case 12:
      guy_dev_GxEPD2_260_M01 = new GxEPD2_BW<GxEPD2_260_M01, GxEPD2_260_M01::HEIGHT>
        (GxEPD2_260_M01(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_260_M01);
    break;
    case 13:
      guy_dev_GxEPD2_266_BN = new GxEPD2_BW<GxEPD2_266_BN, GxEPD2_266_BN::HEIGHT>
        (GxEPD2_266_BN(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_266_BN);
    break;
    case 14:
      guy_dev_GxEPD2_270 = new GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT>
        (GxEPD2_270(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_270);
    break;
    case 15:
      guy_dev_GxEPD2_270_T91 = new GxEPD2_BW<GxEPD2_270_T91, GxEPD2_270_T91::HEIGHT>
        (GxEPD2_270_T91(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_270_T91);
    break;
    case 16:
      guy_dev_290 = new GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT>
        (GxEPD2_290(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_290);
    break;
    case 17:
      guy_dev_GxEPD2_290_T5 = new GxEPD2_BW<GxEPD2_290_T5, GxEPD2_290_T5::HEIGHT>
        (GxEPD2_290_T5(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_290_T5);
    break;
    case 18:
      guy_dev_GxEPD2_290_T5D = new GxEPD2_BW<GxEPD2_290_T5D, GxEPD2_290_T5D::HEIGHT>
        (GxEPD2_290_T5D(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_290_T5D);
    break;
    case 19:
      guy_dev_GxEPD2_290_T94 = new GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT>
        (GxEPD2_290_T94(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_290_T94);
    break;
    case 20:
      guy_dev_GxEPD2_290_M06 = new GxEPD2_BW<GxEPD2_290_M06, GxEPD2_290_M06::HEIGHT>
        (GxEPD2_290_M06(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_290_M06);
    break;
    case 21:
      guy_dev_GxEPD2_420 = new GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT>
        (GxEPD2_420(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_420);
    break;
    case 22:
      guy_dev_GxEPD2_420_M01 = new GxEPD2_BW<GxEPD2_420_M01, GxEPD2_420_M01::HEIGHT>
        (GxEPD2_420_M01(READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy));
      gfx = t_init(guy_dev_GxEPD2_420_M01);
    break;
  }
  gfx->setCursor(0,0);
  gfx->setTextColor(0);
  gfx->fillScreen(0xffff);
}
bool readguy::setMoreDriver(){
  /*重要信息: 有些引脚冲突是难以避免的, 比如8266 尤其需要重写这部分代码
    对于esp32也要注意这个引脚是否是一个合法的引脚
    对于esp8266真的要重写, 比如esp8266需要允许某些引脚是可以复用的
    此处的函数必须是可以反复调用的, 即 "可重入函数" 而不会造成任何线程危险 */
  if(READGUY_sd_cs>=0){
    if(sd_spi == nullptr) sd_spi = new SPIClass(HSPI);
    sd_spi->begin(READGUY_sd_sclk,READGUY_sd_miso,READGUY_sd_mosi); //初始化SPI
    sd_ok = SD.begin(READGUY_sd_cs,*sd_spi,16000000); //初始化频率为16MHz, 不敢太快, 稳定性问题
  }
  else sd_ok=0; //初始化按键. 注意高电平触发的引脚在初始化时要设置为下拉
  if(READGUY_btn1) pinMode(abs(READGUY_btn1)-1,(READGUY_btn1>0)?INPUT_PULLUP:INPUT_PULLDOWN); 
  if(READGUY_btn2) pinMode(abs(READGUY_btn2)-1,(READGUY_btn2>0)?INPUT_PULLUP:INPUT_PULLDOWN); 
  if(READGUY_btn3) pinMode(abs(READGUY_btn3)-1,(READGUY_btn3>0)?INPUT_PULLUP:INPUT_PULLDOWN); 
  if(READGUY_bl_pin>=0) {
    pinMode(READGUY_bl_pin,OUTPUT); //这个是背光灯, 默认也支持PWM调节
    if(digitalPinHasPWM(READGUY_bl_pin)){
    //digitalWrite(READGUY_bl_pin,HIGH);
      ledcAttachPin(READGUY_bl_pin, 0);//稍后加入PWM驱动...
      ledcSetup(0, 8000, 8);
      ledcWrite(0, 8);
    }
  }
  //关于按键策略, 我们在此使用多个Button2的类, 然后在一个task共享变量来确定上一个按键状态
  return sd_ok;
}

void readguy::display(bool part){
  //真的是我c++的盲区了啊....搜索了半天才找到可以这么玩的
  //......可惜'dynamic_cast' not permitted with -fno-rtti
  switch (READGUY_epd_type){
    case  0: guy_dev_154            ->display(part); break;
    case  1: guy_dev_154_D67        ->display(part); break;
    case  2: guy_dev_154_T8         ->display(part); break;
    case  3: guy_dev_154_M09        ->display(part); break;
    case  4: guy_dev_213            ->display(part); break;
    case  5: guy_dev_GxEPD2_213_B72 ->display(part); break;
    case  6: guy_dev_GxEPD2_213_B73 ->display(part); break;
    case  7: guy_dev_GxEPD2_213_B74 ->display(part); break;
    case  8: guy_dev_GxEPD2_213_flex->display(part); break;
    case  9: guy_dev_GxEPD2_213_M21 ->display(part); break;
    case 10: guy_dev_GxEPD2_213_T5D ->display(part); break;
    case 11: guy_dev_GxEPD2_260     ->display(part); break;
    case 12: guy_dev_GxEPD2_260_M01 ->display(part); break;
    case 13: guy_dev_GxEPD2_266_BN  ->display(part); break;
    case 14: guy_dev_GxEPD2_270     ->display(part); break;
    case 15: guy_dev_GxEPD2_270_T91 ->display(part); break;
    case 16: guy_dev_290            ->display(part); break;
    case 17: guy_dev_GxEPD2_290_T5  ->display(part); break;
    case 18: guy_dev_GxEPD2_290_T5D ->display(part); break;
    case 19: guy_dev_GxEPD2_290_T94 ->display(part); break;
    case 20: guy_dev_GxEPD2_290_M06 ->display(part); break;
    case 21: guy_dev_GxEPD2_420     ->display(part); break;
    case 22: guy_dev_GxEPD2_420_M01 ->display(part); break;
  }
}
