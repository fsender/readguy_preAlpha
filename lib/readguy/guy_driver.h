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

//                                  ------------------- includings - 包含 -
#include <Arduino.h>
#include <Preferences.h> //ESP32需要NVS才可以读取引脚信息
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "HTTPUpdateServer.h"
#include <esp-fs-webserver.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#define READGUY_GFX_BASE_CLASS GxEPD2_GFX_BASE_CLASS

#define READGUY_cali     (config_data[0])
#define READGUY_shareSpi (config_data[1])
#define READGUY_epd_type (config_data[2]) // 对应的epd驱动程序代号, -1为未指定
    //显示驱动部分, 显示默认使用vspi (vspi也是默认SPI库的通道)
#define READGUY_epd_mosi (config_data[3]) // 目标显示器的 MOSI 引脚
#define READGUY_epd_sclk (config_data[4]) // 目标显示器的 SCLK 引脚
#define READGUY_epd_cs   (config_data[5]) // 目标显示器的 CS   引脚
#define READGUY_epd_dc   (config_data[6]) // 目标显示器的 DC   引脚
#define READGUY_epd_rst  (config_data[7]) // 目标显示器的 RST  引脚
#define READGUY_epd_busy (config_data[8]) // 目标显示器的 BUSY 引脚
    //sd卡驱动部分, 默认使用hspi (sd卡建议用hspi)
#define READGUY_sd_miso  (config_data[9]) // 目标sd卡的 MISO 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_mosi  (config_data[10])// 目标sd卡的 MOSI 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_sclk  (config_data[11])// 目标sd卡的 SCLK 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_cs    (config_data[12])// 目标sd卡的 CS   引脚
    //按键驱动部分, 为负代表高触发, 否则低触发,
    //注意, 这里的io编号是加1的, 即 1或-1 代表 gpio0 的低触发/高触发
#define READGUY_btn1     (config_data[13]) 
#define READGUY_btn2     (config_data[14]) 
#define READGUY_btn3     (config_data[15]) 
#define READGUY_bl_pin   (config_data[16])//前置光接口引脚IO
#define READGUY_rtc_type (config_data[17])//使用的RTC型号(待定, 还没用上)

