#include "Arduino.h"
#include "KitaLab7SEG.h"
#include <Wire.h>

// KitaLab 7セグメント10桁LED表示装置表示用補助ライブラリ 

// コンストラクタ（初期化処理）
KitaLab7SEG::KitaLab7SEG() {
  SEGLED = 0x60; // KitaLab製 10桁I2C 7セグ表示機用I2Cアドレス
  // 7セグメント点灯パターン
  int d[] = { // 7セグメント点灯パターン
    0x3f, 0x06, 0x5b, 0x4f, 0x66, // 0～5
    0x6d, 0x7d, 0x27, 0x7f, 0x6f, // 6～9
  };
  for (int i = 0; i < 10; i++) digits[i] = d[i];
}

void KitaLab7SEG::init() {
  Wire.begin();
  Wire.beginTransmission(SEGLED) ; // 通信の開始
  Wire.write(0);
  // 起動メッセージなどはここで作ろう
  Wire.write(0b00111110); // U
  Wire.write(0b01010100); // n(N)
  Wire.write(0b00000100); // i(I)
  Wire.write(0b01110110); // H(X)
  Wire.write(0);
  Wire.write(0b01111000); // t(T)
  Wire.write(0b00000100); // i(I)
  Wire.write(0b00110111); // ∏(M)
  Wire.write(0b11111001); // E.
  Wire.write(0);
  Wire.endTransmission();
}

void KitaLab7SEG::display(String str) {
  //stateLED.toggle();
  // 10桁7セグメントLEDに数値を表示
  Wire.beginTransmission(SEGLED); // 通信の開始
  Wire.write(0); // 書き込み桁位置（最上位桁）
  // 1桁ずつパターンを転送
  for (int i = 0; i < str.length(); i++) {
    int chr = str.charAt(i) - 48;
    int ptn = 0;
    if (chr >= 0) {
      ptn = digits[ str.charAt(i) - 48 ];
    }
    Wire.write(ptn);
  }
  Wire.endTransmission();
}
