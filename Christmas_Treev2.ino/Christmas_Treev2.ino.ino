//Christmas Tree v2
#include "FastLED.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN    6
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS    200
#define UPDATES_PER_SECOND 50

#define SEQUENCE_TIME 30

CRGB leds[NUM_LEDS];


#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

bool scatter = false;

/*

void SetupRedAndGreenPalette()
{
    CRGB red = CHSV( HUE_RED, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   red, red, black,  black,
                                   green,  green,  black,  black,
                                   red, red, black,  black );

}

void SetupWhiteAndGoldPalette()
{
    CRGB white = CRGB::White;
    CRGB gold  = CRGB::Goldenrod;
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   white,  white,  black,  black,
                                   gold, gold, black,  black,
                                   white,  white,  black,  black,
                                   gold, gold, black,  black );

}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        
        if( secondHand ==  0)  { SetupRedAndGreenPalette();           currentBlending = LINEARBLEND; }


        if( secondHand == 15)  { SetupWhiteAndGoldPalette();   currentBlending = LINEARBLEND; }


        if( secondHand == 30)  { SetupRedAndGreenPalette();       currentBlending = LINEARBLEND; }


        if( secondHand == 45)  { SetupWhiteAndGoldPalette();           currentBlending = LINEARBLEND; }

    }
}

void setup() {
  delay(500); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(Typical8mmPixel);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  currentBlending = LINEARBLEND;
  SetupRedAndGreenPalette();
  
}


void loop() {
  // put your main code here, to run repeatedly:


  uint8_t maxChanges = 24; 
  nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges);
    ChangePalettePeriodically();
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; 
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}
*/


// This example shows how to cross-fade between different color palettes
// using the function nblendPaletteTowardPalette.
//
// The basic idea is that you always have a "current palette" that you're
// pulling colors from with ColorFromPalette, and you have a "target palette"
// which is the 'next' palette that you want to get to.
// 
// After that, implementation is relatively simple: just periodically call
//   nblendPaletteTowardPalette( currentPalette, targetPalette);
// If the current palette is not yet equal to the target palette, this 
// function will make a few small changes to the current palette to make
// it slightly more like the target.  Over time, the current palette will
// come to be equal to the target.
// There's no need to test the current and target for equality; it's safe
// to keep calling nblendPaletteTowardPalette even after current reaches target.
// For faster blending, call nblendPaletteTowardPalette twice per loop.


//CRGBPalette16 currentPalette( CRGB::Black);

CRGBPalette16 targetPalette( PartyColors_p );


void setup() {
    delay(500); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(Typical8mmPixel);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  currentBlending = LINEARBLEND;

}


void loop()
{
  ChangePalettePeriodically();

  // Crossfade current palette slowly toward the target palette
  //
  // Each time that nblendPaletteTowardPalette is called, small changes
  // are made to currentPalette to bring it closer to matching targetPalette.
  // You can control how many changes are made in each call:
  //   - the default of 24 is a good balance
  //   - meaningful values are 1-48.  1=veeeeeeeery slow, 48=quickest
  //   - "0" means do not change the currentPalette at all; freeze
  
  uint8_t maxChanges = 48; 
  nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges);


  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);

  FastLED.show();
  FastLED.delay(2200 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    if (scatter) {
      leds[i] = ColorFromPalette( currentPalette, colorIndex + sin8(i*16), brightness); 
    }
    else {
      leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness); 
    }
    colorIndex -= 6;
  }
}


void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % (SEQUENCE_TIME * 7);
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    CRGB p = CHSV( HUE_PURPLE, 255, 255);
    CRGB bl = CHSV( HUE_BLUE, 255, 255);
    CRGB g = CHSV( HUE_GREEN, 255, 255);
    CRGB r = CHSV( HUE_RED, 255, 255);
    CRGB go = CRGB::Goldenrod;
    CRGB b = CRGB::Black;
    CRGB w = CRGB::White;
    CRGB aw = CRGB::AntiqueWhite;
    CRGB y = CRGB::Yellow;
    aw -= CRGB( 0, 0, 90);
    if( secondHand == SEQUENCE_TIME * 0)  { targetPalette = CRGBPalette16( g,g,b,b, r,r,b,b, g,g,b,b, r,r,b,b); scatter = false;}
    if( secondHand == SEQUENCE_TIME * 1)  { targetPalette = CRGBPalette16( w,w,w,b, go,go,go,b, w,w,w,b, go,go,go,b);  scatter = false;}
    if( secondHand == SEQUENCE_TIME * 2)  { targetPalette = CRGBPalette16( bl,g,r,b, r,g,bl,p, g,go,p,b, r,p,bl,g);  scatter = false;}
    if( secondHand == SEQUENCE_TIME * 3)  { targetPalette = CRGBPalette16( aw,b,b,b, b,b,b,b, b,b,w,b, b,b,b,b);   scatter = false ;}
    if( secondHand == SEQUENCE_TIME * 4)  { targetPalette = CRGBPalette16( w,b,b,b, b,b,b,b, b,b,b,b, b,b,b,b);   scatter = false;}
    if( secondHand == SEQUENCE_TIME * 5)  { targetPalette = CRGBPalette16( bl,b,b,b, w,b,b,b, bl,b,b,b, w,b,b,b);   scatter = false;}
    if( secondHand == SEQUENCE_TIME * 6)  { targetPalette = CRGBPalette16( g,g,g,b, r,r,r,b, y,y,y,b, b,b,b,b);   scatter = false;}
  }
}

