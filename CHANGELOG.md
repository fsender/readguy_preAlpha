# Pre-alpha 0.0.2 - 2022/11/28

增加了屏幕驱动种类.

以下是受支持的所有显示屏幕型号, 不含三色屏幕驱动

``` C++
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
```
此版本仅用于驱动屏幕测试用.

更多的版本敬请关注 readguy_indev 分支. readguy_indev 会提供更多的功能支持

# Pre-alpha 0.0.1 - 2022/11/23

可以通过WiFi进行配置显示屏和SD卡设置. 目前只支持4种屏幕

开发可以使用adafruit_GFX的函数来绘制到EPD屏幕上.

目前没有按键的驱动, 还不能阅读, 只有墨水屏的demo.
