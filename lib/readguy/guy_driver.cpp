#include "guy_driver.h"

template <class T>
T readguy::t_init(T t, bool initial){ // 参数 initial 默认为true.
  t->init(0,initial);
  t->setFullWindow();
  t->fillScreen(GxEPD_WHITE); //将来会改成我的logo的
  t->display(false);
  t->setPartialWindow(0,0,32767,32767); //此处的意思是全屏局刷开启
  return t;
}
//template <class T>
//void readguy::t_display(T t){
//  t->display(true);
//}

#define INDEV_VERSION_NO_WIFI 1 //debug专用, 必须define, 删掉就每次开机必重新配置
uint8_t readguy::init(){
  const char *tagname = "hwconfig";
  if(cali>=2) //已经初始化过了一次了, 为了防止里面一些volatile的东西出现问题....还是退出吧
    return 0;
  char config_data[18];
  nvsData.begin("readguy");  //初始化NVS
#if defined(INDEV_VERSION_NO_WIFI)
  if(!nvsData.isKey(tagname)){  //如果NVS没有录入数据, 需要打开WiFiAP模式初始化录入引脚数据
#endif
    //开启WiFi和服务器, 然后网页获取数据
    //以下代码仅供测试
    ap_setup();
    server_setup();
    for(;;){
      if(server_loop()) break;
      yield();
    }
    delay(300); //等待网页加载完再关掉WiFi. (有没有用存疑)
    sv.stop();
    WiFi.mode(WIFI_OFF);
#if defined(INDEV_VERSION_NO_WIFI)
  }
  else{ //看来NVS有数据, //从NVS加载数据, 哪怕前面的数据刚刚写入, 还没读取
    nvsData.getBytes(tagname,config_data,18);
    nvsData.end(); //用完NVS记得关闭, 生内存
    cali=config_data[0]; //未校准时, 此处为0, 启动时会自动开启配网模式进行初始化
    sd_share_spi = config_data[1]; // 为 true 时, sd卡与epd共享spi
    epd_driver_type=config_data[2]; // 对应的epd驱动程序代号
    //显示驱动部分, 显示默认使用vspi (vspi也是默认SPI库的通道)
    epd_mosi=config_data[3]; // 目标显示器的 MOSI 引脚
    epd_sclk=config_data[4]; // 目标显示器的 SCLK 引脚
    epd_cs  =config_data[5]; // 目标显示器的 CS   引脚
    epd_dc  =config_data[6]; // 目标显示器的 DC   引脚
    epd_rst =config_data[7]; // 目标显示器的 RST  引脚
    epd_busy=config_data[8]; // 目标显示器的 BUSY 引脚
    //sd卡驱动部分, 不共享时, 默认使用hspi (sd卡建议用hspi)
    sd_miso=config_data[9];  // 目标sd卡的 MISO 引脚, sd_share_spi == 1 时无效
    sd_mosi=config_data[10]; // 目标sd卡的 MOSI 引脚, sd_share_spi == 1 时无效
    sd_sclk=config_data[11]; // 目标sd卡的 SCLK 引脚, sd_share_spi == 1 时无效
    sd_cs  =config_data[12]; // 目标sd卡的 CS   引脚
    //按键驱动部分, 为负代表高触发, 否则低触发,
    //注意, 这里的io编号是加1的, 即 1或-1 代表 gpio0 的低触发/高触发
    btn1     =config_data[13]; 
    btn2     =config_data[14]; 
    btn3     =config_data[15]; 
    bl_pin   =config_data[16];  //前置光接口引脚IO
    rtc_type =config_data[17];  //使用的RTC型号(待定, 还没用上)
    setEpdDriver();  //初始化屏幕
    setMoreDriver();
  }
#endif
  Serial.println(F("init done."));
  cali=2;
  return sd_ok;
}
void readguy::setEpdDriver(){
  switch (epd_driver_type){
    case 0:
      guy_dev_154 = new GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT>
        (GxEPD2_154(epd_cs, epd_dc, epd_rst, epd_busy));
      gfx = t_init(guy_dev_154);
    break;
    case 1:
      guy_dev_154_D67 = new GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>
        (GxEPD2_154_D67(epd_cs, epd_dc, epd_rst, epd_busy));
      gfx = t_init(guy_dev_154_D67);
    break;
    case 2:
      guy_dev_213 = new GxEPD2_BW<GxEPD2_213, GxEPD2_213::HEIGHT>
        (GxEPD2_213(epd_cs, epd_dc, epd_rst, epd_busy));
      gfx = t_init(guy_dev_213);
    break;
    case 3:
      guy_dev_290 = new GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT>
        (GxEPD2_290(epd_cs, epd_dc, epd_rst, epd_busy));
      gfx = t_init(guy_dev_290);
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

  SPIClass sd_spi=SPIClass(HSPI); //其实即使共线也可以用HSPI驱动SD卡, 只可惜, 这个ESP8266也很麻烦
  sd_spi.begin(sd_sclk,sd_miso,sd_mosi); //初始化SPI
  sd_ok = SD.begin(sd_cs,sd_spi,16000000); //初始化频率为16MHz, 不敢太快, 稳定性问题
  if(btn1) pinMode(abs(btn1-1),(btn1>0)?INPUT_PULLUP:INPUT_PULLDOWN); //按键现在就初始化好了
  if(btn2) pinMode(abs(btn2-1),(btn1>0)?INPUT_PULLUP:INPUT_PULLDOWN); //按键现在就初始化好了
  if(btn3) pinMode(abs(btn3-1),(btn1>0)?INPUT_PULLUP:INPUT_PULLDOWN); //按键现在就初始化好了
  if(bl_pin!=-1) pinMode(bl_pin,OUTPUT); //这个是背光灯, 默认也支持PWM调节
  //if(digitalPinHasPWM(bl_pin)) ... //稍后加入PWM驱动...
  digitalWrite(bl_pin,HIGH);
  //关于按键策略, 我们在此使用多个Button2的类, 然后在一个task共享变量来确定上一个按键状态
  return sd_ok;
}

void readguy::display(bool part){
  //真的是我c++的盲区了啊....搜索了半天才找到可以这么玩的
  //......可惜'dynamic_cast' not permitted with -fno-rtti
  switch (epd_driver_type){
    case 0: guy_dev_154    ->display(part); break;
    case 1: guy_dev_154_D67->display(part); break;
    case 2: guy_dev_213    ->display(part); break;
    case 3: guy_dev_290    ->display(part); break;
  }
}
