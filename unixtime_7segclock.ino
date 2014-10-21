/*
  Arduino用Unix時刻時計 unixtime_7segclock.ino
  https://github.com/CLCL/unixtime_7segclock
*/

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h> // https://www.pjrc.com/teensy/td_libs_DS1307RTC.html
#include <FiniteStateMachine.h> // http://arduino-info.wikispaces.com/HAL-LibrariesUpdates
#include <Button.h> // http://arduino-info.wikispaces.com/HAL-LibrariesUpdates
#include <LED.h>    // http://arduino-info.wikispaces.com/HAL-LibrariesUpdates

const int TICK = 100; // システムの1TICKあたりの時間 100ms
const int SEGLED = 0x60; // 北ラボ製 10桁I2C 7セグ表示機用I2Cアドレス
const int digits[] = { // 7セグメント点灯パターン
  0x3f, 0x06, 0x5b, 0x4f, 0x66, // 0～5
  0x6d, 0x7d, 0x27, 0x7f, 0x6f, // 6～9
};

// FiniteStateMachine 有限状態機械ライブラリ（State, FSM）
// State:状態（モード）の設定
// UTIME=Unix時間表示モード、CLOCK=時計（MMHHhhmmss）表示モード
State UTIME = State(S_UTIME_enter, S_UTIME_update, S_UTIME_exit);
State CLOCK = State(S_CLOCK_enter, S_CLOCK_update, S_CLOCK_exit);
State SETY  = State(S_SETY_enter,  S_SETY_update,  S_SETY_exit);
State SETM  = State(S_SETM_enter,  S_SETM_update,  S_SETM_exit);
State SETD  = State(S_SETD_enter,  S_SETD_update,  S_SETD_exit);
State SETh  = State(S_SETh_enter,  S_SETh_update,  S_SETh_exit);
State SETm  = State(S_SETm_enter,  S_SETm_update,  S_SETm_exit);
State SETs  = State(S_SETs_enter,  S_SETs_update,  S_SETs_exit);
// FSM:初期状態はUTIME
FSM stateMachine = FSM(UTIME);

// Buttonライブラリ
Button modeButton = Button( 4, PULLUP); // 4ピン
Button setButton  = Button(A3, PULLUP); // A3ピン

// LEDライブラリ
LED stateLED = LED(13); // Arduino本体のパイロットランプLED

// 日付時刻構造体
tmElements_t tm; 
tmElements_t tm2; // 時刻合わせの一時的設定値 

//グローバル変数
unsigned short counter       = 0; // TICKカウンタ
unsigned short counter_blink = 0; // BLINKカウンタ
String str7seg = ""; // 7segLEDの現在の表示文字列

// 初期化
void setup() {
  init7seg();   // 7segLED初期化
  initSerial(); // デバッグモニタ用シリアル初期化
  initTime();   // システム時刻初期化（RTCとシンクロする）
  //adjustCompiledTime(); // RTCに時刻を設定したい時に利用
}

