#include "Arduino.h"
#include "UTILITIES.h"
#include <Time.h>

// コンストラクタ（初期化処理）
UTILITIES::UTILITIES() {
}

time_t UTILITIES::adjustCompiledTime() { // RTCをPCの時刻に合わせる
  // コンパイラが__DATE__と__TIME__を定数に置換する
  if (getDate(__DATE__) && getTime(__TIME__)) {
    // getDate/getTimeは定数文字列をグローバル構造体 tm にセットする
    time_t t = makeTime(tm); // time_t はUnix時間（2038年問題あり）
    t += 13; // コンパイルから実行までのオフセット秒
    return t; // システム時刻を返す
  }
}

// __TIME__パース用関数
bool UTILITIES::getTime(const char *str) {
  int Hour, Min, Sec;
 
  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour   = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}
 
// __DATE__パース用関数
bool UTILITIES::getDate(const char *str) {
  const char *monthName[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
  };
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;
 
  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}
