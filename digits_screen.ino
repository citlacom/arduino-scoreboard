#if !defined(SCREEN_TYPE)
  #error SCREEN_TYPE is not defined
#endif

#if SCREEN_TYPE == 2

  #include <LedControl.h>
  #include <CountUpDownTimer.h>
  
  // Digit screen.
  #define DATA_PIN 14
  #define CS_PIN 15
  #define CLOCK_PIN 16
  
  LedControl lc = LedControl(DATA_PIN, CLOCK_PIN, CS_PIN, 1);
  CountUpDownTimer T(UP, HIGH);
  
  void init_screen() {
    lc.shutdown(0, false);
    lc.setIntensity(0,8);
    T.StartTimer();
  }
  
  void screen_display_result() {
    T.Timer();
    lc.setChar(0, 5, 0, false);
    lc.setChar(0, 6, '-', false);
    lc.setChar(0, 7, 0, false);
  
    if (T.TimeHasChanged()) {
      screen_print_timer(T.ShowSeconds());
    }
  }
  
  void screen_print_timer(int v) {
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
  
    // Now print the number digit by digit
    lc.setDigit(0, 3, (byte)thousands, false);
    lc.setDigit(0, 2, (byte)hundreds, false);
    lc.setDigit(0, 1, (byte)tens, false);
    lc.setDigit(0, 0, (byte)ones, false); 
  }

#endif
