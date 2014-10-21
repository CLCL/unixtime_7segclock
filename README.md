# unixtime_7segclock

Arduino用UNIX時間表示7セグメントLED表示器

![Photo1](https://github.com/CLCL/unixtime_7segclock/wiki/images/img_1056_720.jpg)

## 説明

Arduino Pro Miniと10桁7セグメントLEDで作る、UNIX時間を表示する時計です。

## 機能

* UNIXエポック1970-01-01T00:00:00Zからの疑似経過秒表示（UNIX時間）
* 普通の時計（MMDDhhmmss）
* RTCハードウェアによる時刻のバックアップ
* タクトスイッチ2個による時刻合わせ機能

## Hardware

### 必要なもの

詳細な部品表と参考価格は [Wiki](https://github.com/CLCL/unixtime_7segclock/wiki) で

* 1機  Arduino Pro Mini 328 3.3V 8MHz（※）
* 1個  DS3231 RTC Battery 3.3V（※）
* 1個  I2Cバス用双方向電圧レベル変換モジュールPCA9306（※）
* 1台  10桁7セグ表示モジュール基板 赤色7seg付き（※）
* 2個  2ピンのタクトスイッチ
* 適量 ブレッドボード用 固いジャンパワイヤ
* 2本  ジャンパワイヤ オス～メス I2Cバス引き回し用
* 1個  USBマイクロBコネクタ・ピッチ変換基板 5V電源入力用
* 1個  超小型ブレッドボード 白 連結できないタイプ
* 2ﾋﾟﾝ ピンヘッダ I2Cバス接続用

### オプション

* アクリル板 2mm厚 スモークブラウン1枚・2mm厚 黒2枚
  - ケースがあると出来栄えが全く違うので、ハードルは高いですが用意したほうがいいです

※全て5V製品（Arduino Pro Mini 328 5V 16MHz・、DS1307 リアルタイムクロック・モジュール など）を使うとI2Cバスレベル変換モジュールが不要になります。7セグ表示器は5V製品ですが3.3Vでも動かせるので固定抵抗の調整等検討してください。

安価に作りたい場合は、時間はかかりますが、すべての部品をAliExpressなどの海外通販サイトで集めるとよいです。

## Software

* Arduino IDE 1.0.6

## 回路図・配線

![breadboard1](https://github.com/CLCL/unixtime_7segclock/wiki/images/unixtime_7segclock_breadboard_720.jpg)

## Images

![Photo2](https://github.com/CLCL/unixtime_7segclock/wiki/images/img_1069_720.jpg)