class readguy{ // readguy 基础类
  protected:
    //以下是支持的所有屏幕型号 Add devices here!
    GxEPD2_BW<GxEPD2_154     , GxEPD2_154     ::HEIGHT> *guy_dev_154;
    GxEPD2_BW<GxEPD2_154_D67 , GxEPD2_154_D67 ::HEIGHT> *guy_dev_154_D67;
    GxEPD2_BW<GxEPD2_154_T8  , GxEPD2_154_T8  ::HEIGHT> *guy_dev_154_T8;
    GxEPD2_BW<GxEPD2_154_M09 , GxEPD2_154_M09 ::HEIGHT> *guy_dev_154_M09;
    GxEPD2_BW<GxEPD2_213     , GxEPD2_213     ::HEIGHT> *guy_dev_213;
    GxEPD2_BW<GxEPD2_213_B72 , GxEPD2_213_B72 ::HEIGHT> *guy_dev_GxEPD2_213_B72;
    GxEPD2_BW<GxEPD2_213_B73 , GxEPD2_213_B73 ::HEIGHT> *guy_dev_GxEPD2_213_B73;
    GxEPD2_BW<GxEPD2_213_B74 , GxEPD2_213_B74 ::HEIGHT> *guy_dev_GxEPD2_213_B74;
    GxEPD2_BW<GxEPD2_213_flex, GxEPD2_213_flex::HEIGHT> *guy_dev_GxEPD2_213_flex;
    GxEPD2_BW<GxEPD2_213_M21 , GxEPD2_213_M21 ::HEIGHT> *guy_dev_GxEPD2_213_M21;
    GxEPD2_BW<GxEPD2_213_T5D , GxEPD2_213_T5D ::HEIGHT> *guy_dev_GxEPD2_213_T5D;
    GxEPD2_BW<GxEPD2_260     , GxEPD2_260     ::HEIGHT> *guy_dev_GxEPD2_260;
    GxEPD2_BW<GxEPD2_260_M01 , GxEPD2_260_M01 ::HEIGHT> *guy_dev_GxEPD2_260_M01;
    GxEPD2_BW<GxEPD2_266_BN  , GxEPD2_266_BN  ::HEIGHT> *guy_dev_GxEPD2_266_BN;
    GxEPD2_BW<GxEPD2_270     , GxEPD2_270     ::HEIGHT> *guy_dev_GxEPD2_270;
    GxEPD2_BW<GxEPD2_270_T91 , GxEPD2_270_T91 ::HEIGHT> *guy_dev_GxEPD2_270_T91;
    GxEPD2_BW<GxEPD2_290     , GxEPD2_290     ::HEIGHT> *guy_dev_290;
    GxEPD2_BW<GxEPD2_290_T5  , GxEPD2_290_T5  ::HEIGHT> *guy_dev_GxEPD2_290_T5;
    GxEPD2_BW<GxEPD2_290_T5D , GxEPD2_290_T5D ::HEIGHT> *guy_dev_GxEPD2_290_T5D;
    GxEPD2_BW<GxEPD2_290_T94 , GxEPD2_290_T94 ::HEIGHT> *guy_dev_GxEPD2_290_T94;
    GxEPD2_BW<GxEPD2_290_M06 , GxEPD2_290_M06 ::HEIGHT> *guy_dev_GxEPD2_290_M06;
    GxEPD2_BW<GxEPD2_420     , GxEPD2_420     ::HEIGHT> *guy_dev_GxEPD2_420;
    GxEPD2_BW<GxEPD2_420_M01 , GxEPD2_420_M01 ::HEIGHT> *guy_dev_GxEPD2_420_M01;
    //添加屏幕驱动范例: 直接添加对应屏幕的类就可以用了
    int8_t config_data[18];
    const char *projname = "readguy";
    const char *tagname = "hwconfig";
    //数据是否已经校准
    /* int8_t &cali = config_data[0]; //未校准时, 此处为0, 启动时会自动开启配网模式进行初始化
    //校准之后从nvs/eeprom读取的是1, 这样启动时就可以确定可以正常运行了, 然后设为2
    //当为2时, 函数是不可重入的, 因为init里面有些函数是智能执行一次的
    int8_t &sd_share_spi = config_data[1];    // 为 true 时, sd卡与epd共享spi
    int8_t &epd_driver_type = config_data[2]; // 对应的epd驱动程序代号, -1为未指定
    //显示驱动部分, 显示默认使用vspi (vspi也是默认SPI库的通道)
    int8_t &epd_mosi= config_data[3]; // 目标显示器的 MOSI 引脚
    int8_t &epd_sclk= config_data[4]; // 目标显示器的 SCLK 引脚
    int8_t &epd_cs  = config_data[5]; // 目标显示器的 CS   引脚
    int8_t &epd_dc  = config_data[6]; // 目标显示器的 DC   引脚
    int8_t &epd_rst = config_data[7]; // 目标显示器的 RST  引脚
    int8_t &epd_busy= config_data[8]; // 目标显示器的 BUSY 引脚
    //sd卡驱动部分, 默认使用hspi (sd卡建议用hspi)
    int8_t &sd_miso = config_data[9]; // 目标sd卡的 MISO 引脚, sd_share_spi == 1 时无效
    int8_t &sd_mosi = config_data[10];// 目标sd卡的 MOSI 引脚, sd_share_spi == 1 时无效
    int8_t &sd_sclk = config_data[11];// 目标sd卡的 SCLK 引脚, sd_share_spi == 1 时无效
    int8_t &sd_cs   = config_data[12];// 目标sd卡的 CS   引脚
    //按键驱动部分, 为负代表高触发, 否则低触发,
    //注意, 这里的io编号是加1的, 即 1或-1 代表 gpio0 的低触发/高触发
    int8_t &btn1    = config_data[13]; 
    int8_t &btn2    = config_data[14]; 
    int8_t &btn3    = config_data[15]; 
    int8_t &bl_pin  = config_data[16];//前置光接口引脚IO
    int8_t &rtc_type= config_data[17];//使用的RTC型号(待定, 还没用上) */
    int8_t sd_ok    ; //SD卡已经成功初始化
    char randomch[4]; //校验用字符串
    //NVS数据操作函数, 无NVS的使用EEProm的最后几个字节块
    Preferences nvsData;
    void nvs_init();
    void nvs_deinit();
    void nvs_read();
    void nvs_write();
    //.........敬请期待更多屏幕ic...........
    SPIClass *sd_spi =nullptr;
    SPIClass *epd_spi=nullptr;
    READGUY_GFX_BASE_CLASS *gfx; // 图形引擎类指针, 可以用这个指针去操作屏幕缓冲区
    //EpdIf *guy_dev = nullptr;

