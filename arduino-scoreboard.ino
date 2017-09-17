// Use the https://github.com/bhagman/Tone library to play songs
// using a passive buzzer.
#include <Tone.h>
// Use the https://github.com/adafruit/Adafruit_SSD1306
// and https://github.com/adafruit/Adafruit-GFX-Library
// libraries to display results to OLED screen.
#include <Adafruit_SSD1306.h>

// BUZZER
#define BUZZER_PIN 13
// Red button
#define BUTTON_PLAYER1 1
// Yellow button
#define BUTTON_PLAYER2 2
// Player leds size.
#define LEDS_SIZE 5
// Variables for song play.
#define OCTAVE_OFFSET 0
// Screen OLED variables.
#define OLED_RESET A0
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

// Notes array required by Tone library.
int notes[] = { 0,
  NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
  NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
  NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
  NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};

// Available songs to play when goal or win events occurs.
char *songs[] = {
  (char*)"smb:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6",
  (char*)"The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6",
  (char*)"Smurfs:d=32,o=5,b=200:4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#",
};

// Variables to control state of button1 state and player1 score.
int button1_status = 0;
int button1_last_status = 0;
int player1_counter = 0;
int player1_leds[LEDS_SIZE] = {3, 4, 5, 6, 7};

// Variables to control state of button2 state and player2 score.
int button2_status = 0;
int button2_last_status = 0;
int player2_counter = 0;
int player2_leds[LEDS_SIZE] = {8, 9, 10, 11, 12};

// Winer player record, when game is finished should be 1 or 2.
int winner_player = 0;

// Unsigned longs because the time is milliseconds.
unsigned long button1_debounce_time = 0;
unsigned long button2_debounce_time = 0;
// The debounce time to determine button activation.
unsigned int debounce_delay = 50;

int reading1;
int reading2;

// Initialize Tone object.
Tone tone1;
// Initialize Adafruit_SSD1306 object.
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  pinMode(BUTTON_PLAYER1, INPUT);
  pinMode(BUTTON_PLAYER2, INPUT);
  // Initialize buzzer to Tone.
  tone1.begin(BUZZER_PIN);

  // Initialize player1 leds pins as outputs.
  for (int i = 0; i < LEDS_SIZE; i++) {
    pinMode(player1_leds[i], OUTPUT);
    pinMode(player2_leds[i], OUTPUT);
  }

  // Initialize screen with the I2C addr 0x3C (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Initialize screen.
  screen_display_result();
}

void loop() {
  int reading1 = digitalRead(BUTTON_PLAYER1);
  int reading2 = digitalRead(BUTTON_PLAYER2);

  // Reset the debouncing timer at any state change.
  if (button1_status != button1_last_status) {
    button1_debounce_time = millis();
  }

  if (button2_status != button2_last_status) {
    button2_debounce_time = millis();
  }

  // Current state longer than the debounce so take as the actual state.
  if ((millis() - button1_debounce_time) > debounce_delay) {
    button1_status = reading1;
    // If the button state has changed to pressed.
    if (button1_status != button1_last_status && button1_status == HIGH) {
      // Ensure do not exceed player leds size.
      if (player1_counter < LEDS_SIZE && winner_player == 0) {
        player1_counter++;

        // Mark current player score on leds.
        for (int i = 0; i < player1_counter; i++) {
          digitalWrite(player1_leds[i], HIGH);
        }
      }

      evaluate_winner();

      if (winner_player == 0) {
        // Play goal song.
        play_rtttl(songs[0]);
      }
    }
  }

  // Current state longer than the debounce so take as the actual state.
  if ((millis() - button2_debounce_time) > debounce_delay) {
    button2_status = reading2;
    // If the button state has changed to pressed.
    if (button2_status != button2_last_status && button2_status == HIGH) {
      // Ensure do not exceed player leds size.
      if (player2_counter < LEDS_SIZE && winner_player == 0) {
        player2_counter++;

        // Mark current player score on leds.
        for (int i = 0; i < player2_counter; i++) {
          digitalWrite(player2_leds[i], HIGH);
        }
      }

      evaluate_winner();

      if (winner_player == 0) {
        // Play goal song.
        play_rtttl(songs[1]);
      }
    }
  }

  // Save the reading for next time through the loop.
  button1_last_status = reading1;
  button2_last_status = reading2;
}

// Evaluate if there is a winner and if that case turn off loser leds
// and plays the end game over song.
void evaluate_winner() {
  int *loser_pins;
  // Display current result in screen.
  screen_display_result();

  if (player1_counter == LEDS_SIZE) {
    loser_pins = player2_leds;
    winner_player = 1;
  }

  if (player2_counter == LEDS_SIZE) {
    loser_pins = player1_leds;
    winner_player = 2;
  }

  // Play end game sound if there is a winner.
  if (winner_player != 0) {
    // Turn off the loser pins.
    if (loser_pins) {
      for (int i = 0; i < LEDS_SIZE-1; i++) {
        digitalWrite(loser_pins[i], LOW);
      }
    }

    play_rtttl(songs[2]);
  }
}

#define isdigit(n) (n >= '0' && n <= '9')

// Play sound Ring Tone Transfer Language processing
// function, copied from Tone library RTTTL example.
// https://en.wikipedia.org/wiki/Ring_Tone_Transfer_Language
void play_rtttl(char *p) {
  byte default_dur = 4;
  byte default_oct = 6;
  int bpm = 63;
  int num;
  long wholenote;
  long duration;
  byte note;
  byte scale;

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd') {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p)) {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  // get default octave
  if(*p == 'o') {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  // get BPM
  if(*p == 'b') {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p)) {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  // BPM usually expresses the number of quarter notes per minute
  // this is the time for whole note (in milliseconds)
  wholenote = (60 * 1000L / bpm) * 4;

  // now begin note loop
  while(*p) {
    // first, get note duration, if available
    num = 0;
    while(isdigit(*p)) {
      num = (num * 10) + (*p++ - '0');
    }

    // we will need to check if we are a dotted note after
    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;

    // now get the note
    note = 0;

    switch(*p) {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }

    p++;

    // now, get optional '#' sharp
    if(*p == '#') {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.') {
      duration += duration/2;
      p++;
    }

    // now, get scale
    if(isdigit(*p)) {
      scale = *p - '0';
      p++;
    }
    else {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      // skip comma for next note (or we may be at the end)
      p++;

    // now play the note
    if(note) {
      tone1.play(notes[(scale - 4) * 12 + note]);
      delay(duration);
      tone1.stop();
    }
    else {
      delay(duration);
    }
  }
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
