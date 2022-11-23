/**
 * @file guy_driver.h
 * @author your name (you@domain.com)
 * @brief 包含多个屏幕驱动程序的移植, 功能性的引脚设定等
 * @date 2022-11-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _GUY_DRIVER_H_FILE
#define _GUY_DRIVER_H_FILE

//                                  ------------------ definations - 定义 -
#define GUY_DEV_SUPPORTED_IC_NUM 4        //支持的ic数量

//                                  ------------------- includings - 包含 -
#include <Arduino.h>
#include <Preferences.h> //需要NVS才可以读取引脚信息
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "HTTPUpdateServer.h"
#include <esp-fs-webserver.h>

class readguy{ // readguy 基础类
  protected:
    //数据是否已经校准
    int8_t cali=0; //未校准时, 此处为0, 启动时会自动开启配网模式进行初始化
    //校准之后从nvs/eeprom读取的是1, 这样启动时就可以确定可以正常运行了, 然后设为2
    //当为2时, 函数是不可重入的, 因为init里面有些函数是智能执行一次的
    int8_t sd_share_spi = true; // 为 true 时, sd卡与epd共享spi
    int8_t epd_driver_type; // 对应的epd驱动程序代号
    //显示驱动部分, 显示默认使用vspi (vspi也是默认SPI库的通道)
    int8_t epd_mosi; // 目标显示器的 MOSI 引脚
    int8_t epd_sclk; // 目标显示器的 SCLK 引脚
    int8_t epd_cs  ; // 目标显示器的 CS   引脚
    int8_t epd_dc  ; // 目标显示器的 DC   引脚
    int8_t epd_rst ; // 目标显示器的 RST  引脚
    int8_t epd_busy; // 目标显示器的 BUSY 引脚
    //sd卡驱动部分, 默认使用hspi (sd卡建议用hspi)
    int8_t sd_miso; // 目标sd卡的 MISO 引脚, sd_share_spi == 1 时无效
    int8_t sd_mosi; // 目标sd卡的 MOSI 引脚, sd_share_spi == 1 时无效
    int8_t sd_sclk; // 目标sd卡的 SCLK 引脚, sd_share_spi == 1 时无效
    int8_t sd_cs  ; // 目标sd卡的 CS   引脚
    //按键驱动部分, 为负代表高触发, 否则低触发,
    //注意, 这里的io编号是加1的, 即 1或-1 代表 gpio0 的低触发/高触发
    int8_t btn1     ; 
    int8_t btn2     ; 
    int8_t btn3     ; 
    int8_t bl_pin   ; //前置光接口引脚IO
    int8_t rtc_type ; //使用的RTC型号(待定, 还没用上)
    int8_t sd_ok    ; //SD卡已经成功初始化
    char randomch[4]; //校验用字符串
    //NVS数据
    Preferences nvsData;
    //以下是支持的所有屏幕型号
    GxEPD2_BW<GxEPD2_154     , GxEPD2_154     ::HEIGHT> *guy_dev_154;
    GxEPD2_BW<GxEPD2_154_D67 , GxEPD2_154_D67 ::HEIGHT> *guy_dev_154_D67;
    GxEPD2_BW<GxEPD2_213     , GxEPD2_213     ::HEIGHT> *guy_dev_213;
    GxEPD2_BW<GxEPD2_290     , GxEPD2_290     ::HEIGHT> *guy_dev_290;
    //.........敬请期待更多屏幕ic...........
    GxEPD2_GFX_BASE_CLASS *gfx; // 图形引擎类指针, 可以用这个指针去操作屏幕缓冲区

    //内部调用函数: 初始化epd
    template <class T> T t_init(T t, bool initial = true);
    //template <class T>
    //void t_display(T t);

    WebServer sv;
    HTTPUpdateServer httpUpdater;
    void handleInit();
    void handleInitPost();
    void handleFinal();
    void handleFinalPost();
    void handleWiFiPost();
    void handleNotFound();
    void FSBrowser_setup();
    /// @brief 初始化屏幕, 设置驱动代号, 引脚排列顺序
    void setEpdDriver();
    /** @brief 初始化按钮, 背光和SD卡, 设置驱动代号, 引脚排列顺序
     *  @return SD卡初始化成功与否 */
    bool setMoreDriver();

  public:
    /** @brief 初始化readguy
     *  @return SD卡是否就绪
     */
    uint8_t init();
    GxEPD2_GFX_BASE_CLASS *g() const { return gfx; }; //使用 gfx 绘制内容.
    void display(bool part = true);
    //ap配网设置页面
    void ap_setup();
    void server_setup();
    bool server_loop();


  private:
    static const PROGMEM char *header_html;
    static const PROGMEM char *s_html ;
    static const PROGMEM char *v_html;
    static const PROGMEM char *f_html;
    static const PROGMEM char *f_html2;
    static const PROGMEM uint8_t faviconData[1150];
};
#endif