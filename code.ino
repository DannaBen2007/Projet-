#include "lcdgfx.h"
#include <FastLED.h>

// ---------------- OLED ----------------
DisplaySSD1306_128x64_I2C display(-1);

// ---------------- LED ----------------
#define LED_PIN     4
#define NUM_LEDS    21
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

// ---------------- ULTRASON ----------------
const int trigPin = A3;
const int echoPin = A2;

float duration, distance;

// seuil détection (cm)
const float SEUIL = 7.0;

// état mémoire
bool courrierDetecte = false;

void setup() {

  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // OLED
  display.begin();
  display.fill(0x00);
  display.setFixedFont(ssd1306xled_font6x8);

  display.printFixed(0, 8, "MailBox+", STYLE_BOLD);
  display.printFixed(0, 24, "En attente...", STYLE_NORMAL);

  // LED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {

  // ---------------- MESURE DISTANCE ----------------
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;

  Serial.println(distance);

  // ---------------- LED ----------------
  uint8_t dist_temp = map(distance, 0, 200, 0, 255);

  fill_solid(leds, NUM_LEDS,
             ColorFromPalette(RainbowColors_p, dist_temp, BRIGHTNESS, LINEARBLEND));

  FastLED.show();

  // ---------------- DETECTION ----------------
  if (distance > 0 && distance < SEUIL) {
    courrierDetecte = true;
  }

  // ---------------- OLED ----------------
  display.clear();

  if (courrierDetecte) {

    display.printFixed(0, 20, "Nouveau courrier", STYLE_BOLD);
    display.printFixed(0, 40, "Recuperez-le !", STYLE_NORMAL);

  } else {

    display.printFixed(0, 20, "MailBox+", STYLE_BOLD);
    display.printFixed(0, 40, "En attente...", STYLE_NORMAL);
  }

  delay(100);
}
