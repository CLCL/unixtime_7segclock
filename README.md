# unixtime_7segclock

Arduino用UNIX時間表示7セグメントLED表示器

![Photo1](https://github.com/CLCL/unixtime_7segclock/wiki/images/img1050_720.jpg)

## 説明

Arduino Pro Miniと10桁7セグメントLEDで作る、UNIX時間を表示する時計です。

## 機能

* UNIXエポック1970-01-01T00:00:00Zからの疑似経過秒表示（UNIX時間）
* 普通の時計（MMDDhhmmss）
* RTCハードウェアによる時刻のバックアップ
* タクトスイッチ2個による時刻合わせ機能

## Hardware

### 必要なもの

* Arduino Pro Mini 328 3.3V 8MHz（1,124円）[[switch-science.com](http://www.switch-science.com/catalog/876/)]
* DS3231 RTC Battery 3.3V （160円）[[aliexpress.com](http://www.aliexpress.com/snapshot/6174388492.html)]
* I2Cバス用双方向電圧レベル変換モジュールPCA9306（150円）[[akizukidenshi.com](http://akizukidenshi.com/catalog/g/gM-05452)]
* 10桁7セグ表示モジュール基板 赤色7seg付き（3,600円） [[kitalab.com](http://store.kitalab.com/p_segmod.html)]
* 2ピンのタクトスイッチ 20個入り（637円） [[switch-science.com](http://www.switch-science.com/catalog/1706/)]
* ブレッドボード用 固いジャンパワイヤ（257円）[[switch-science.com](http://www.switch-science.com/catalog/314/)]
* ジャンパワイヤ オス～メス（446円）[[switch-science.com](http://www.switch-science.com/catalog/209/)]
* USBマイクロBコネクタ・ピッチ変換基板（220円）[[switch-science.com](http://www.switch-science.com/catalog/1599/)]
* SparkFun 超小型ブレッドボード 白（445円）[[switch-science.com](http://www.switch-science.com/catalog/1475/)]

### オプション

* アクリル板 2mm厚 スモークブラウン1枚・2mm厚 黒2枚（約1,500円）
  - ケースがあると出来栄えが全く違うので、ハードルは高いですが用意したほうがいいです

なお、全て5V製品（[Arduino Pro Mini 328 5V 16MHz](http://www.switch-science.com/catalog/946/)、[DS1307 リアルタイムクロック・モジュール](http://www.switch-science.com/catalog/1726/) など）を使うとI2Cバスレベル変換モジュールが不要になります。

安価に作りたい場合は、時間はかかりますが、すべての部品をAliExpressなどの海外通販サイトで集めるとよいです。

## Software

* Arduino IDE 1.0.6

## 回路図・配線

（後で書く）

## Images

![Photo2](https://github.com/CLCL/unixtime_7segclock/wiki/images/img1051_720.jpg)
