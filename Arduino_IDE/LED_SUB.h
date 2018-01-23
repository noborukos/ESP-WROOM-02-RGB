/*
 * File:   LED_SUB.h
 * Author: Noboru
 */
 
#ifndef __LED_SUB_H__
#define __LED_SUB_H__

/* プロトタイプ宣言 */
void LED_init( void );
void LED_set( uint8_t R, uint8_t G, uint8_t B );
void LED_OnOff( uint8_t on_off );


/* 定数 */
#define LED_B_pin 0
#define LED_G_pin 2
#define LED_R_pin 15

/* 変数 */
#ifdef __LED_SUB_C__
uint8_t LED_R;
uint8_t LED_G;
uint8_t LED_B;
uint8_t LED_onoff;
#else
extern uint8_t LED_R;
extern uint8_t LED_G;
extern uint8_t LED_B;
extern uint8_t LED_onoff;

#endif

#endif

