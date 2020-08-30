/* 	
*	Character LCD definition and configuration file.
*	https://github.com/ConnorBelman/Character-LCD-Driver-TM4C123
*/

#include <stdint.h>
#include "TM4C123.h"

// LCD dimensions
#define LCD_WIDTH 20
#define LCD_HEIGHT 4

// GPIO port configurations
// DATA port for pins DB4-DB7
// CTRL port for pins RS, RW, and EN
#define LCD_DATA_PORT GPIOD
#define LCD_CTRL_PORT GPIOE
// Bits corresponding to each GPIOs clock enable
// PORTA - PORTF  -----  0 - 5
#define LCD_DATA_PORT_RCGC_BIT	3
#define LCD_CTRL_PORT_RCGC_BIT	4

// GPIO pins used for data port
#define DB4_PIN 0
#define DB5_PIN 1
#define DB6_PIN 2
#define DB7_PIN 3


// GPIO pins used for control port
#define RS_PIN	1
#define RW_PIN	2
#define EN_PIN	3

// Initialization settings
#define BLINK_ON		0		// 0: cursor blink is off		1: cursor blink is on		
#define CURSOR_ON		0		// 0: cursor is off					2: cursor is on
#define	DISPLAY_ON	4		// 0: display is off				4: display is on

int LCD_Init(void);
void LCD_Write_Command(uint8_t c);
void LCD_Write_Character(uint8_t c);
void LCD_Print(char *s, uint32_t length);
void LCD_Set_Cursor(uint8_t row, uint8_t column);
void LCD_Modify_Character(uint8_t *pattern, uint8_t location);
