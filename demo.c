/*
*	Demo code for the character LCD driver library for the TM4C123.
*	Creates a custom character in the first character slot and prints a series of lines to the LCD.
* Demo is designed for a 20x4 character LCD.
* https://github.com/ConnorBelman/Character-LCD-Driver-TM4C123
*/

#include "LCD.h"

int main(){
	LCD_Init();
	
	uint8_t pattern[] = {0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A};
	uint8_t line[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	LCD_Modify_Character(pattern, 0x00);
	LCD_Set_Cursor(0,0);
	LCD_Print((char *)line, 20);
	LCD_Set_Cursor(1, 3);
	LCD_Print("Hello, world!", 13);
	LCD_Set_Cursor(2, 2);
	LCD_Print("LCD Driver Demo", 15);
	LCD_Set_Cursor(3, 0);
	LCD_Print((char *)line, 20);
	
	return 0;
}
