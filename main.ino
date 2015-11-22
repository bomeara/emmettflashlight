/*
  This sketch is used to control an HLS flashlight created by Dan Rasmussen.
  See instructions on how to construct the flashlight at makezine.com
  Please feel free to email questions or comments to dan@retro-tronics.com

  It was modified by Naomi (age 9) & Brian O'Meara (age 36) to make a
  flashlight for Naomi's brother, Emmett (age 6). We are using a
  Trinket Pro 5V from Adafruit for this.

  This is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoPixel.  If not, see
  <http://www.gnu.org/licenses/>.
*/

#include <Adafruit_NeoPixel.h>
// First define some constants to help write the different modes.
// The following comments help to understand how some of the pixel
// maniulations in the code 
// 0x1    =     1 = 0000000000000001
// 0x2    =     2 = 0000000000000010
// 0x4    =     4 = 0000000000000100
// 0x8    =     8 = 0000000000001000
// 0x10   =    16 = 0000000000010000
// 0x20   =    32 = 0000000000100000
// 0x40   =    64 = 0000000001000000
// 0x80   =   128 = 0000000010000000
// 0x100  =   256 = 0000000100000000
// 0x200  =   512 = 0000001000000000
// 0x400  =  1024 = 0000010000000000
// 0x800  =  2048 = 0000100000000000
// 0x1000 =  4096 = 0001000000000000
// 0x2000 =  8192 = 0010000000000000
// 0x4000 = 16384 = 0100000000000000
// 0x8000 = 32768 = 1000000000000000

#define PIXEL_1  0x1
#define PIXEL_2  0x2
#define PIXEL_3  0x4
#define PIXEL_4  0x8
#define PIXEL_5  0x10
#define PIXEL_6  0x20
#define PIXEL_7  0x40
#define PIXEL_8  0x80
#define PIXEL_9  0x100
#define PIXEL_10 0x200
#define PIXEL_11 0x400
#define PIXEL_12 0x800
#define PIXEL_13 0x1000
#define PIXEL_14 0x2000
#define PIXEL_15 0x4000
#define PIXEL_16 0x8000

static unsigned int pixelSelect[] = {PIXEL_1,  PIXEL_2,  PIXEL_3,  PIXEL_4,  PIXEL_5, PIXEL_6,  
                                     PIXEL_7,  PIXEL_8,  PIXEL_9,  PIXEL_10, PIXEL_11, PIXEL_12, 
                                     PIXEL_13, PIXEL_14, PIXEL_15, PIXEL_16 };

// rothue - used for autorotate mode.
float rothue = 0.0;

// Initial rgb values
int red   = 0;
int blue  = 0;
int green = 0;

// Intial rotation directions
int redUp = 0;
int greenUp = 0;
int blueUp = 0;


// Digital input pin definitions
#define WHITE_PIN   2
#define HLS_PIN     3
#define HLS_ROT_PIN 4
#define MROT_PIN    5
#define RANDOM_PIN  6
#define CYLON_PIN   7
#define STROBE_PIN  10
#define MODE_G_PIN  9
#define MODE_H_PIN  8
#define MODE_I_PIN  11

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
#define NUMPIXELS   12
#define NEOPIX_PIN  12
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIX_PIN, NEO_GRB + NEO_KHZ800);

/*
 * Function: loop
 * Purpose: Standard Arduino setup function
 */
void setup() 
{
  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(NEOPIX_PIN, OUTPUT);

  // The switch pins
  pinMode(WHITE_PIN,   INPUT_PULLUP);
  pinMode(HLS_PIN,     INPUT_PULLUP);
  pinMode(HLS_ROT_PIN, INPUT_PULLUP);
  pinMode(RANDOM_PIN,  INPUT_PULLUP);
  pinMode(MROT_PIN,    INPUT_PULLUP);
  pinMode(CYLON_PIN,   INPUT_PULLUP);
  pinMode(STROBE_PIN,  INPUT_PULLUP);
  pinMode(MODE_G_PIN,  INPUT_PULLUP);
  pinMode(MODE_H_PIN,  INPUT_PULLUP);
  pinMode(MODE_I_PIN,  INPUT_PULLUP);

  // Set up the HLS pots as an INPUT:
  pinMode(A1, INPUT); //H
  pinMode(A2, INPUT); //S
  pinMode(A3, INPUT); //V

  // The on-baord LED
  pinMode(13, OUTPUT);

  // Comment out the following pin 13 write/delays to have start-up go faster.
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
}


/*
 * Function: loop
 * Purpose: Standard Arduino main loop 
 */
