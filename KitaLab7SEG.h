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

private:
  int SEGLED;     // I2C アドレス
  int digits[10]; // 7セグメントパターン（0～9）
};

#endif

