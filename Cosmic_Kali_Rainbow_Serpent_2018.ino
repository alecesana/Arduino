#include <SimpleTimer.h>

/*written by Alessandro Cesana for Rainbow Serpent 2018, Cosmic Kali interactive installation.
A few videos of the installation: https://www.instagram.com/p/BepblSAF94v/
Inspired by Color Palette and other examples in the FastLed Library*/

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include <SerialCommands.h>

#define LED_PIN     3
#define NUM_LEDS    1000
#define BRIGHTNESS  100
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 30   /*change updates per second to adjust speed of color change in a smooth way,
or make it an adjustable parameter to interact with*/



CRGBPalette16 targetPalette( PartyColors_p );
CRGBPalette16 currentPalette( CRGB::Black);


TBlendType    currentBlending;

////color palettes declarations
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

extern CRGBPalette16 GoldenColors;
extern const TProgmemPalette16 GoldenColors_p PROGMEM;

extern CRGBPalette16 OrangeColors;
extern const TProgmemPalette16 OrangeColors_p PROGMEM;


extern CRGBPalette16 FunkyRedColors;
extern const TProgmemPalette16 FunkyRedColors_p PROGMEM;


String a = "0";

//bool to switch between a non interactive mode(automode = true) and an interactive mode(automode = false)
bool automode = true;

///serial commands to switch automode on or off, not always working when tested with Openframeworks , maybe due to interrupts?

char serial_command_buffer_[32];
SerialCommands serial_commands_(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");

//default handler, gets called when no other command matches. 
void cmd_unrecognized(SerialCommands* sender, const char* cmd)
{
  sender->GetSerial()->print("Unrecognized command [");
  sender->GetSerial()->print(cmd);
  sender->GetSerial()->println("]");
}

void cmd_AUTOMODE_ON_(SerialCommands* sender)
{ 
  {   
  automode = true; }
}

 void cmd_AUTOMODE_OFF_(SerialCommands* sender)
{ 
  {  automode = false;
      
  }
}


SerialCommand cmd_AUTOMODE_OFF("0", cmd_AUTOMODE_OFF_);
SerialCommand cmd_AUTOMODE_ON("1", cmd_AUTOMODE_ON_);


void ChangePalettePeriodically()
{ 
  static uint8_t lastSecond = 0;
  uint8_t secondHand = (millis() / 1000) % 8;  // loop that rotates between color palettes (example when millis() = 1000 -> 1000/1000 % 8 = 0 -, millis() = 10000 --> 10000/1000  % 8 = 2-> change of color palette happens at 0 seconds, 10 seconds, 20 seconds, 30, 4
                                              // would be way nicer to use variables rather than hard coded but will do it for the moment
  if( lastSecond != secondHand) {
    lastSecond = secondHand;

    if( secondHand ==  0)  { targetPalette = OrangeColors_p;  }
    if( secondHand == 2)  { targetPalette = FunkyRedColors_p;  }
    if( secondHand == 4)  { targetPalette = OceanColors_p;  }
    if( secondHand == 6)  { targetPalette =  ForestColors_p;   }
 
  }
}

void ChangeColorModePeriodically()   /*timer inserted to rotate between a fixed color mode and a rotating color mode 
  (or, more generally, different color modes)*/
{ 
  uint8_t secondHand = (millis() / 1000) % 30;
  static uint8_t lastSecond = 99;
  Serial.println(secondHand);
  Serial.println(lastSecond);
  if( lastSecond != secondHand) {
    lastSecond = secondHand;

    if( secondHand ==  0)  { targetPalette = GoldenColors_p; }
    if( secondHand == 10)  { ChangePalettePeriodically(); }
    //if(secondHand == 40) {s
 
  }
}



void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    Serial.begin(9600);
    currentPalette = GoldenColors_p;
    currentBlending = LINEARBLEND;

     
    serial_commands_.SetDefaultHandler(cmd_unrecognized);
   
    serial_commands_.AddCommand(&cmd_AUTOMODE_OFF);
    serial_commands_.AddCommand(&cmd_AUTOMODE_ON);
}


void loop()
{   
    serial_commands_.ReadSerial();
    ///Serial.println(modetimer);
    //ChangeColorModePeriodically();

    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* change the point on the strip at which leds start changing color adding to startindex to adjust 
    speed in a quite glitchy way, 1 gives a smooth transition */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
    
    uint8_t maxChanges = 24;
    nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges);

}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

///programmable palettes

const TProgmemRGBPalette16 GoldenColors_p PROGMEM =
{
    CRGB::DarkGoldenrod,
    CRGB::Goldenrod,
    CRGB::Gold,
    CRGB::Goldenrod,

   CRGB::Gold,
   CRGB::Gold,
   CRGB::Gold,
   CRGB::Gold,

    CRGB::DarkGoldenrod,
    CRGB::Yellow,
    CRGB::Gold,
    CRGB::Goldenrod,

    CRGB::Goldenrod,
    CRGB::Gold,
    CRGB::DarkGoldenrod,
    CRGB::Orange
};

const TProgmemRGBPalette16 OrangeColors_p PROGMEM =
{
    CRGB::Orange,
    CRGB::White,
    CRGB::Orange,
    CRGB::Orange,
    
     CRGB::DarkOrange,
     CRGB::DarkOrange,
     CRGB::DarkOrange,
     CRGB::DarkOrange,
    

    CRGB::Black,
    CRGB::Orange,
    CRGB::Orange,
    CRGB::Orange,
    
     CRGB::DarkOrange,
     CRGB::White,
     CRGB::DarkOrange,
     CRGB::DarkOrange,   

};

const TProgmemRGBPalette16 FunkyRedColors_p PROGMEM =
{
   CRGB::Black,
    CRGB::Maroon,
    CRGB::Black,
    CRGB::Orange,

    CRGB::DarkRed,
    CRGB::Maroon,
    CRGB::DarkRed,

    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::Red,
    CRGB::Orange,

    CRGB::Black,
    CRGB::Orange,
    CRGB::Red,
    CRGB::DarkRed,
};


// Additionl notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
