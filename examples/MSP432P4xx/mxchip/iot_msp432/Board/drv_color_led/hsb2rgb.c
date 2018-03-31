/**
 ******************************************************************************
 * @file    hsb2rgb_led.c
 * @author  Eshen Wang
 * @version V1.0.0
 * @date    17-Mar-2015
 * @brief   converts HSB color values to RGB colors to control RGB LED.
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ******************************************************************************
 */

#include "mx_common.h"
#include "mx_debug.h"
#include "color_led.h"
#include "hsb_led.h"

#define hsb2rgb_led_log(M, ...) custom_log("HSB2RGB_LED", M, ##__VA_ARGS__)
#define hsb2rgb_led_log_trace() custom_log_trace("HSB2RGB_LED")

#define H2R_MAX_RGB_val 255.0f

static float constrain(float value, float min, float max){
  if(value >= max)
    return max;
  if(value <=min )
    return min;
  return value;
}

//float Percent(float value){
//  return value = (((float)value / 255.0) * 100.0);
//}

static void H2R_HSBtoRGB(float hue, float sat, float bright, float *color) {
  // constrain all input variables to expected range
  hue = constrain(hue, 0, 360);
  sat = constrain(sat, 0, 100);
  bright = constrain(bright, 0, 100);
  
  // define maximum value for RGB array elements
  float max_rgb_val = H2R_MAX_RGB_val;
  // convert saturation and brightness value to decimals and init r, g, b variables
  float sat_f = (float)sat / 100.0f;
  float bright_f = (float)bright / 100.0f;
  float r=0, g=0, b=0;
  // If brightness is 0 then color is black (achromatic)
  // therefore, R, G and B values will all equal to 0
  if (bright <= 0) {
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
  }
  // If saturation is 0 then color is gray (achromatic)
  // therefore, R, G and B values will all equal the current brightness
  if (sat <= 0) {
    color[0] = bright_f * max_rgb_val;
    color[1] = bright_f * max_rgb_val;
    color[2] = bright_f * max_rgb_val;
  }
  // if saturation and brightness are greater than 0 then calculate
  // R, G and B values based on the current hue and brightness
  else {
    if (hue >= 0 && hue < 120) {
      float hue_primary = 1.0f - ((float)hue / 120.0f);
      float hue_secondary = (float)hue / 120.0f;
      float sat_primary = (1.0f - hue_primary) * (1.0f - sat_f);
      float sat_secondary = (1.0f - hue_secondary) * (1.0f - sat_f);
      float sat_tertiary = 1.0f - sat_f;
      r = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
      g = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
      b = (bright_f * max_rgb_val) * sat_tertiary;
    }
    else if (hue >= 120 && hue < 240) {
      float hue_primary = 1.0f - (((float)hue-120.0f) / 120.0f);
      float hue_secondary = ((float)hue-120.0f) / 120.0f;
      float sat_primary = (1.0f - hue_primary) * (1.0f - sat_f);
      float sat_secondary = (1.0f - hue_secondary) * (1.0f - sat_f);
      float sat_tertiary = 1.0f - sat_f;
      r = (bright_f * max_rgb_val) * sat_tertiary;
      g = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
      b = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
    }
    else if (hue >= 240 && hue <= 360) {
      float hue_primary = 1.0f - (((float)hue-240.0f) / 120.0f);
      float hue_secondary = ((float)hue-240.0f) / 120.0f;
      float sat_primary = (1.0f - hue_primary) * (1.0f - sat_f);
      float sat_secondary = (1.0f - hue_secondary) * (1.0f - sat_f);
      float sat_tertiary = 1.0f - sat_f;
      r = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
      g = (bright_f * max_rgb_val) * sat_tertiary;
      b = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
    }
    color[0] = r;
    color[1] = g;
    color[2] = b;
  }
  //  color[0] = Percent(color[0]);
  //  color[1] = Percent(color[1]);
  //  color[2] = Percent(color[2]);
}

/*----------------------------------------------------- INTERNAL FUNCTION  ---------------------------------------*/

// call RGB LED driver to control LED
static void OpenLED_RGB(float *color)
{
  uint8_t blue = (uint8_t)(color[2]);
  uint8_t green = (uint8_t)(color[1]);
  uint8_t red = (uint8_t)(color[0]);

  color_led_init();
  color_led_open(red, green, blue);
}

static void CloseLED_RGB()
{
    color_led_init();
	color_led_close();
}


/*----------------------------------------------------- USER INTERFACES ---------------------------------------*/

#include <math.h>

void hsb2rgb_led_init( void )
{
    color_led_init();
}

void hsb2rgb_led_open(float hues, float saturation, float brightness)
{
	float color[3] = {0};
		
	//saturation = saturation;//*pow(saturation/100.0, 0.2);
	saturation = saturation*pow(saturation/100.0, 0.1);
	brightness = brightness*pow(brightness/100.0, 2);

	H2R_HSBtoRGB(hues, saturation, brightness, color);
	OpenLED_RGB(color);
}

void hsb2rgb_led_close(void)
{
  CloseLED_RGB();
}
