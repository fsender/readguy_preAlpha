# readguy 自由的墨水屏阅读器
A multi-platform E-paper TXT reader program for ESP chips

完全自由+跨平台的阅读器！而且还是gpl v3证书哟～。
All free and multi-platform TXT reader with a GPL Version 3 License.

---

2022/11/28 增加了屏幕驱动种类.

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

---
## 逝用方法 - 烧录

1. esp8266 版本直接烧录(烧录工具设置为0x0000即可)

2. esp32 版本按照以下流程进行烧录

  1. 在压缩包内准备好以下三个文件:

  如果压缩包内没有, 请浏览群文件内更大的压缩包, 其中应当右这些文件
  ```
  bootloader_dio_80m 
  partitions.bin     
  boot_app0.bin      
  firmware.bin       
  ```
  2. 打开烧录软件 flash_download_tool_3.9.2.exe 烧录选项: flash 80MHz, DIO模式

  ```
  bootloader_dio_80m 0x1000
  partitions.bin     0x8000
  boot_app0.bin      0xe000
  firmware.bin       0x10000
  ```
  3. 等待烧录完成

## 逝用方法 - 开始驱动

1. 烧录完毕之后, 打开手机WiFi, 连接到 AP 热点 readguy , 然后输入密码 12345678来连接到Wifi

2. 连接好后, 浏览网页 [192.168.4.1](http://192.168.4.1), 然后即可进入引脚配置页面

3. 根据您的电子墨水屏的引脚连接进行配置

4. 配置好之后, 根据墨水屏的显示内容, 输入到验证码输入框内, 确保屏幕显示正常

5. 随后就进入Hello World演示程序

---

支持芯片：esp8266/esp32/esp32s3/esp32s2/esp32c3
Supported chips: esp8266/esp32/esp32s3/esp32s2/esp32c3

支持屏幕：1.54寸价签屏，2.13寸价签屏，2.66，2.9，4.2均支持
Supported displays: 1.54-inch, 2.13-inch, 2.66-inch, 2.9-inch, 4.2-inch.
可以自己在代码中加入你想要的屏幕型号
You can add the screen model you want in the code.

支持sd卡读取
SD is available
仅需1-3个gpio按键
Only 1 button is needed and can be added to maximum 3 buttons.
可选的ds1302，ds1307，ds3231 rtc
Using a real-time clock module is optional.
可选的背光灯调节
Light-brightness-control is optional.

功能：
1.littlefs读取
2.web配网/文件管理
3.自定义布局的时钟（需要bmp图片和一个布局json文件）
4.心知天气
5.资讯查看+二维码查看
6.相册
7.txt阅读器带进度指示
8.sd卡固件更新/切换

使用方法
1.烧录成功后，即刻出现一个wifi热点“readguy”，（wifi密码为12345678）
2.连接此wifi之后访问192.168.4.1之后就可以对其中的初始设定项目更改。
3.尽情使用吧！
4.单按键操作说明
单击：向右移动
双击：向左移动
长按半秒：选择/确定
三连击：菜单/滑动条状态下可以输入数字快速定位，键盘输入状态下可以快速切换英文大小写
5.双按键操作说明：
右键单击：向右移动
左键单击：向左移动
右键长按半秒：确定
左键长按半秒：菜单/滑动条状态下可以输入数字快速定位，键盘输入状态下可以快速切换英文大小写
6.三按键操作说明：
左键向左，右键向右，中键确定。
左右键同时按下：菜单/滑动条状态下可以输入数字快速定位，键盘输入状态下可以快速切换英文大小写

项目思路:
1. 启动时, 通过WiFi进行初始化配置
2. 配置好之后, 程序会按照配置里面的数据进行对屏幕的初始化等工作
3. 如果屏幕初始化成功, 屏幕上将会显示一段文本, 然后在web上输入此文本即可完成显示设备验证
4. 按键初始化也需要验证. 只需要按下对应按键就可以了, 可以自由选择是低电平按下or高电平按下
5. sd卡和背光初始化是可选的
6. 设备最好都要支持littlefs来存放书籍文件