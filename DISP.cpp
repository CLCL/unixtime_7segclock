#include "Arduino.h"
#include "DISP.h"
#include "KitaLab7SEG.h"

// 数値表示ディスプレイ 表示用補助ライブラリ 

// コンストラクタ（初期化処理）
DISP::DISP() :lcd(0x3c, (uint8_t)127) {
  KitaLab7SEG k7seg;
  str7seg = "";
  counter_blink = 0;
}

void DISP::init() {
  k7seg.init();
  lcd.begin(16, 2);
  lcd.print("UNIX TIME.");
}

void DISP::display(String str) {
  // 現在表示文字列と同じだったら処理しない
  if ( str7seg.equals(str) ) return;
  str7seg = str;
  k7seg.display(str);
  lcd.setCursor(0, 0);
  lcd.print(str);
  Serial.println(str);
}

void DISP::blink(String str1, String str2)
{ // str1とstr2を交互に表示
  switch ( counter_blink & 0x08 ) { // 400msごと
  case 0:  
    k7seg.display(str1); 
    lcd.setCursor(0, 0);
    lcd.print(str1);
    Serial.println(str1);
    break;
  default: 
    k7seg.display(str2);
    lcd.setCursor(0, 0);
    lcd.print(str2);
    Serial.println(str2);
  }
  counter_blink++;
}

void DISP::resetBlinkCounter() {
  counter_blink = 0;
}