// メインループ
void loop() {
  // 状態にかかわらず共通実行される（FSM前）
  {
    // ビット演算でcounterの値によってシステム時刻をRTCに合わせる
    // 51.2秒ごとにRTCに合わせる
    if ( ( counter & 0x0100 ) == 0x0100 ) {   // tickカウンタ0x1FC～01FF
      if ( ( counter & 0x00FC ) == 0x00FC ) { // 4回合わせる（tick=100ms）
        Serial.println("Check RTC.");
        stateLED.toggle(); // RTCシンクロをパイロットランプで表示
        time_t t_rtc = getRTC();
        time_t t_now = now();
        if ( t_rtc != t_now ) {
          setTime(t_rtc);
          Serial.println("Adjust from RTC.");
        }
      }
    }
  }
  // FSMによる状態遷移分岐（Rooter/Dispatcher）

  // Unix時間表示
  if (stateMachine.isInState(UTIME)) {
    // セットボタンとモードボタン同時押し
    if (setButton.isPressed() && modeButton.uniquePress() ) {
      stateMachine.transitionTo(SETY);
    }
    // モードボタンだけ押したとき
    else if ( modeButton.uniquePress() ) { 
      stateMachine.transitionTo(CLOCK);
    }
  }
  // 日付時刻表示
  else if (stateMachine.isInState(CLOCK)) {
    // セットボタンとモードボタン同時押し
    if (setButton.isPressed() && modeButton.uniquePress() ) {
      stateMachine.transitionTo(SETY);
    }
    // モードボタンだけ押したとき
    else if ( modeButton.uniquePress() ) {
      stateMachine.transitionTo(UTIME);
    }
  }
  // 年設定
  else if (stateMachine.isInState(SETY)) {
    if ( modeButton.uniquePress() ) {
      // セットボタンとモードボタン同時押し
      if (setButton.isPressed() ) {
        stateMachine.transitionTo(UTIME);
      }
      // モードボタンだけ押したとき
      else {
        stateMachine.transitionTo(SETM);
      }
    }
    // セットボタンだけ押したとき
    else if ( setButton.uniquePress() ) {
      tm2.Year++;
      if (tm2.Year > 68) tm2.Year = 0;
      counter_blink = 0;
    }
  }
  // 月設定
  else if (stateMachine.isInState(SETM)) {
    if ( modeButton.uniquePress() ) {
      // セットボタンとモードボタン同時押し
      if (setButton.isPressed() ) {
        stateMachine.transitionTo(UTIME);
      }
      // モードボタンだけ押したとき
      else {
        stateMachine.transitionTo(SETD);
      }
    }
    // セットボタンだけ押したとき
    else if ( setButton.uniquePress() ) {
      tm2.Month++;
      if (tm2.Month > 12) tm2.Month = 1;
      counter_blink = 0;
    }
  }
  // 日設定
  else if (stateMachine.isInState(SETD)) {
    if ( modeButton.uniquePress() ) {
      // セットボタンとモードボタン同時押し
      if (setButton.isPressed() ) {
        stateMachine.transitionTo(UTIME);
      }
      // モードボタンだけ押したとき
      else {
        stateMachine.transitionTo(SETh);
      }
    }
    // セットボタンだけ押したとき
    else if ( setButton.uniquePress() ) {
      tm2.Day++;
      int dayofmonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31,
      };
      if ( (tm2.Year + 1970) % 4 == 0 ) { 
        dayofmonth[1]++; // 1976年～2038年の閏日分
      }
      int d = dayofmonth[tm2.Month -1]; // 月の日数
      if (tm2.Day > d) tm2.Day = 1;
      counter_blink = 0;
    }
  }
  // 時設定
  else if (stateMachine.isInState(SETh)) {
    if ( modeButton.uniquePress() ) {
      // セットボタンとモードボタン同時押し
      if (setButton.isPressed() ) {
        stateMachine.transitionTo(UTIME);
      }
      // モードボタンだけ押したとき
      else {
        stateMachine.transitionTo(SETm);
      }
    }
    // セットボタンだけ押したとき
    else if ( setButton.uniquePress() ) {
      tm2.Hour++;
      if (tm2.Hour > 23) tm2.Hour = 0;
      counter_blink = 0;
    }
  }
  // 分設定
  else if (stateMachine.isInState(SETm)) {
    if ( modeButton.uniquePress() ) {
      // セットボタンとモードボタン同時押し
      if (setButton.isPressed() ) {
        stateMachine.transitionTo(UTIME);
      }
      // モードボタンだけ押したとき
      else {
        stateMachine.transitionTo(SETs);
      }
    }
    // セットボタンだけ押したとき
    else if ( setButton.uniquePress() ) {
      tm2.Minute++;
      if (tm2.Minute > 59) tm2.Minute = 0;
      counter_blink = 0;
    }
  }
  // 秒設定
  else if (stateMachine.isInState(SETs)) {
    if ( modeButton.uniquePress() ) {
      // セットボタンとモードボタン同時押し
      if (setButton.isPressed() ) {
        stateMachine.transitionTo(UTIME);
      }
      // モードボタンだけ押したとき
      else {
        time_t t = makeTime(tm2); // time_t はUnix時間（2038年問題あり）
        setTime(t); // システム時刻を設定する
        if (RTC.set(t)) {  // RTCに時刻を設定する
          Serial.println("Write RTC.");
        }
        stateMachine.transitionTo(UTIME);
      }
    }
    // セットボタンだけ押したとき
    else if ( setButton.uniquePress() ) {
      tm2.Second++;
      if (tm2.Second > 59) tm2.Second = 0;
      counter_blink = 0;
    }
  }
  stateMachine.update();
  // 状態にかかわらず共通実行される（FSM後）
  {
    delay(TICK); // const TICK = 100(ms)
    counter++;
  }
}

// FiniteStateMachine 状態ごとの振る舞い

// UTIME UNIX時間表示
void S_UTIME_enter() {
  stateLED.off();  // Pin13(LED)を消灯
}
void S_UTIME_update() {
  draw7seg( String( now() ));
}
void S_UTIME_exit() {
  // NOOP
}

// S_CLOCK 時刻表示
void S_CLOCK_enter() {
  stateLED.on();  // Pin13(LED)を点灯
}
void S_CLOCK_update() {
  char str1[10];
  sprintf( str1, "%02d%02d%02d%02d%02d",
    month(), day(), hour(), minute(), second()
  );
  draw7seg( str1 );
}
void S_CLOCK_exit() {
  // NOOP
}