    //内部调用函数: 初始化epd
    template <class T> T t_init(T t, bool initial = true);
    //template <class T> void t_display(T t);

    WebServer sv;
    HTTPUpdateServer httpUpdater;
    void handleInit();      //服务器初始化系统(初次访问时, 跳转至引脚设定函数)
    void handleInitPost();  //服务器响应初始化请求
    void handlePinSetup();  //服务器-引脚设定函数
    void handleFinal();     //服务器-校验屏幕是否正常
    void handleFinalPost(); //服务器-校验屏幕是否正常回调函数
    void handleWiFiPost();  //服务器-处理WiFi连接相关内容和API接口密钥功能
    void handleNotFound();  //服务器-404响应
    void FSBrowser_setup(); //服务器-初始化文件服务器
    /// @brief 初始化屏幕, 设置驱动代号, 引脚排列顺序
    void setEpdDriver();
    /** @brief 初始化按钮, 背光和SD卡, 设置驱动代号, 引脚排列顺序
     *  @return SD卡初始化成功与否 */
    bool setMoreDriver();

  public:
    readguy();
    /** @brief 初始化readguy
     *  @return SD卡是否就绪
     */
    uint8_t init();
    READGUY_GFX_BASE_CLASS *g() const { return gfx; }; //使用 gfx 绘制内容.
    void display(bool part = true);
    //ap配网设置页面
    void ap_setup();
    void server_setup();
    bool server_loop();


  private:
    static constexpr size_t EPD_DRIVERS_NUM = 23;
    static const PROGMEM char *epd_drivers_list[EPD_DRIVERS_NUM];
    static const PROGMEM char header_html[];
    static const PROGMEM char index_cn_html[];
    static const PROGMEM char index_cn_html1[];
    static const PROGMEM char index_cn_html2[];
    static const PROGMEM char index_cn_html3[];
    static const PROGMEM char index_cn_html4[];
    static const PROGMEM char index_cn_html5[];
    static const PROGMEM char index_cn_html6[];
    static const PROGMEM char index_cn_html7[];
    static const PROGMEM char index_cn_html8[];
    static const PROGMEM char index_cn_html9[];
    static const PROGMEM char index_cn_html10[];
    static const PROGMEM char index_cn_html11[];
    static const PROGMEM char index_cn_html12[];
    static const PROGMEM char index_cn_html13[];
    static const PROGMEM char index_cn_html14[];
    static const PROGMEM char index_cn_html15[];
    static const PROGMEM char index_cn_html16[];
    static const PROGMEM char verify_html[];
    static const PROGMEM char final_html[];
    static const PROGMEM char final2_html[];
    static const PROGMEM uint8_t faviconData[1150];
};
#endif