/*
 * File:   LED_SUB.cpp
 * Author: Noboru
 */
#ifndef __LED_SUB_C__
#define __LED_SUB_C__
#define DBG_OUTPUT_PORT Serial

#include <Arduino.h>
#include "LED_SUB.h"

void LED_init( void )
{
  pinMode(LED_B_pin, OUTPUT);
  pinMode(LED_G_pin, OUTPUT);
  pinMode(LED_R_pin, OUTPUT);
  analogWriteFreq(2000);
  analogWriteRange(256);
  analogWrite( LED_B_pin, 0 );
  analogWrite( LED_G_pin, 0 );
  analogWrite( LED_R_pin, 0 );
  LED_R = 0;
  LED_G = 0;
  LED_B = 0;
  LED_onoff = 0;
}

void LED_set( uint8_t R, uint8_t G, uint8_t B )
{
  LED_R = R;
  LED_G = G;
  LED_B = B;
  if( LED_onoff != 0 )
  {
    LED_OnOff(1);
  }
}

void LED_OnOff( uint8_t on_off )
{
  if( on_off != 0 )
  {
    LED_onoff = 1;
    analogWrite( LED_B_pin, (uint16_t)( LED_B ) );
    analogWrite( LED_G_pin, (uint16_t)( LED_G ) );
    analogWrite( LED_R_pin, (uint16_t)( LED_R ) );
  }
  else
  {
    LED_onoff = 0;
    analogWrite( LED_B_pin, 0 );
    analogWrite( LED_G_pin, 0 );
    analogWrite( LED_R_pin, 0 );
  }

}



#endif