void loop() 
{
   // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
   int pot1   =  analogRead(A1);
   int pot2   =  analogRead(A2);
   int pot3   =  analogRead(A3);
   float sat  =  pot2/1024.0;
   float val  =  pot3/1024.0;

   // White only
   if (false == digitalRead(WHITE_PIN))
   {
      hsvToRgb(0.99, 0.0, val);
      for (int i=0; i < NUMPIXELS; i++) 
      {
   pixels.setPixelColor(i, pixels.Color(red, green, blue)); 
      }
      pixels.show();   // This sends the updated pixel color to the hardware.
   }
   // Manual HLS
   else  if (false == digitalRead(HLS_PIN))
   {
      hsvToRgb(pot1/1024.0, sat, val);
      for (int i=0; i < NUMPIXELS; i++) 
      {
   pixels.setPixelColor(i, pixels.Color(red, green, blue)); 
      }
      pixels.show();   // This sends the updated pixel color to the hardware.
   }
   // Rotate HLS
   else  if (false == digitalRead(HLS_ROT_PIN))
   {
      rothue = rothue + 0.01;
      if (1.0 < rothue) rothue = 0.01;
      hsvToRgb(rothue, sat, val);
      for (int i=0; i < NUMPIXELS; i++) 
      {
   pixels.setPixelColor(i, pixels.Color(red, green, blue)); 
      }
      pixels.show();   // This sends the updated pixel color to the hardware.
      delay(pot1/10); // Delay for a period of time (in milliseconds).
   }
   else  if (false == digitalRead(RANDOM_PIN))
   {
      multiRotate(pot1, sat, val);
   }
   else  if (false == digitalRead(MROT_PIN))
   {
      triRotate(pot1, sat, val);
   }
   else  if (false == digitalRead(CYLON_PIN))
   {
      cylon(pot1, sat, val);
   }
   else  if (false == digitalRead(STROBE_PIN))
   {
      strobe(pot1, sat, pot3);
   }
   else  if (false == digitalRead(MODE_G_PIN))
   {
      mode_g(pot1, sat, pot3);
   }
   else  if (false == digitalRead(MODE_H_PIN))
   {
      mode_h(pot1, sat, pot3);
   }
   else  if (false == digitalRead(MODE_I_PIN))
   {
      mode_i(pot1, sat, pot3);
   }
   else 
   {
      // Should not happen
      strobe(pot1, sat, pot3);
   }
}


/*
 * Function: cylon
 * Purpose: Like Battlestar Gallactica exept in a circle
 * Args: pot1: controls speed
 *       sat: saturation setting (between 0.0 and 1.0)
 *       val: value setting      (between 0.0 and 1.0)
 */
void cylon(float pot1, float sat, float val)
{
   static int direction = 1;
   static int onPin = 0;
   onPin += direction;
   if ((onPin >= NUMPIXELS-1) || (onPin <= 0))
   {
      direction = -direction;
   }

   rothue = rothue + 0.01;
   if (1.0 < rothue) rothue = 0.01;
   hsvToRgb(rothue, sat, val);
   for (int i=0; i < NUMPIXELS; i++) 
   {
      if (i == onPin)
      {
   pixels.setPixelColor(i, pixels.Color(red, green, blue)); 
      }
      else
      {
   pixels.setPixelColor(i, pixels.Color(0,0,0)); 
      }
   }
   delay(pot1/10); 
   pixels.show();  
}

/*
 * Function: multiRotate
 * Purpose: Rotates all pixels around the ring while slowly animating individual pixel color.
 * Args: pot1: controls speed
 *       sat: saturation setting (between 0.0 and 1.0)
 *       val: value setting      (between 0.0 and 1.0)
 */
