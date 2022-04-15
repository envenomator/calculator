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
#include "statusmessage.h"
#include "statusflags.h"
#include "operation.h"

// TFT Screen setup
#define TFT_CS        10
#define TFT_RST        8
#define TFT_DC         7
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Matrix keypad setup
const byte ROWS = 7;
const byte COLS = 6;
char keys[ROWS][COLS] = {   //CAPS and numbers are direct input, lower-case are commands
  {'h','u','s','i','j','k'},
  {'b','A','l','r','c','X'},
  {'d','B','m','n','%','/'},
  {'a','C','7','8','9','*'},
  {'o','D','4','5','6','-'},
  {'x','E','1','2','3','+'},
  {'!','F','^','0','p','='} // p isn't mapped to the grid
};
byte rowPins[ROWS] = {0,1,3,4,5,6,9};
byte colPins[COLS] = {14,15,16,17,18,19};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// value setup
displayValue result(0,status::Dec, 8, true);  // default Decimal mode, start from 0, nothing to show
inputBox input(0,status::Dec,8, true);          // default Decimal mode, start from 0, show value

// status box
statusMessage currentstatus(0, status::Dec, 8, true);
statusFlags flags;
operation op;

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
  result.init(&tft, area(0,0,319,23), ST77XX_WHITE);
  flags.init(&tft,area(0,30,319,46), ST77XX_YELLOW);
  op.init(&tft, area(0,75,319,99), ST77XX_BLUE);
  input.init(&tft, area(0,100,319,129), ST77XX_BLUE);
  currentstatus.init(&tft, area(0,224,319,239), ST77XX_GREEN);

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
      case 'X':
        input.deleteDigit();
        break;
      case 'c': // Clear
        result.setValue(0); // no results
        result.hide();
        op.set(operation::Method::None);
        op.hide();
        flags.hide();
        flags.clearFlags();
        input.setValue(0);
        break;
      case 'h':
        currentstatus.setBase(status::Hex);
        input.setBase(status::Hex);
        result.setBase(status::Hex);
        break;
      case 'b':
        currentstatus.setBase(status::Bin);
        input.setBase(status::Bin);
        result.setBase(status::Bin);
        break;
      case 'd':
        currentstatus.setBase(status::Dec);
        input.setBase(status::Dec);
        result.setBase(status::Dec);
        break;
      case 'i': // 8-bit mode
        currentstatus.setBitLength(8);
        input.setBitLength(8);
        //result.hide();
        result.setBitLength(8);
        break;
      case 'j': // 16-bit mode
        currentstatus.setBitLength(16);
        input.setBitLength(16);
        //result.hide();
        result.setBitLength(16);
        break;
      case 'k': // 32-bit mode 
        currentstatus.setBitLength(32);
        input.setBitLength(32);
        //result.hide();
        result.setBitLength(32);
        break;
      case 's': // change to 2s complement
        input.setSign(true);
        result.setSign(true);
        currentstatus.setSign(true);
        break;
      case 'u': // change to unsigned
        input.setSign(false);
        result.setSign(false);
        currentstatus.setSign(false);
        break;
      case '^': // change sign, if signed decimal
        if(input.getBase() == status::Dec && input.getSign())
        {
            input.negateDisplay();
            /*
            switch(input.getBitLength())
            {
              case 8:
                input.setValue(-(int8_t)(input.getValue()));
                break;
              case 16:
                input.setValue(-(int16_t)(input.getValue()));
                break;
              case 32:
                input.setValue(-(int32_t)(input.getValue()));
                break;
            }
            */
        }
        break;
      case '!': // Bitwise NOT
        if(result.getValue() != 0 && input.getValue() == 0)
          result.setValue(~(result.getValue()));
        else
          result.setValue(~(input.getValue()));
        result.show();
        flags.setFlags(false,status::isNegative(result.getValue(),result.getBitLength()),false);
        flags.show();
        op.hide();
        op.set(operation::None);
        input.setValue(0);
        break;
      case '=':
        // clear old flags first
        //currentstatus.clearFlags();
        if(!op.inProgress()) result.setValue(input.getValue());
        // always perform an operation at =, even 'None'
        result.setValue(op.perform(result.getValue(),input.getValue(),input.getBitLength(),input.getBase(),input.getSign()));
        result.show();
        flags.setFlags(op.getCarry(),status::isNegative(result.getValue(),result.getBitLength()),op.getOverflow());
        flags.show();
        // wait for any errors
        if(op.error())
        { 
          op.showError();
          while(op.error()) // blocked wait until user acknowledges error
          {
            key = keypad.getKey();
            if(key == 'c' || key == 'X') op.clearError();
          }
        }
        // clear operation and reset input
        op.hide();
        op.set(operation::None);
        input.setValue(0);
        break;
      default: // Operators with two operands
        if(!op.inProgress())  // only one operator at a time
        {
          // Select previous result, or current input
          if(input.getValue() != 0)
            result.setValue(input.getValue());
          else
            result.setValue(result.getValue());
          result.show();
          switch(key)
          {
            case 'a': // AND
              op.set(operation::And);
              break;
            case 'o': // OR
              op.set(operation::Or);
              break;
            case 'x': // XOR
              op.set(operation::Xor);
              break;
            case 'l': // LSHIFT
              op.set(operation::SL);
              break;
            case 'r': // RSHIFT
              op.set(operation::SR);
              break;
            case 'm': // Rotate Left
              op.set(operation::RL);
              break;
            case 'n': // Rotate Right
              op.set(operation::RR);
              break;
            case '%': // Modulo
              op.set(operation::Modulo);
              break;
            case '/': // Divide
              op.set(operation::Divide);
              break;
            case '*':
              op.set(operation::Multiply);
              break;
            case '-':
              op.set(operation::Minus);
              break;
            case '+':
              op.set(operation::Plus);
          }
          op.show();
          input.setValue(0);
        }
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
