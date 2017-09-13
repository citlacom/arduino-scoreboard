// Red button
#define BUTTON_PLAYER1 3
// Yellow button
#define BUTTON_PLAYER2 2
// Player leds size.
#define LEDS_SIZE 5

int button1_status = 0;
int button1_last_status = 0;
int player1_counter = 0;
int player1_leds[LEDS_SIZE] = {4, 5, 6, 7, 8};

int button2_status = 0;
int button2_last_status = 0;
int player2_counter = 0;
int player2_leds[LEDS_SIZE] = {19, 18, 17, 16, 15};

// Unsigned longs because the time is milliseconds.
unsigned long button1_debounce_time = 0;
unsigned long button2_debounce_time = 0;
// The debounce time to determine button activation.
unsigned int debounce_delay = 50;

int reading1;
int reading2;

void setup() {
  pinMode(BUTTON_PLAYER1, INPUT);
  pinMode(BUTTON_PLAYER2, INPUT);

  // Initialize player1 leds pins as outputs.
  for (int i = 0; i < LEDS_SIZE; i++) {
    pinMode(player1_leds[i], OUTPUT);
    pinMode(player2_leds[i], OUTPUT);
  }
  
  Serial.begin(9600);
}

void loop() {
  
  int reading1 = digitalRead(BUTTON_PLAYER1);
  int reading2 = digitalRead(BUTTON_PLAYER2);
  //Serial.println(reading);

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
      player1_counter++;
      for (int i = 0; i < player1_counter; i++) {
        Serial.println(player1_leds[i]); 
        digitalWrite(player1_leds[i], HIGH);
      }

      // Avoid multiples counts over few seconds period.
      delay(1000);
    }
  }

 // Current state longer than the debounce so take as the actual state.
  if ((millis() - button2_debounce_time) > debounce_delay) {
    button2_status = reading2;
    // If the button state has changed to pressed.
    if (button2_status != button2_last_status && button2_status == HIGH) {
      player2_counter++;
      for (int i = 0; i < player2_counter; i++) {
        Serial.println(player2_leds[i]); 
        digitalWrite(player2_leds[i], HIGH);
      }

      // Avoid multiples counts over few seconds period.
      delay(5000);
    }
  }

  // Save the reading for next time through the loop.
  button1_last_status = reading1;
  button2_last_status = reading2;
}