void multiRotate(float pot1, float sat, float val)
{
   static int onCounter          = 0;
   static boolean onMode         = true;
   static unsigned int pixelMask = 0;
   static int colorRed[] = {0,0,0,0,0,0,0,0,0,0,0,0};
   static int colorGre[] = {0,0,0,0,0,0,0,0,0,0,0,0};
   static int colorBlu[] = {0,0,0,0,0,0,0,0,0,0,0,0};
   static int nextPixelIndex = 0;
   static int newColor = 0;

   // Choose a random pixel
   int currentState   = pixelMask;

   if ((false == onMode) || (pixelMask < 0xfff))
   {
      newColor = random(1024);
      while (pixelMask == currentState)
      {
   nextPixelIndex = random(NUMPIXELS);
   if (onMode)
   {
      // turn on the bit
      pixelMask |= pixelSelect[nextPixelIndex];
   }
   else
   {
      // turn off the bit (& with 0xfff to keep the latter math correct)
      pixelMask = pixelMask & (~pixelSelect[nextPixelIndex] & 0xfff);
   }
      }
   }
   else
   {
      if (++nextPixelIndex > 11) nextPixelIndex = 0;
      newColor += 104;
      if (newColor > 1024) newColor = 1;
   }

   // Set the chosen pixel's color
   if (onMode)
   {
      hsvToRgb(newColor/1024.0, sat, val);
      colorRed[nextPixelIndex] = red;
      colorGre[nextPixelIndex] = green;
      colorBlu[nextPixelIndex] = blue;
   }
   else
   {
      colorRed[nextPixelIndex] = 0;
      colorGre[nextPixelIndex] = 0;
      colorBlu[nextPixelIndex] = 0;
   }

   // Execute the setting
   pixels.setPixelColor(nextPixelIndex, 
      pixels.Color(colorRed[nextPixelIndex], 
             colorGre[nextPixelIndex], 
             colorBlu[nextPixelIndex])); 

   // Check to see if we ar changing direction.
   //if (pixelMask >= 0xfff) onMode = false;
   if (onCounter++ > 120)
   {
     onMode = false;
   }

   if (pixelMask <= 0)
   {
     onMode    = true;
     onCounter = 0;
   }

   pixels.show();   // This sends the updated pixel color to the hardware.
   delay(pot1/10); // Delay for a period of time (in milliseconds).
}

/*
 * Function: triRotate
 * Purpose: Rotates three pixels around the ring while slowly animating their color.
 * Args: sat: saturation setting (between 0.0 and 1.0)
 *       val: value setting      (between 0.0 and 1.0)
 */
void triRotate(float pot1, float sat, float val)
{
   // Use static so that the function remembrers the value of the variables between calls.
   // Initial values are set here but it will always retain latest value afterward.
   static float color1  = 0.01;
   static float color2  = 0.33;
   static float color3  = 0.66;
   static unsigned int lighted_pixel1 = PIXEL_1;
   static unsigned int lighted_pixel2 = PIXEL_5;
   static unsigned int lighted_pixel3 = PIXEL_9;

   // Execute the setting
   for (int i=0; i<NUMPIXELS; i++)
   {
      if (pixelSelect[i] == lighted_pixel1)
      {
   hsvToRgb(color1, sat, val);
   pixels.setPixelColor(i, red, green, blue); 
      } 
      else if (pixelSelect[i] == lighted_pixel2)
      {
   hsvToRgb(color2, sat, val);
   pixels.setPixelColor(i, red, green, blue); 
      } 
      else if (pixelSelect[i] == lighted_pixel3)
      {
   hsvToRgb(color3, sat, val);
   pixels.setPixelColor(i, red, green, blue); 
      } 
      else  // off
      {
   pixels.setPixelColor(i, 0, 0, 0); 
      }
   }

   // Incemente the color and reset if out of bounds (0.0 to 1.0)
   color1 += 0.01;
   color2 += 0.01;
   color3 += 0.01;
   if (color1 >= 1.0)  color1  = 0.1;
   if (color2 >= 1.0)  color2  = 0.1;
   if (color3 >= 1.0)  color3  = 0.1;

   // Left rotate each pixel selector, reset if out of bounds
   lighted_pixel1 = lighted_pixel1 << 1;
   lighted_pixel2 = lighted_pixel2 << 1;
   lighted_pixel3 = lighted_pixel3 << 1;
   if (lighted_pixel1 > PIXEL_12) lighted_pixel1 = PIXEL_1;
   if (lighted_pixel2 > PIXEL_12) lighted_pixel2 = PIXEL_1;
   if (lighted_pixel3 > PIXEL_12) lighted_pixel3 = PIXEL_1;

   pixels.show();   // This sends the updated pixel color to the hardware.
   delay(pot1/10); // Delay for a period of time (in milliseconds).

}

/*
 * Function: strobe
 * Purpose: Simple on/off strobe
 * Args: pot1: Used for hue value
 *       sat: saturation setting (between 0.0 and 1.0)
 *       pot3: controls frequecy
 */
void strobe(int pot1, float sat, int pot3)
{
   static boolean on  = true;
   on = !on;

   // Execute the setting
   for (int i=0; i<NUMPIXELS; i++)
   {
      if (on)
      {
   hsvToRgb(pot1/1024.0, sat, 1.0);
         pixels.setPixelColor(i, red, green, blue); 
      }
      else
      {
         pixels.setPixelColor(i, 0, 0, 0); 
      }
   }

   pixels.show(); // This sends the updated pixel color to the hardware.
   delay(pot3/2); // Delay for a period of time (in milliseconds).
}


