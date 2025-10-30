int buzzer = 8;

// Buttons (wire to GND, using INPUT_PULLUP)
const int buttonPinB = 3;   // Blue button
const int buttonPinR = 13;  // Red  button

// RGB LED pins (active HIGH)
#define BLUE 6
#define GREEN 5
#define RED 9

// Team indicator LEDs
#define ledR 12
#define ledB 2

bool stopped = false;
int winner = 0; // 1 = Blue, 2 = Red

// ---------- functions ----------
bool pressed(int pin) {
  if (digitalRead(pin) == LOW) { delay(15); return digitalRead(pin) == LOW; } //to make sure both of the buttons don't get presssed at the same time
  
  return false;
}

void setRGB(bool r, bool g, bool b) {   // makes it easier to switch the colours
  if (r)
    digitalWrite(RED, HIGH);
  else
    digitalWrite(RED, LOW);

  if (g)
    digitalWrite(GREEN, HIGH);
  else
    digitalWrite(GREEN, LOW);

  if (b)
    digitalWrite(BLUE, HIGH);
  else
    digitalWrite(BLUE, LOW);
}

void allOff() {
  setRGB(false,false,false);
  digitalWrite(ledB, LOW);
  digitalWrite(ledR, LOW);
}

void beep(int ms) {  
  unsigned long t0 = millis();
  while (millis() - t0 < (unsigned long)ms) {
    digitalWrite(buzzer, HIGH); delay(1);
    digitalWrite(buzzer, LOW);  delay(1);
  }
}


void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(buttonPinB, INPUT_PULLUP);
  pinMode(buttonPinR, INPUT_PULLUP);

  pinMode(ledB, OUTPUT);
  pinMode(ledR, OUTPUT);

  allOff();
}

void loop() {
  if (stopped) return;

  // Initiates a 3-second countdown: RED -> YELLOW -> GREEN

  // second 1: RED
  allOff();
  setRGB(true,false,false);
  beep(100); delay(900);

  // second 2: YELLOW (red + green)
    setRGB(true,true,false);
  beep(100); delay(900);

  // second 3: GREEN
    setRGB(false,true,false);
  beep(100); delay(900);

  // "GO": WHITE (all on) + team LEDs on
  setRGB(true,true,true);
  digitalWrite(ledB, HIGH);
  digitalWrite(ledR, HIGH);

  // Waiting window: first to press wins
  for (int i = 0; i <= 500; i++) {
    // soft ticking on the buzzer while waiting
    digitalWrite(buzzer, HIGH); delay(1);
    digitalWrite(buzzer, LOW);  delay(1);

    if (pressed(buttonPinB)) {
      winner = 1;        // Blue wins
      stopped = true;
      digitalWrite(ledR, LOW);   // other team off

      // Show BLUE, blink a bit
      setRGB(false,false,false);
      for (int k = 0; k < 50; k++) {
        digitalWrite(BLUE, HIGH); delay(10);
        digitalWrite(BLUE, LOW);  delay(10);
      }
      setRGB(false,false,false);
      break;
    }

    if (pressed(buttonPinR)) {
      winner = 2;        // Red wins
      stopped = true;
      digitalWrite(ledB, LOW);   // other team off

      // Show RED, blink a bit
      setRGB(false,false,false);
      for (int k = 0; k < 50; k++) {
        digitalWrite(RED, HIGH); delay(10);
        digitalWrite(RED, LOW);  delay(10);
      }
      setRGB(false,false,false);
      break;
    }

    delay(5);
  }

  if (stopped) {
    // confirmation beep then freeze forever
    beep(400);
    while (true) {  }
  }

  // optional idle delay if nobody pressed in time
  delay(10000);
}
