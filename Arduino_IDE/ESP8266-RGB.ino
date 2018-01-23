/*
 * File:   ESP8266-RGB.ino
 * Author: Noboru
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <SD.h>
#include <Ticker.h>
#include <EEPROM.h>

#include "I2C_LCD.h"
#include "LED_SUB.h"

extern "C" {
#include "user_interface.h" //system_adc_read()使用のため
}


#define DBG_OUTPUT_PORT Serial
#define SSpin 16
#define ADC_SW_THRES  500


////////////////////////////////////////////////////////

ESP8266WebServer server(80);
Ticker ticker;
static bool hasSD = false;
bool readyForTicker = false;
uint16_t  TickCount = 0;
uint16_t adc_val;
uint8_t AP_ST_mode; // 1= :AP mode, 0= :ST mode

////////////////////////////////////////////////////////

void returnOK() {
  server.send(200, "text/plain", "");
}

void returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
}

bool loadFromSdCard(String path){
  String dataType = "text/plain";
  if(path.endsWith("/")) path += "index.htm";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";

  File dataFile = SD.open(path.c_str());
  if(dataFile.isDirectory()){
    path += "/index.htm";
    dataType = "text/html";
    dataFile = SD.open(path.c_str());
  }

  if (!dataFile)
    return false;

  if (server.hasArg("download")) dataType = "application/octet-stream";

  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
    DBG_OUTPUT_PORT.println("Sent less data than expected!");
  }

  dataFile.close();
  return true;
}

void handleNotFound(){
  if(hasSD && loadFromSdCard(server.uri())) return;
  String message = "SDCARD Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  DBG_OUTPUT_PORT.print(message);
}

//
////////////////////////////////////////////////////////
//  LEDの制御
void handleLED() {
  String msg;
  String cmd;
  uint32_t hex_val;
  uint8_t r,g,b;
  
  if (!server.hasArg("LED")) {
    msg = "Command Error\n";
    returnFail(msg);
    return;
  }
  cmd = server.arg("LED");
  if (cmd == "on") {
    LED_OnOff(1);
    DBG_OUTPUT_PORT.println("LED ON");
  }    
  else if (cmd == "off") {
    LED_OnOff(0);
    DBG_OUTPUT_PORT.println("LED OFF");
  }
  else
  {
    hex_val = strtol( &cmd[0], NULL, 0 );
    DBG_OUTPUT_PORT.println( hex_val,HEX );
    r = ( hex_val >> 16) & 0x00ff;
    g = ( hex_val >> 8 ) & 0x00ff;
    b =   hex_val        & 0x00ff;
    LED_set( r, g, b );
  }
  returnOK();
}

//
////////////////////////////////////////////////////////
//  Ticker割り込み
void setReadyForTicker() {
  readyForTicker = true;
}

//
////////////////////////////////////////////////////////
//  ステータス表示
void status_LCD_disp() {
  extern char* APssid;
  extern char* APpassword;
  IPAddress myIP = WiFi.localIP();

  switch( TickCount )
  {
    case 0:
      I2C_LCD_clear();
      I2C_LCD_pos( 0, 0 );
      sprintf( lcd_msg,"ESP8266-RGB" );
      I2C_LCD_disp( lcd_msg );
      I2C_LCD_pos( 0, 1 );
      if( hasSD )
      {
        sprintf( lcd_msg,"SD CARD OK" );
      }
      else
      {
        sprintf( lcd_msg,"SD CARD NG" );
      }
      I2C_LCD_disp( lcd_msg );
      break;
    case 1:
      I2C_LCD_clear();
      I2C_LCD_pos( 0, 0 );
      if( AP_ST_mode == 0 )
      {
        sprintf( lcd_msg,"STATION MODE" );
        I2C_LCD_disp( lcd_msg );
        I2C_LCD_pos( 0, 1 );
        sprintf( lcd_msg, "%d.%d.%d.%d", myIP[0], myIP[1], myIP[2], myIP[3]);
        I2C_LCD_disp( lcd_msg );
      }
      if( AP_ST_mode == 1 )
      {
        sprintf( lcd_msg,"SSID:%s",APssid );
        I2C_LCD_disp( lcd_msg );
        I2C_LCD_pos( 0, 1 );
        sprintf( lcd_msg,"PASS:%s",APpassword );
        I2C_LCD_disp( lcd_msg );
      }
      break;
    case 2:
      I2C_LCD_clear();
      I2C_LCD_pos( 0, 0 );
      if( LED_onoff != 0 ){ sprintf( lcd_msg,"LED is ON " ); } else { sprintf( lcd_msg,"LED is OFF" ); }
      I2C_LCD_disp( lcd_msg );
      I2C_LCD_pos( 0, 1 );
      sprintf( lcd_msg,"R=%02X G=%02X B=%02X",LED_R,LED_G,LED_B );
      I2C_LCD_disp( lcd_msg );
      break;
  }
  TickCount++;
  if( TickCount > 2 ){ TickCount = 0; }
  readyForTicker = false;
}

//
////////////////////////////////////////////////////////
//  初期化
void setup(void){
  extern void ApMode_init(void);
  extern void StMode_init(void);
  extern void disp_chip_info( void );
 
  LED_init();
  LED_set( 0, 0, 0 );
  I2C_LCD_init();
  I2C_LCD_nocursor();
  I2C_LCD_clear();
  I2C_LCD_pos( 0, 0 );
  I2C_LCD_disp( "ESP8266-RGB" );
  I2C_LCD_pos( 0, 1 );
  I2C_LCD_disp( "INITIALIZE..." );


  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.setDebugOutput(true);
  delay(10);
  DBG_OUTPUT_PORT.print("\n");

  disp_chip_info();

  adc_val = system_adc_read();
  if( adc_val > ADC_SW_THRES )
  {
    // ステーションモード初期化
    AP_ST_mode = 0;
    StMode_init();
  }
  else
  {
    // アクセスポイントモード初期化
    AP_ST_mode = 1;
    ApMode_init();
  }

  server.onNotFound(handleNotFound);
  server.on("/do",HTTP_GET, handleLED);
  server.begin();
  
  DBG_OUTPUT_PORT.println("HTTP server started");

  if (SD.begin(SSpin)){
     DBG_OUTPUT_PORT.println("SD Card initialized.");
     hasSD = true;
  }

  ticker.attach( 3, setReadyForTicker );
  status_LCD_disp(); //1st
}

//
////////////////////////////////////////////////////////
//  ループ
void loop(void){
  extern void ApMode_task(void);
  extern void StMode_task(void);
 
  if( AP_ST_mode == 0 ){ StMode_task(); }
  if( AP_ST_mode == 1 ){ ApMode_task(); }
  server.handleClient();

  if (readyForTicker){
    status_LCD_disp();
  }

}

