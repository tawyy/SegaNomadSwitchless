#define RESET_OUT_PIN A0
#define VIDEOMODE_PIN A2
#define LANGUAGE_PIN A3

#define MD_BTN_START 2
#define MD_BTN_UP 3
#define MD_BTN_DOWN 4
#define MD_BTN_LEFT 5
#define MD_BTN_RIGHT 6
#define MD_BTN_A 7
#define MD_BTN_B 8
#define MD_BTN_C 9

// pour activer les combos, mettre des + 
#define TRIGGER_COMBO (MD_BTN_START + MD_BTN_B)

#define RESET_COMBO (MD_BTN_A + MD_BTN_C)

#define EUR_COMBO MD_BTN_DOWN
#define USA_COMBO MD_BTN_RIGHT
#define JAP_COMBO MD_BTN_LEFT

// 8 entries and index from 0 to 7
int button_pins[8] = { MD_BTN_START, MD_BTN_UP, MD_BTN_DOWN, MD_BTN_LEFT, MD_BTN_RIGHT, MD_BTN_A, MD_BTN_B, MD_BTN_C };
int output_pins[3] = { RESET_OUT_PIN, VIDEOMODE_PIN, LANGUAGE_PIN };

unsigned long debounceDuration = 1000; // millis
unsigned long lastTimeButtonStateChanged = 0; // initialisation

// longueur du contact qui fait reset sur le pin 17
const unsigned long RESET_LEN = 350U;

word lastButtonState = "";

void setup() {
    for (int i = 0; i <= 7; i++)
    {
      pinMode(button_pins[i], INPUT);
    }
    for (int j = 0; j <= 2; j++)
    {
      pinMode(output_pins[j], OUTPUT);
      digitalWrite (output_pins[j], HIGH);
    }
}


word read_pad () {
  word buttonscombo = 0;
    for (int i = 0; i <= 7; i++)
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
      if (buttonState == (bit(TRIGGER_COMBO) + bit(RESET_COMBO))) {
        digitalWrite (RESET_OUT_PIN, LOW);
        delay (RESET_LEN);
        digitalWrite (RESET_OUT_PIN, HIGH);
      }
      
      // EURO
      if (buttonState == (bit(TRIGGER_COMBO) + bit(EUR_COMBO))) {
        digitalWrite (VIDEOMODE_PIN, LOW);
        digitalWrite (LANGUAGE_PIN, HIGH);
      }

      // USA
      if (buttonState == (bit(TRIGGER_COMBO) + bit(USA_COMBO))) {
        digitalWrite (VIDEOMODE_PIN, HIGH);
        digitalWrite (LANGUAGE_PIN, HIGH);
      }

      // JAP
      if (buttonState == (bit(TRIGGER_COMBO) + bit(JAP_COMBO))) {
        digitalWrite (VIDEOMODE_PIN, HIGH);
        digitalWrite (LANGUAGE_PIN, LOW);
      }
// une solution pour toggle des états, 
//      ledState = (ledState == HIGH) ? LOW: HIGH;
//      digitalWrite(VideoPin, ledState);
//        
    }
  }
}
