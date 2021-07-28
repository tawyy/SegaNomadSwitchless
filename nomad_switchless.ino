#define VIDEOMODE_PIN A1 // JP1/JP2
#define LANGUAGE_PIN A2 // JP3/JP4
#define LED_BLINK A3 //common cathode/gnd, replace actual led "low battery" and wire the third pin of the led to LED_BLINK pin

// each of these pin are active low
#define RESET_OUT_PIN A0
#define MD_BTN_START 3
#define MD_BTN_DOWN 4
#define MD_BTN_LEFT 5
#define MD_BTN_RIGHT 6
#define MD_BTN_A 7
#define MD_BTN_B 8

//#define TRIGGER_COMBO (MD_BTN_START+MD_BTN_A)

#define RESET_COMBO MD_BTN_B

#define EUR_COMBO MD_BTN_DOWN
#define USA_COMBO MD_BTN_RIGHT
#define JAP_COMBO MD_BTN_LEFT

// 6 entries and last_index is 5 -> for testing purposes
int button_pins[6] = { MD_BTN_START, MD_BTN_DOWN, MD_BTN_LEFT, MD_BTN_RIGHT, MD_BTN_A, MD_BTN_B };

unsigned long last_index = 5; // last index

int output_pins[3] = { RESET_OUT_PIN, VIDEOMODE_PIN, LANGUAGE_PIN };

unsigned long debounceDuration = 100; // millis
unsigned long lastTimeButtonStateChanged = 0; // initialisation

// longueur du contact qui fait reset sur le pin 17
const unsigned long RESET_LEN = 350U;

word lastButtonState = "";

void setup() {
    for (int i = 0; i <= last_index; i++)
    {
      pinMode(button_pins[i], INPUT);
    }
    for (int j = 0; j <= 2; j++)
    {
      pinMode(output_pins[j], OUTPUT);
      digitalWrite (output_pins[j], HIGH);
    }
    pinMode(LED_BUILTIN, OUTPUT); 
    pinMode(LED_BLINK, OUTPUT);
    digitalWrite (LED_BLINK, LOW);
    // force 50hz, the arduino does not respond after the main menu
    if(digitalRead(MD_BTN_START) == LOW){
      digitalWrite (VIDEOMODE_PIN, LOW);
    }
    // force JAP
    if(digitalRead(MD_BTN_A) == LOW){
      digitalWrite (LANGUAGE_PIN, LOW);
    }

}

void led_blinker (int j) {
  for (byte i = 0; i < j; ++i) {
    digitalWrite (LED_BUILTIN, HIGH);
    digitalWrite (LED_BLINK, HIGH);
    delay (500);
    digitalWrite (LED_BUILTIN, LOW);
    digitalWrite (LED_BLINK, HIGH);
    delay (500);
  }
}

word read_pad () {
  word buttonscombo = 0;
    for (int i = 0; i <= last_index; i++)
    {
      byte buttonHere = digitalRead(button_pins[i]);
        if (buttonHere == LOW)
        {
          // we are using bit to be able to have 2+4 != 1+5 bit2+bit4 != bit1+bit5 / 4+16 != 2+32 / 20 != 33
          buttonscombo += bit(button_pins[i]);
        }
    }   
  return buttonscombo;
}

void loop() {
  if (millis() - lastTimeButtonStateChanged > debounceDuration) {
    word buttonState = read_pad();
    
    if (buttonState != lastButtonState) {
      lastTimeButtonStateChanged = millis();
      lastButtonState = buttonState;

      // Reset
      if (buttonState == (bit(MD_BTN_START) + bit(MD_BTN_A) + bit(RESET_COMBO))) {
        digitalWrite (RESET_OUT_PIN, LOW);
        delay (RESET_LEN);
        digitalWrite (RESET_OUT_PIN, HIGH);
        led_blinker(1);
      }
      
      // EURO
      if (buttonState == (bit(MD_BTN_START) + bit(MD_BTN_A) + bit(EUR_COMBO))) {
        digitalWrite (VIDEOMODE_PIN, LOW);
        digitalWrite (LANGUAGE_PIN, HIGH);
        led_blinker(3);
      }

      // USA
      if (buttonState == (bit(MD_BTN_START) + bit(MD_BTN_A) + bit(USA_COMBO))) {
        digitalWrite (VIDEOMODE_PIN, HIGH);
        digitalWrite (LANGUAGE_PIN, HIGH);
        led_blinker(1);
      }

      // JAP
      if (buttonState == (bit(MD_BTN_START) + bit(MD_BTN_A) + bit(JAP_COMBO))) {
        digitalWrite (VIDEOMODE_PIN, HIGH);
        digitalWrite (LANGUAGE_PIN, LOW);
        led_blinker(2);
      }
    }
  }
}
