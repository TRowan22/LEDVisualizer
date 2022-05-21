/*#include <FastLED.h>

#define NUM_LEDS 160
#define LED_PIN 4

DEFINE_GRADIENT_PALETTE(snowy_palette) {
  0, 200, 204, 0,
  64, 255, 70, 255,
  128, 255, 255, 255,
  191, 255, 70, 255,
  255, 200, 204, 0
};

DEFINE_GRADIENT_PALETTE(greenblue_gp) {
  0, 0, 255, 245,
  46, 0, 21, 255,
  179, 12, 250, 0,
  255, 0, 255, 245
};

DEFINE_GRADIENT_PALETTE (heatmap_gp) {
  0, 0, 0, 0, //black
  128, 255, 0, 0, //red
  200, 130, 126, 0, //bright yellow
  255, 255, 255, 255 //white
};

uint8_t whichPalette = 0;

CRGB leds[NUM_LEDS];
//uint8_t pal_index[NUM_LEDS];

//CRGBPalette16 pal(snowy_palette);
CRGBPalette16 gb = greenblue_gp;
CRGBPalette16 ht = heatmap_gp;

CRGBPalette16 currentPalette(greenblue_gp);
CRGBPalette16 targetPalette(heatmap_gp);

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  FastLED.setCorrection(TypicalPixelString);
  Serial.begin(9600);
}

void loop() {
 //random16_add_entropy(random());

 uint8_t sinBeat = beatsin8(20, 0, NUM_LEDS - 1, 0, 0);
 //uint8_t sinBeat2 = beatsin8(50, 0, NUM_LEDS - 1, 0, 500);

 for (uint8_t i = 0; i < NUM_LEDS; i++) {
  leds[i] = ColorFromPalette(currentPalette, i + sinBeat);
 }

 nblendPaletteTowardPalette(currentPalette, targetPalette, 15);

 switch (whichPalette) {
  case 0:
    targetPalette = heatmap_gp;
    break;
  case 1:
    targetPalette = greenblue_gp;
    break;
 }

 EVERY_N_SECONDS(4) {
   whichPalette = (whichPalette + 1) % 2;
 }

 FastLED.show();

} */

#include <FastLED.h>

#define NUM_LEDS 160
#define LED_PIN 4

CRGB leds[NUM_LEDS];

int strobe = 8;
int reset = 10;
int out = A3;
int levels[7]; 

int starting = NUM_LEDS / 2;

void setup() {
  
  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  FastLED.setCorrection(TypicalPixelString);
  Serial.begin(9600);

  pinMode(strobe, OUTPUT);
  pinMode(reset, OUTPUT);
  pinMode(out, INPUT);

  digitalWrite(reset, LOW);
  digitalWrite(strobe, LOW);
  delay(1);

  digitalWrite(reset, HIGH);
  delay(1);
  digitalWrite(reset, LOW);
  digitalWrite(strobe, HIGH);
  delay(1);

  

  for (uint8_t i = 0; i < starting; i++) {
    leds[i] = CRGB::Red;
    leds[NUM_LEDS - i] = CRGB::Blue;
  }
  FastLED.show();
}

void loop() {
  unsigned long timer = millis();
  //uint8_t beat = beatsin8(20, 0, NUM_LEDS - 1, 0, 0);

  for (int i = 0; i < 7; i++) {
    digitalWrite(strobe, LOW);
    delayMicroseconds(100);
    levels[i] = analogRead (out);
    digitalWrite(strobe, HIGH);
    delayMicroseconds  (100);                    // Delay necessary due to timing diagram  
  }

  int gh = map(levels[0], 0, 1024, 0, (NUM_LEDS / 2));

  
  int difference = starting - gh;
  //Serial.println(difference);
  
  if (difference > 0) {
    for (int i = starting; i >= gh; i--) {
      leds[i] = CRGB::Black;
    }
    for (int j = NUM_LEDS - starting; j <= NUM_LEDS - gh; j++) {
      leds[j] = CRGB::Black;
    }
  } else if (difference < 0) {
    for (int i = starting; i <= gh; i++) {
      leds[i] = CRGB::Red;
    }
    for (int j = NUM_LEDS - starting; j >= NUM_LEDS - gh; j--) {
      leds[j] = CRGB::Blue;
    }
  }
  Serial.print(gh);
  Serial.print("   ");
  Serial.print(NUM_LEDS - gh);
  Serial.print("   ");
  Serial.print(levels[0]);
  Serial.println();

  FastLED.show();
  starting = gh;
  delay(40);
}