/*
 * Function: mode_g
 * Purpose: Alternate pixel on/off - 180 degree pixel color change
 * Args: pot1: Used for hue value
 *       sat: saturation setting (between 0.0 and 1.0)
 *       pot3: controls frequecy
 */
void mode_g(int pot1, float sat, int pot3)
{
   static boolean on[] = {false,true,false,true,false,true,false,true,false,true,false,true};
   if (on[0])
   {
      // Move color by 180 degrees
      pot1 += 512;
      if (1024 < pot1) pot1-=1024;
   }


   // Execute the setting
   for (int i=0; i<NUMPIXELS; i++)
   {
      if (on[i])
      {
   hsvToRgb(pot1/1024.0, sat, 1.0);
         pixels.setPixelColor(i, red, green, blue); 
      }
      else
      {
         pixels.setPixelColor(i, 0, 0, 0); 
      }
      on[i] = !on[i];
   }

   pixels.show(); // This sends the updated pixel color to the hardware.
   delay(pot3/2); // Delay for a period of time (in milliseconds).
}


/*
 * Function: mode_h
 * Purpose: Alternate half moon on/off - 180 degree pixel color change
 * Args: pot1: Used for hue value
 *       sat: saturation setting (between 0.0 and 1.0)
 *       pot3: controls frequecy
 */
void mode_h(int pot1, float sat, int pot3)
{
   static boolean on[] = {false,false,false,false,false,false,true,true,true,true,true,true};
   if (on[0])
   {
      // Move color by 180 degrees
      pot1 += 512;
      if (1024 < pot1) pot1-=1024;
   }


   // Execute the setting
   for (int i=0; i<NUMPIXELS; i++)
   {
      if (on[i])
      {
   hsvToRgb(pot1/1024.0, sat, 1.0);
         pixels.setPixelColor(i, red, green, blue); 
      }
      else
      {
         pixels.setPixelColor(i, 0, 0, 0); 
      }
      on[i] = !on[i];
   }

   pixels.show(); // This sends the updated pixel color to the hardware.
   delay(pot3/2); // Delay for a period of time (in milliseconds).
}


/*
 * Function: mode_i
 * Purpose: Alternate dual pixel on/off - 180 degree pixel color change
 * Args: pot1: Used for hue value
 *       sat: saturation setting (between 0.0 and 1.0)
 *       pot3: controls frequecy
 */
void mode_i(int pot1, float sat, int pot3)
{
   static boolean on[] = {false,false,true,true,false,false,true,true,false,false,true,true};
   if (on[0])
   {
      // Move color by 180 degrees
      pot1 += 512;
      if (1024 < pot1) pot1-=1024;
   }

   // Execute the setting
   for (int i=0; i<NUMPIXELS; i++)
   {
      if (on[i])
      {
   hsvToRgb(pot1/1024.0, sat, 1.0);
         pixels.setPixelColor(i, red, green, blue); 
      }
      else
      {
         pixels.setPixelColor(i, 0, 0, 0); 
      }
      on[i] = !on[i];
   }

   pixels.show(); // This sends the updated pixel color to the hardware.
   delay(pot3/2); // Delay for a period of time (in milliseconds).
}


// function: hsvToRgb 
// Purpose: Convert HSV to RGB (RGB is necessary to drive NeoPixel)
// Input: 0.0 < hue        < 1.0
// Input: 0.0 < saturation < 1.0
// Input: 0.0 < value      < 1.0
// Output (via global) 0 <= red, green, blue <= 255;
void hsvToRgb(float hue, float saturation, float value) 
{
   float r, g, b;
   int   h = (int)(hue * 6);
   float f = hue * 6 - h;
   float p = value * (1 - saturation);
   float q = value * (1 - f * saturation);
   float t = value * (1 - (1 - f) * saturation);

   if (h == 0) 
   {
      r = value;
      g = t;
      b = p;
   } 
   else if (h == 1) 
   {
      r = q;
      g = value;
      b = p;
  } 
   else if (h == 2) 
   {
      r = p;
      g = value;
      b = t;
   } 
   else if (h == 3) 
   {
      r = p;
      g = q;
      b = value;
   } 
   else if (h == 4) 
   {
      r = t;
      g = p;
      b = value;
   } 
   else if (h == 5) 
   {
      r = value;
      g = p;
      b = q;
   } 
   else 
   {
      //printf("Huh?\n");
   }

   red   = (int)(r*255.0);
   green = (int)(g*255.0); 
   blue  = (int)(b*255.0);
}

