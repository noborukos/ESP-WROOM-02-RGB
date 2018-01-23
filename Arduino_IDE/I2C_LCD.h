/*
 * File:   I2C_LCD.h
 * Author: Noboru
 */
 
#ifndef __I2C_LCD_H__
#define __I2C_LCD_H__
 
/* プロトタイプ宣言 */
void I2C_LCD_init( void );
void I2C_LCD_clear( void );
void I2C_LCD_home( void );
void I2C_LCD_nocursor( void );
void I2C_LCD_cursor( void );
void I2C_LCD_pos( uint8_t x, uint8_t y );
void I2C_LCD_putc( uint8_t data );
void I2C_LCD_disp( char* buf );

/* 定数 */
#define I2C_LCD_ADDR 0x50
#define SDA_pin 4
#define SCL_pin 5

/* 変数 */
#ifdef __I2C_LCD_C__
char lcd_msg[32];
#else
extern char lcd_msg[];
#endif




#endif /* __I2C_LCD_H__ */