// S_SETY 時刻合わせ
void S_SETY_enter() {
  stateLED.on();  // Pin13(LED)を消灯
  tm2 = tm;
}
void S_SETY_update() {
  char str1[10], str2[10];
  sprintf( str1, "%4d      ",  tm2.Year + 1970 );
  sprintf( str2, "          " );
  draw7seg_blink( str1, str2 );
}
void S_SETY_exit() {
  // NOOP
}

// S_SETM 時刻合わせ
void S_SETM_enter() {
  // NOOP
}
void S_SETM_update() {
  char str1[10], str2[10];
  sprintf( str1, "%4d%02d    ", tm2.Year + 1970, tm2.Month );
  sprintf( str2, "%4d      ",   tm2.Year + 1970 );
  draw7seg_blink( str1, str2 );
}
void S_SETM_exit() {
  // NOOP
}

// S_SETD 時刻合わせ
void S_SETD_enter() {
  // NOOP
}
void S_SETD_update() {
  char str1[10], str2[10];
  sprintf( str1, "%4d%02d%02d  ", tm2.Year + 1970, tm2.Month, tm2.Day );
  sprintf( str2, "%4d%02d    ",   tm2.Year + 1970 ,tm2.Month );
  draw7seg_blink( str1, str2 );
}
void S_SETD_exit() {
  // NOOP
}

// S_SETh 時刻合わせ
void S_SETh_enter() {
  tm2.Hour = hour();
}
void S_SETh_update() {
  char str1[10], str2[10];
  sprintf( str1, "    %02d    ", tm2.Hour );
  sprintf( str2, "          " );
  draw7seg_blink( str1, str2 );
}
void S_SETh_exit() {
  // NOOP
}

// S_SETm 時刻合わせ
void S_SETm_enter() {
  tm2.Minute = minute();
}
void S_SETm_update() {
  char str1[10], str2[10];
  sprintf( str1, "    %02d%02d  ", tm2.Hour, tm2.Minute );
  sprintf( str2, "    %02d    ",   tm2.Hour );
  draw7seg_blink( str1, str2 );
}
void S_SETm_exit() {
  // NOOP
}

// S_SETm 時刻合わせ
void S_SETs_enter() {
  tm2.Second = second();
}
void S_SETs_update() {
  char str1[10], str2[10];
  sprintf( str1, "    %02d%02d%02d", tm2.Hour, tm2.Minute, tm2.Second );
  sprintf( str2, "    %02d%02d  ",   tm2.Hour, tm2.Minute );
  draw7seg_blink( str1, str2 );
}
void S_SETs_exit() {
  // NOOP
}


// サブルーチン集

void initTime() {   // システム時刻をRTCの精密な時刻に設定する
  Serial.println("Sync to RTC...");
  time_t t_old = getRTC();
  time_t t_now;
  // RTCから秒単位しか取得できない。1秒ぐらい待つとRTCの時刻
  // が変わるはずなので、ループでRTCの時刻が変わるまで待って、
  // 変わった瞬間にシステムの時刻に設定する
  while ( t_old == t_now ) {
    Serial.println("sync...");
    t_now = getRTC();
  }
  setTime(t_now); // システム時刻を設定する
}

void adjustCompiledTime() { // RTCをPCの時刻に合わせる
  // コンパイラが__DATE__と__TIME__を定数に置換する
  if (getDate(__DATE__) && getTime(__TIME__)) {
    // getDate/getTimeは定数文字列をグローバル構造体 tm にセットする
    time_t t = makeTime(tm); // time_t はUnix時間（2038年問題あり）
    t += 11; // コンパイルから実行までのオフセット秒
    setTime(t); // システム時刻を設定する
    if (RTC.set(t)) {  // RTCに時刻を設定する
      Serial.println("Write RTC.");
    }
  }
}

time_t getRTC() {
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
      Serial.println("RTC is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } 
    else {
      // RTC読み込みエラー
      Serial.println("RTC read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(1000);
  } 
}

void initSerial() { // Arduino IDEのシリアルコンソールに送信
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
}

void init7seg() { // 7セグメント表示器の初期化
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

void draw7seg(String str) {
  // 現在表示文字列と同じだったら処理しない
  if ( str7seg.equals(str) ) return;
  str7seg = str;
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
  Serial.println(str);
}

void draw7seg_blink(String str1, String str2) { // str1とstr2を交互に表示
  switch (  counter_blink & 0x08 ) { // 800msごと
    case 0:  draw7seg( str1 ); break;
    default: draw7seg( str2 );
  }
  counter_blink++;
}

// __TIME__パース用関数
bool getTime(const char *str) {
  int Hour, Min, Sec;
 
  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour   = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}
 
// __DATE__パース用関数
bool getDate(const char *str) {
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

