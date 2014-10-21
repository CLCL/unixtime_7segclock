#ifndef KitaLab7SEG_H_
#define KitaLab7SEG_H_
#include "arduino.h"

// KitaLab 7セグメント10桁LED表示装置表示用補助ライブラリ 

// クラスの定義
class KitaLab7SEG
{
public:
  KitaLab7SEG(void);   // コンストラクタ
  void init(void);
  void display(String);            // 7セグ表示
  void blink(String, String);   // 7セグ交互表示
  void resetBlinkCounter(void); // 交互表示のカウンターリセット

private:
  int SEGLED;     // I2C アドレス
  int digits[10]; // 7セグメントパターン（0～9）
  String str7seg; // 表示している文字
  unsigned short counter_blink; // 7セグLED ブリンク管理カウンタ
};

#endif

