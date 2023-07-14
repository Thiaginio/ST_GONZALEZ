//Nicolás Golonbek y Simón Leyell

#include "U8g2lib.h"
#include "DHT.h"
#include "Adafruit_Sensor.h"
#include <Preferences.h>

Preferences preferences;


U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);


#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


int mE = 0;
#define SCREEN1 0
#define SCREEN2 1
#define CHANGE1 2
#define CHANGE2 3
#define SUMAR_UMBRAL 4
#define RESTAR_UMBRAL 5


unsigned int tumbral;


#define BTN_PIN1 35
#define BTN_PIN2 34


void setup() {


  u8g2.begin();


  dht.begin();


  pinMode(BTN_PIN1, INPUT_PULLUP);
  pinMode(BTN_PIN2, INPUT_PULLUP);


  preferences.begin("my-app", false);
  tumbral = preferences.getUInt("UMBRAL", 0);
}

void loop() {
  float t = dht.readTemperature();

  char temp[5];
  sprintf(temp, "%2.1f", t);

  char umbral[2];
  sprintf(umbral, "%i", tumbral);

  switch (mE) {
    case SCREEN1:
      u8g2.setFont(u8g2_font_cupcakemetoyourleader_tr);
      u8g2.drawStr(5, 15, "Temperatura: ");
      u8g2.drawStr(5, 30, temp);
      u8g2.drawStr(60, 30, "C");
      u8g2.drawStr(5, 45, "T. Umbral:");
      u8g2.drawStr(5, 60, umbral);
      u8g2.sendBuffer();

      if (digitalRead(BTN_PIN1) == LOW && digitalRead(BTN_PIN2) == LOW) {
        mE = CHANGE1;
      }
      break;


    case SCREEN2:
      u8g2.setFont(u8g2_font_cupcakemetoyourleader_tr);
      u8g2.drawStr(0, 30, "T. Umbral: ");
      u8g2.drawStr(0, 50, umbral);
      u8g2.sendBuffer();

      if (digitalRead(BTN_PIN1) == LOW && digitalRead(BTN_PIN2) == LOW) {
        mE = CHANGE2;
      }

      if (digitalRead(BTN_PIN1) == LOW && digitalRead(BTN_PIN2) == HIGH) {
        mE = SUMAR_UMBRAL;
      }

      if (digitalRead(BTN_PIN1) == HIGH && digitalRead(BTN_PIN2) == LOW) {
        mE = RESTAR_UMBRAL;
      }

      break;


    case CHANGE1:
      u8g2.clearBuffer();
      u8g2.sendBuffer();
      if (digitalRead(BTN_PIN1) == HIGH && digitalRead(BTN_PIN2) == HIGH) {
        mE = SCREEN2;
      }
      break;


    case CHANGE2:
      u8g2.clearBuffer();
      u8g2.sendBuffer();
      if (digitalRead(BTN_PIN1) == HIGH && digitalRead(BTN_PIN2) == HIGH) {
        preferences.putUInt("UMBRAL", tumbral);
        preferences.end();
        mE = SCREEN1;
      }
      break;

    case SUMAR_UMBRAL:
      if (digitalRead(BTN_PIN1) == HIGH && digitalRead(BTN_PIN2) == HIGH) {
        tumbral++;
        u8g2.clearBuffer();
        u8g2.sendBuffer();
        mE = SCREEN2;
      }
      break;

    case RESTAR_UMBRAL:
      if (digitalRead(BTN_PIN1) == HIGH && digitalRead(BTN_PIN2) == HIGH) {
        tumbral--;
        u8g2.clearBuffer();
        u8g2.sendBuffer();
        mE = SCREEN2;
      }
      break;
  }
}

void stateMachine() {
}
