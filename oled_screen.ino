#if !defined(SCREEN_TYPE)
  #error SCREEN_TYPE is not defined
#endif

#if SCREEN_TYPE == 1

  // Use the https://github.com/adafruit/Adafruit_SSD1306
  // and https://github.com/adafruit/Adafruit-GFX-Library
  // libraries to display results to OLED screen.
  #include <Adafruit_SSD1306.h>
  
  // Screen OLED variables.
  #define OLED_RESET A3
  #define NUMFLAKES 10
  #define XPOS 0
  #define YPOS 1
  #define DELTAY 2
  
  // Initialize Adafruit_SSD1306 object.
  Adafruit_SSD1306 display(OLED_RESET);
  
  void init_screen() {
    // Initialize screen with the I2C addr 0x3C (for the 128x64)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
    // Initialize screen.
    screen_display_result();
  }
  
  // Display the score at OLED screen.
  void screen_display_result() {
    char *label;
  
    if (winner_player == 0) {
      label = "MARCADOR";
    }
    else {
      label = "FINALIZADO";
    }
  
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(display.width()/3, 0);
    display.println(label);
    display.drawRect(5, 10, display.width()-5, display.height()-10, WHITE);
  
    // Display score.
    display.setTextSize(2);
    display.setCursor((display.width()/4), 13);
    display.print(player1_counter);
    display.setCursor((display.width()/2), 13);
    display.print("-");
    display.setCursor((display.width()/4) * 3, 13);
    display.print(player2_counter);
    display.display();
  }

#endif
