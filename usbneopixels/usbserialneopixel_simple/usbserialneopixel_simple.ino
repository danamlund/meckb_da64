#include <DigiCDC.h>
#include <Adafruit_NeoPixel.h>

static const int NUM_LEDS = 16;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(
  NUM_LEDS,             // Number of pixels in strip
  0,                   // Pin number (most are valid)
  NEO_GRB + NEO_KHZ800  //  pixel type flags, add together as needed:
                        //   NEO_RGB     Pixels are wired for RGB bitstream
                        //   NEO_GRB     Pixels are wired for GRB bitstream
                        //   NEO_KHZ400  400 KHz bitstream (e.g. Old FLORA pixels)
                        //   NEO_KHZ800  800 KHz bitstream (e.g. New FLORA pixels and most WS2811 strips)
);

void setup() {                
  strip.begin();
  strip.show();

  for (int i = 0; i < NUM_LEDS; i++) {
    uint32_t c = strip.Color(0, 0, 0);
    strip.setPixelColor(i, c);
  }
  uint32_t c = strip.Color(0, 0, 50);
    strip.setPixelColor(0, c);
  strip.show();

  // initialize the digital pin as an output.
  SerialUSB.begin(); 
}

int await() {
  uint8_t waits = 0;
  while (true) {
    if (SerialUSB.available()) {
      return 1;
    } 
    if (waits > 20) {
      return 0;
    }
    waits++;
  }
}

// the loop routine runs over and over again forever:
void loop() {

  if (!SerialUSB.available()) {
    SerialUSB.refresh();
    return;
  }

  for (uint8_t led = 0; led < NUM_LEDS; led++) {
    if (!await()) {
      return;
    }
    uint8_t red = SerialUSB.read();
    
    if (!await()) {
      return;
    }
    uint8_t green = SerialUSB.read();
    
    if (!await()) {
      return;
    }
    uint8_t blue = SerialUSB.read();

    strip.setPixelColor(led, strip.Color(red, green, blue));
  }
  strip.show();
}
 
