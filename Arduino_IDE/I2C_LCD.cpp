/*
 * File:   I2C_LCD.cpp
 * Author: Noboru
 */
#ifndef __I2C_LCD_C__
#define __I2C_LCD_C__

#include <Arduino.h>
#include "I2C_LCD.h"

/////////////////////////////////////////////////////////
// 自前のI2C関数(低速デバイス用)
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//// 各ピンの制御
void sclk_L( void )
{
  pinMode( SCL_pin, OUTPUT);
  digitalWrite( SCL_pin, 0 );
  delayMicroseconds(1);
}

void sclk_H( void )
{
  pinMode( SCL_pin, INPUT);
  delayMicroseconds(1);
}

void sda_L( void )
{
  pinMode( SDA_pin, OUTPUT);
  digitalWrite( SDA_pin, 0 );
  delayMicroseconds(1);
}

void sda_H( void )
{
  pinMode( SDA_pin, INPUT);
  delayMicroseconds(1);
}

unsigned char sda_in( void )
{
  unsigned char res;
  
  pinMode( SDA_pin, INPUT);
  res = digitalRead( SDA_pin );
  return res;
}


//// スタートコンディション
void I2CStart(void)
{
  sclk_H();         //sclk = 1;
  delayMicroseconds(1);
  sda_H();          //sda = 1;
  delayMicroseconds(1);
  sda_L();          //sda = 0;
  delayMicroseconds(1);
  sclk_L();         //sclk = 0;
  delayMicroseconds(1);
}

//// ストップコンディション
void I2CStop(void)
{
  sclk_H();         //sclk = 1;
  delayMicroseconds(1);
  sda_L();          //sda = 0;
  delayMicroseconds(1);
  sda_H();          //sda = 1;
  delayMicroseconds(1);
  sclk_L();         //sclk = 0;
  delayMicroseconds(1);
}

/// 1バイト出力
unsigned char I2COut( uint8_t data )
{
  unsigned char i,d;
  char ack;

  d = data;
  for (i = 8; i > 0; i--)
  {
    if ((d & 0x80) != 0)
    {
      sda_H();  //sda = 1;
    }
    else
    {
      sda_L();  // sda = 0;
    }
    sclk_H(); //sclk = 1;
    delayMicroseconds(4);
    sclk_L(); //sclk = 0;
    delayMicroseconds(2);
    d = (d << 1);
  }
  delayMicroseconds(1);
  sda_H();  //sda = 1;
  delayMicroseconds(1);
  sclk_H(); //sclk = 1
  delayMicroseconds(1);

  ack = 0;
  while( ack != 0 ) // ack wait
  {
    ack = sda_in();
  }
  sclk_L(); //sclk = 0;
  delayMicroseconds(80); //wait for a moment,you can adjust it.
  return (ack);
}


/////////////////////////////////////////////////////////
// LCD用関数
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// LCDにデータまたはコマンドを送る
/////////////////////////////////////////////////////////
void I2C_LCD_write( uint16_t code )
{
  I2CStart();
  delayMicroseconds(10);
  I2COut( I2C_LCD_ADDR << 1 );  // 7bit + w
  I2COut( code >> 8 ); //コマンド指定
  I2COut( code & 0x00ff );
  delayMicroseconds(10);
  I2CStop();
  delayMicroseconds(10);
}

/////////////////////////////////////////////////////////
// LCD用関数
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// LCD初期化
void I2C_LCD_init( void )
{
  sda_H();  //sda = 1;
  sclk_H(); //sclk = 1;
  delay(100);
  I2C_LCD_write(0x0001);
  delay(3);
  I2C_LCD_write(0x0038);
  delay(3);
  I2C_LCD_write(0x000f);
  delay(3);
  I2C_LCD_write(0x0006);
  delay(3);
}

/////////////////////////////////////////////////////////
// 画面消去コマンド
void I2C_LCD_clear( void )
{
  I2C_LCD_write(0x0001);
  delay(3);
}

/////////////////////////////////////////////////////////
// カーソルホームコマンド
void I2C_LCD_home( void )
{
  I2C_LCD_write(0x0002);
  delay(3);
}

/////////////////////////////////////////////////////////
// カーソルなしエントリーモード
void I2C_LCD_nocursor( void )
{
  I2C_LCD_write(0x000C); //カーソルなし、ブリンクなし、表示ON
  delayMicroseconds(80);
  I2C_LCD_write(0x0006); //書き込み位置は右に移動
  delayMicroseconds(80);
}

/////////////////////////////////////////////////////////
// カーソルありエントリーモード
void I2C_LCD_cursor( void )
{
  I2C_LCD_write(0x000E); //カーソルあり、ブリンクなし、表示ON
  delayMicroseconds(80);
  I2C_LCD_write(0x0006); //書き込み位置は右に移動
  delayMicroseconds(80);
}

/////////////////////////////////////////////////////////
// 表示位置のセット
void I2C_LCD_pos( uint8_t x, uint8_t y )
{
  uint8_t pos;
  pos = ( x & 0x0f ) + ( 0x40 * ( y & 0x01 ) );
  I2C_LCD_write(0x0080 + pos);
  delayMicroseconds(80);
}

/////////////////////////////////////////////////////////
// 1キャラクタの表示
void I2C_LCD_putc( uint8_t data )
{
  I2C_LCD_write(0x8000 + data);
  delayMicroseconds(80);
}

/////////////////////////////////////////////////////////
// LCD表示バッファの表示
void I2C_LCD_disp( char* buf )
{
  while( *buf != 0 )
  {
    I2C_LCD_putc( *buf );
    buf++;
  }
}

#endif

