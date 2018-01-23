#include <Arduino.h>
#include "Serial_readln.h"

/////////////////////////////////////////////////////////
// Serial文字列入力
/////////////////////////////////////////////////////////
// すでに Serial.begin() を行っているものとする


void Serial_readln( const byte n, char* buf )
{
  char c;
  byte m = 0;

  if( n == 0 ){ return; }
  while(1)
  {
    if( Serial.available() > 0 )
    {
      c = Serial.read();
      Serial.write( c );
      if( c == '\r' )
      {
        *buf = '\0';
        break;
      }
      else
      {
        if ( c != '\n' )
        {
          *buf = c;
          buf++;
          m++;
          if( m >= n-1 )
          {
            *buf = '\0';
            break;
          }
        }
      }
    }
  }
}

