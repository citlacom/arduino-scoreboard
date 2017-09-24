#include <LedControl.h>
#include <CountUpDownTimer.h>

#define dataPin  5
#define csPin 6
#define clockPin  7

LedControl lc = LedControl(dataPin, clockPin, csPin, 1);
CountUpDownTimer T(UP, HIGH);

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0,8);
  T.StartTimer();
}

void loop() {
  T.Timer();
  lc.setChar(0, 5, 0, false);
  lc.setChar(0, 6, '-', false);
  lc.setChar(0, 7, 0, false);

  if (T.TimeHasChanged()) {
    printSeconds(T.ShowSeconds());
  }
}

void printSeconds(int v) {
  int ones;
  int tens;
  int hundreds;
  int thousands;

  ones = v%10;
  v = v/10;
  tens = v%10;
  v = v/10;
  hundreds = v%10;
  thousands = v/10;

  //Now print the number digit by digit
  lc.setDigit(0, 3, (byte)thousands, false);
  lc.setDigit(0, 2, (byte)hundreds, false);
  lc.setDigit(0, 1, (byte)tens, false);
  lc.setDigit(0, 0, (byte)ones, false);
}
