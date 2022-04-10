// GFX library font dimensions, in pixels:
// 1: X/Y: 5+1 / 7+1
// 2: X/Y: 10+2/14+2
// 3: X/Y: 15+3/21+3
// + denotes spacing between glyphs

#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <Arduino.h>
#include <Keypad.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "displayvalue.h"
#include "inputbox.h"
#include "area.h"
#include "status.h"

// TFT Screen setup
#define TFT_CS        10
#define TFT_RST        8
#define TFT_DC         7
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Matrix keypad setup
const byte ROWS = 7;
const byte COLS = 6;
char keys[ROWS][COLS] = {   //CAPS and numbers are direct input, lower-case are commands
  {'h','u','s','h','i','j'},
  {'b','A','l','r','c','d'},
  {'d','B','m','n','%','/'},
  {'a','C','7','8','9','*'},
  {'o','D','4','5','6','-'},
  {'x','E','1','2','3','+'},
  {'n','F','-','0','p','='} // p isn't mapped to the grid
};
byte rowPins[ROWS] = {0,1,3,4,5,6,9};
byte colPins[COLS] = {14,15,16,17,18,19};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// value setup
displayValue result(0,displayValue::Hex, 16);  // default Decimal mode, start from 0, nothing to show
inputBox input(0,displayValue::Hex,16);          // default Decimal mode, start from 0, show value

// status box
status currentstatus(displayValue::Hex, 16, false);

void wakeUpNow()
{
}  

void sleepNow()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(0,wakeUpNow,LOW);
  sleep_mode();
  detachInterrupt(0);
}

void setup(void) {
  // Screen init
  tft.init(240, 320);           // Init ST7789 320x240
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  // attach screen object and asign area to each output object
  input.init(&tft, area(0,100,319,129));
  result.init(&tft, area(0,0,319,23));
  currentstatus.init(&tft, area(0,224,319,239));

  // set backlight pin to on
  DDRB |= (1 << 6);
  PORTB |= (1 << 6); // set bit 6 on for backlight (BL)

  // PWR button is an input
  pinMode(2, INPUT);  // PWR button connected to PIN 2

  // show default objects on screen
  input.show();
  currentstatus.show();
}

void loop() {
  char key = keypad.getKey();

  if(key)
  {
    switch(key)
    {
      case '0' ... '9':
      case 'A' ... 'F':
        input.processKeyValue(key);
        break;
      case 'c':
        result.hide();
        input.set(0);
        break;
      case 'h':
        currentstatus.set(displayValue::Hex);
        input.setBase(displayValue::Hex);
        result.setBase(displayValue::Hex);
        break;
      case 'b':
        currentstatus.set(displayValue::Bin);
        input.setBase(displayValue::Bin);
        result.setBase(displayValue::Bin);
        break;
      case 'd':
        currentstatus.set(displayValue::Dec);
        input.setBase(displayValue::Dec);
        result.setBase(displayValue::Dec);
        break;
      
      case '=':
        result.set(input.get());
        result.show();
      default:
        break;
    }
  }

  if(digitalRead(2) == 0)
  {
    PORTB &= ~(1 << 6); // backlight off
    while(digitalRead(2) == 0);
    sleepNow();
    delay(50);  // debounce button
    while(digitalRead(2) == 0); // wait for button release
    PORTB |= (1 << 6);  // backlight back on again
  }
}
