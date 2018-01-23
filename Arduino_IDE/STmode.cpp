/*
 * File:   STmode.cpp
 * Author: Noboru
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

#include "I2C_LCD.h"
#include "LED_SUB.h"
#include "Serial_readln.h"

#define DBG_OUTPUT_PORT Serial

struct CONFIG {
  char flag[2];
  char STssid[32];
  char STpassword[32];
};
CONFIG STbuf;
const char SThost[] = "ESP8266sd";

void setting_AP_data( void )
{
  char str1[32];
  char str2[32];
  char str3[32];

  I2C_LCD_clear();
  I2C_LCD_pos( 0, 0 );
  I2C_LCD_disp( "ACCESS POINT" );
  I2C_LCD_pos( 0, 1 );
  I2C_LCD_disp( "SETTING...  " );

  while( 1 )
  {
    DBG_OUTPUT_PORT.println();
    DBG_OUTPUT_PORT.println();
    DBG_OUTPUT_PORT.println();
    DBG_OUTPUT_PORT.println();
    DBG_OUTPUT_PORT.println();

    DBG_OUTPUT_PORT.print("Input AP SSID : ");
    Serial_readln( 32, &str1[0] );
    DBG_OUTPUT_PORT.println();
    DBG_OUTPUT_PORT.print("Input AP PASSWORD : ");
    Serial_readln( 32, &str2[0] );
    DBG_OUTPUT_PORT.println();

    DBG_OUTPUT_PORT.print("AP SSID : ");
    DBG_OUTPUT_PORT.println( str1 );
    DBG_OUTPUT_PORT.print("AP PASSWOED : ");
    DBG_OUTPUT_PORT.println( str2 );

    DBG_OUTPUT_PORT.print("OK?[Y/N]");
    Serial_readln( 32, &str3[0] );
    if( ( str3[0] == 'Y' ) || ( str3[0] == 'y' ) ){ break; }
  }  
  DBG_OUTPUT_PORT.println();

  STbuf.flag[0] = 0x55;
  STbuf.flag[1] = 0xaa;
  memcpy( &STbuf.STssid[0], &str1[0], 32 );
  memcpy( &STbuf.STpassword[0], &str2[0], 32 );
  I2C_LCD_clear();
}

void StMode_init( void )
{
  EEPROM.begin(128);
  EEPROM.get<CONFIG>(0, STbuf);
  if( ( STbuf.flag[0] != 0x55 ) || ( STbuf.flag[1] != 0xaa ) )
  {
    delay(3000);
    setting_AP_data();
  }
  EEPROM.put<CONFIG>(0, STbuf);
  EEPROM.commit();
  
  WiFi.begin( STbuf.STssid, STbuf.STpassword);
  DBG_OUTPUT_PORT.print("Connecting to ");
  DBG_OUTPUT_PORT.println( STbuf.STssid );

  // Wait for connection
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) {//wait 10 seconds
    delay(500);
  }
  if(i == 21){
    DBG_OUTPUT_PORT.print("Could not connect to");
    DBG_OUTPUT_PORT.println( STbuf.STssid );
    while(1) delay(500);
  }
  DBG_OUTPUT_PORT.print("Connected! IP address: ");
  DBG_OUTPUT_PORT.println(WiFi.localIP());

  if( MDNS.begin( SThost ) ) {
    MDNS.addService("http", "tcp", 80);
    DBG_OUTPUT_PORT.println("MDNS responder started");
    DBG_OUTPUT_PORT.print("You can now connect to http://");
    DBG_OUTPUT_PORT.print( SThost );
    DBG_OUTPUT_PORT.println(".local");
  }
}

void StMode_task( void )
{
  char c;
  char str1[2];

  if( DBG_OUTPUT_PORT.available() > 0 )
  {
      c = DBG_OUTPUT_PORT.read();
      DBG_OUTPUT_PORT.write( c );
      if( c == 'x' )
      {
        DBG_OUTPUT_PORT.print( "\r\n\n\nDo you want clear AP setting? : " );
        Serial_readln( 2, &str1[0] );
        if( ( str1[0] == 'Y' ) || ( str1[0] == 'y' ) )
        {
          EEPROM.begin(128);
          STbuf.flag[0] = 0;
          STbuf.flag[1] = 0;
          EEPROM.put<CONFIG>(0, STbuf);
          EEPROM.commit();
          DBG_OUTPUT_PORT.write( "\r\nPlease RESET system" );
        }
      }
  }
  return;
}


