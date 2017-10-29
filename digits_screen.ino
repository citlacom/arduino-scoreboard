#if !defined(SCREEN_TYPE)
  #error SCREEN_TYPE is not defined
#endif

#if SCREEN_TYPE == 2

  #include <LedControl.h>
  
  // Digit screen.
  #define DATA_PIN A2
  #define CS_PIN A1
  #define CLOCK_PIN A0
  
  LedControl lc = LedControl(DATA_PIN, CLOCK_PIN, CS_PIN, 1);
  
  void init_screen() {
    lc.shutdown(0, false);
    lc.setIntensity(0,8);
    screen_display_result();
  }
  
  void screen_display_result() {
    if (winner_player != 0) {
      T.StopTimer();
    }
    
    lc.setChar(0, 7, player1_counter, false);
    lc.setChar(0, 6, '-', false);
    lc.setChar(0, 5, player2_counter, false);
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
