/*

 Original DJI LED project
 Author(s): Yves Gohy (diogeneyves@gmail.com)
 Thanks to: Leandre Gohy (leandre.gohy@hexeo.be)
 
 Current fork: Basic Adafruit Trinket NAZA LED Adaptation
 Version: V1.3 October 27,2017
 Author: ElGuano on RCGroups.com
 Updated by: Scruffy4386
 Description: This version is modified to fit the arduino nano.
 It is simplified from the original to save 
 RAM and flash space, using just basic orientation lights and NAZA mimicry. 
 
 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 For more detail on the MIT license, see <http://opensource.org/licenses/MIT>
 
 */

#include <Adafruit_NeoPixel.h>
//#include <math.h>
#define RSNUM 2 //How may LED are in each strip or led segment
#define PNUM 8 //Number of pixels in total in your project
#define PIN 9 // LED output Pin (An easy pin to use considering the use of pin 10 and 11 from original code)

#define BRIGHTNESS 100 // brightness 0-255  
#define STATICSPEED 30 // Placeholder for spin animation (not used here)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PNUM, PIN);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  pinMode(A3, INPUT);  // GPIO pins 3 and 4 for analogRead
  pinMode(A4, INPUT);
}

unsigned long int getAnimationColor(word animationSpeed) {
  static const unsigned long int animationColors[] = { 
    0x000000, 0x00FF00, 0xFF0000, 0xFFFF00       }; // colors of animation OFF, GREEN, RED, YELLOW
  static unsigned long int animationColor = 0x000000;
  // read Naza green led
  int sensorValue0 = analogRead(A4);  //Analog pin 4 
  // read Naza red led
  int sensorValue1 = analogRead(A3);   //Analog pin 3 
  
  byte animationColorIndex = (sensorValue0 > 500) + ((sensorValue1 > 500) << 1); // e.g., 1 + 2 = 3 (yellow), 0 + 2 = 2 (red)

  animationColor = animationColors[animationColorIndex];

  return animationColor;
}

void playAnimation(word animationSpeed, unsigned long int animationColor) {
    playNoAnimation(animationColor);
}

void playNoAnimation(unsigned long int animationColor) {
  for (byte i = 0; i< RSNUM; i++) {

// This shows all front/back directional colors, which all flash NAZA lights when lit up

    if (animationColor != 0x000000) {   // Assign led segments to Naza Blink if not-black
      strip.setPixelColor(i, animationColor);       // Back right led segment
      strip.setPixelColor(i+RSNUM, animationColor);    // Front right led segment
      strip.setPixelColor(i+(RSNUM*2), animationColor);    // Front left led segment
      strip.setPixelColor(i+(RSNUM*3), animationColor);    // Back left led segment
    }
    
    else {                                         // otherwise assigns led segments to directional color
      strip.setPixelColor(i ,0x00FF00);        // #1 Back right led segment 
      strip.setPixelColor(i+RSNUM ,0x000055); // #2 back right led segment
      strip.setPixelColor(i+(RSNUM*2), 0xCC00CC);     // Front right led segment
      strip.setPixelColor(i+(RSNUM*3), 0xCCFFCC);     //Front left led segment
    }
 }

  strip.setBrightness(180);
  strip.show();
}

void loop() {
  unsigned long int animationColor = getAnimationColor(STATICSPEED);
  playAnimation(STATICSPEED, animationColor);

  //Extra serial readout to help DEBUG
   // read the input pin:
  int LED1State = analogRead(A3);
  int LED2State = analogRead(A4);
  // print out the state of the LED input:
  Serial.println(LED1State,"LED1");
  delay(1);
    Serial.println(LED2State,"LED2");
  delay(1);  
  //DEBUG END
  
}
