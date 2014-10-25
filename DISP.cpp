#include "Arduino.h"
#include "DISP.h"
#include "KitaLab7SEG.h"

// KitaLab 7セグメント10桁LED表示装置表示用補助ライブラリ 

// コンストラクタ（初期化処理）
DISP::DISP() {
  KitaLab7SEG seg;
  counter_blink = 0;
}

void DISP::init() {
  seg.init();
}

void DISP::display(String str) {
  seg.display(str);
  Serial.println(str);
}

void DISP::blink(String str1, String str2)
{ // str1とstr2を交互に表示
  switch ( counter_blink & 0x08 ) { // 400msごと
  case 0:  
    seg.display(str1); 
    Serial.println(str1);
    break;
  default: 
    seg.display(str2);
    Serial.println(str2);
  }
  counter_blink++;
}

void DISP::resetBlinkCounter() {
  counter_blink = 0;
}

