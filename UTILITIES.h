#ifndef UTILITIES_H_
#define UTILITIES_H_
#include "arduino.h"
#include <Time.h>

// クラスの定義
class UTILITIES
{
public:
  UTILITIES(void);   // コンストラクタ
  time_t adjustCompiledTime(void);
  bool getTime(const char *str);
  bool getDate(const char *str);
private:
  tmElements_t tm; 
};

#endif

