#include "Arduino.h"
#include "UTILITIES.h"
#include <Time.h>
#include <DS1307RTC.h> // https://www.pjrc.com/teensy/td_libs_DS1307RTC.html

// コンストラクタ（初期化処理）
UTILITIES::UTILITIES() {
}

void UTILITIES::initSerial() { // Arduino IDEのシリアルコンソールに送信
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
}

void UTILITIES::initTime() {   // システム時刻をRTCの精密な時刻に設定する
  Serial.println(F("Sync to RTC..."));
  time_t t_old = getRTC();
  time_t t_now;
  // RTCから秒単位しか取得できない。1秒ぐらい待つとRTCの時刻
  // が変わるはずなので、ループでRTCの時刻が変わるまで待って、
  // 変わった瞬間にシステムの時刻に設定する
  while ( t_old == t_now ) {
    Serial.println(F("sync..."));
    t_now = getRTC();
  }
  setTime(t_now); // システム時刻を設定する
}

time_t UTILITIES::getRTC() {
  tmElements_t tm; 
  // RTCからの読み取り処理
  if (RTC.read(tm)) {
    // RTCから読み込み成功
    time_t t = makeTime(tm);
    return t;
  } 
  else {
    // RTCから読み取り失敗
    if (RTC.chipPresent()) {
      // RTCが無いか動いていない
      Serial.println(F("RTC is stopped.  Please run the SetTime"));
      Serial.println(F("example to initialize the time and begin running."));
      Serial.println();
    } 
    else {
      // RTC読み込みエラー
      Serial.println(F("RTC read error!  Please check the circuitry."));
      Serial.println();
    }
    delay(1000);
  } 
}

void UTILITIES::adjustCompiledTime() { // RTCをPCの時刻に合わせる
  // コンパイラが__DATE__と__TIME__を定数に置換する
  if (getDate(__DATE__) && getTime(__TIME__)) {
    // getDate/getTimeは定数文字列をグローバル構造体 tm にセットする
    time_t t = makeTime(tm); // time_t はUnix時間（2038年問題あり）
    t += 13; // コンパイルから実行までのオフセット秒
    setTime(t); // システム時刻を設定する
    if (RTC.set(t)) {  // RTCに時刻を設定する
      Serial.println(F("Adjust from compiled time."));
      Serial.println(F("Write RTC."));
    }
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
