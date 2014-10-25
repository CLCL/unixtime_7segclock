#ifndef DISP_H_
#define DISP_H_
#include "arduino.h"
#include "KitaLab7SEG.h"

// DISP 7セグメント10桁LED表示装置表示用補助ライブラリ 

// クラスの定義
class DISP
{
public:
  DISP(void);   // コンストラクタ
  void init(void);
  void display(String);            // 7セグ表示
  void blink(String, String);   // 7セグ交互表示
  void resetBlinkCounter(void); // 交互表示のカウンターリセット

private:
  KitaLab7SEG seg;
  unsigned short counter_blink; // 7セグLED ブリンク管理カウンタ
};

#endif
