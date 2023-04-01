#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  4, 0,
  false, false, false,     // No X, Y and Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);

// piezo pins and buttons
#define pata_pin 21
#define don_pin 20
#define pon_pin 19
#define chaka_pin 18

/*
PIEZO PINS
PATA 19
DON 18
PON 17
CHAKA 16
*/



// piezo velocity threshlold
#define vel_th 250

// single-tap flags for pata & chaka
bool pata_tap = false;
bool chaka_tap = false;

// general timer and two timers for double-tapped drums
unsigned long timer = 0;
unsigned long pata_timer = 0;
unsigned long chaka_timer = 0;


void setup() {
  Serial.begin(9600);
  pinMode(pata_pin, INPUT);
  pinMode(pon_pin, INPUT);
  pinMode(chaka_pin, INPUT);
  pinMode(don_pin, INPUT);
  Joystick.begin();
}

void loop() {
  if (millis() - timer > 100) {
    int pata = analogRead(pata_pin);
    int chaka = analogRead(chaka_pin);
    int don = analogRead(don_pin);
    int pon = analogRead(pon_pin);
    // PATA
    // double tap check
    if (pata_tap == true) {
      Joystick.releaseButton(3);
      if (millis() - pata_timer > 400) {
        pata_tap = false;
        timer = millis();

        Joystick.pressButton(2);

        Serial.println("PATA FAIL!");

      } else if (pata > vel_th) {
        // PATA SUCCESS
        pata_tap = false;
        timer = millis();

        Serial.print("TA! ");
        Serial.println(pata);
      }
    // first pata tap
    } else if (pata > vel_th) {
      timer = millis();
      pata_timer = millis();
      pata_tap = true;

      Joystick.pressButton(3);

      Serial.print(pata);
      Serial.print(" PA-");
    }
    // CHAKA
    // double tap check
    else if (chaka_tap == true) {
      Joystick.releaseButton(0);
      if (millis() - chaka_timer > 400) {
        chaka_tap = false;
        timer = millis();

        Joystick.pressButton(2);

        Serial.println("CHAKA FAIL!");

      } else if (chaka > vel_th) {
        // CHAKA SUCCESS
        chaka_tap = false;
        timer = millis();

        Serial.print("KA! ");
        Serial.println(chaka);
      }
    // first chaka tap
    } else if (chaka > vel_th) {
      timer = millis();
      chaka_timer = millis();
      chaka_tap = true;

      Joystick.pressButton(0);

      Serial.print(chaka);
      Serial.print(" CHA-");
    }
    // PON
    else if (pon > vel_th) {
      timer = millis();

      Joystick.pressButton(1);

      Serial.print(pon);
      Serial.println(" PON!");
    }
    // DON
    else if (don > vel_th) {
      timer = millis();

      Joystick.pressButton(2);

      Serial.print(don);
      Serial.println(" DON!");
    } else {
      Joystick.releaseButton(0);
      Joystick.releaseButton(1);
      Joystick.releaseButton(2);
      Joystick.releaseButton(3);
    }
  }
}
