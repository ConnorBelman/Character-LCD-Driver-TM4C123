/*
* Character LCD driver functions.
* https://github.com/ConnorBelman/Character-LCD-Driver-TM4C123
*/

#include "LCD.h"

#define DB4 (1U << DB4_PIN)
#define DB5 (1U << DB5_PIN)
#define DB6 (1U << DB6_PIN)
#define DB7 (1U << DB7_PIN)
#define RS	(1U << RS_PIN)
#define RW  (1U << RW_PIN)
#define EN  (1U << EN_PIN)

/*
*	Delay function to allow the LCD controller time to accept the previous command
* inputs:	d = number of delay cycles
* outputs: none
*/
static void delay(uint32_t d) {
	uint32_t i;
	for(i = 0; i < d; i++) {
		__asm__("nop");
	}
}

/*
*	Formats a 4-bit message to write to the correct data pins and sends it to the LCD controller
* inputs: n = 4-bit message to send
* outputs: none
*/
static void LCD_Send_Nybble(uint8_t n) {
	// Format the byte to use the correct bits in the data register
	uint8_t fn = 0;	
	fn |= (n & 0x01) << DB4_PIN;
	fn |= ((n & 0x02) >> 1) << DB5_PIN;
	fn |= ((n & 0x04) >> 2) << DB6_PIN;
	fn |= ((n & 0x08) >> 3) << DB7_PIN;
	// Send the byte to the LCD 
	LCD_DATA_PORT->DATA	&= ~(DB4 | DB5 | DB6 | DB7);
	LCD_DATA_PORT->DATA |= fn;
	LCD_CTRL_PORT->DATA |= EN;
	LCD_CTRL_PORT->DATA &= ~EN;
}

/*
*	Writes an 8-bit command to the LCD controller
* inputs: c = 8-bit command to send
* outputs: none
*/
void LCD_Write_Command(uint8_t c) {
	// Set control pins to write, 
	LCD_CTRL_PORT->DATA &= ~(RS | RW | EN);
	LCD_Send_Nybble(c  >> 4);
	LCD_Send_Nybble(c & 0x0F);
	// Allow time for the LCD controller to process the command
	delay(7000);
}

/* 
* Writes a character to the LCD
* inputs: c = character to send
* outputs: none
*/
void LCD_Write_Character(uint8_t c) {
	LCD_CTRL_PORT->DATA &= ~(RS | RW | EN);
	LCD_CTRL_PORT->DATA |= RS;
	LCD_Send_Nybble(c >> 4);
	LCD_Send_Nybble(c & 0x0F);
	// Allow time for the LCD controller to process the character
	delay(4000);
}

/* 
* Writes a string of characters to the LCD
* inputs: s = buffer of characters to send
*					length = length of string
* outputs: none
*/
void LCD_Print(char *s, uint32_t length) {
	uint32_t i;
	for(i = 0; i < length; i++) {
		LCD_Write_Character(s[i]);
	}
}

/* 
* Moves the cursor of the LCD. (0,0) is the top left of the screen.
*	inputs: row: y position to move cursor to
*					column: x position to move cursor to
* outputs: none
*/
void LCD_Set_Cursor(uint8_t row, uint8_t column) {
	uint8_t ddram_addr = row % 2 == 0 ? 0x00 : 0x40;
	ddram_addr += row >= (LCD_HEIGHT / 2) ? LCD_WIDTH : 0;
	ddram_addr += column;
	LCD_Write_Command(0x80 | ddram_addr);
}

/* 
* Modifies one the custom character slots in CGRAM.
* inputs: pattern = Buffer of at least 8 bytes. Upper 3 bits of each byte are unused.
*										Bottom 5 bits represent the pattern of the current row of the character.
*										Bytes represent character from top to bottom.
*					location = Character slot to write to. Should be between 0 and 7.
* outputs: none
*/
void LCD_Modify_Character(uint8_t *pattern, uint8_t location) {
	uint8_t i;
	if(location >= 8) {
		return;
	}
	for(i = 0; i < 8; i++) {
		LCD_Write_Command(0x40 + (8 * location) + i);		// Move address counter to correct place in CGRAM
		LCD_Write_Character(pattern[i]);								// Write pattern to CGRAM
	}
}
/*
*	Initializes the GPIO pins for the LCD.
* Sends initialization commands to the LCD controller.
* inputs: none
* outputs: none
*/
int LCD_Init() {
	// Initialize GPIO pins
	SYSCTL->RCGCGPIO |= (1 << LCD_DATA_PORT_RCGC_BIT);	// Enable clock for data GPIO port
	LCD_DATA_PORT->DIR |= DB4 | DB5 | DB6 | DB7;				// Set pins to output
	LCD_DATA_PORT->DEN |= DB4 | DB5 | DB6 | DB7;				// Digital Enable
	LCD_DATA_PORT->AMSEL &= ~(DB4 | DB5 | DB6 | DB7);		// Disable analog function
	LCD_DATA_PORT->PDR |= DB4 | DB5 | DB6 | DB7;
	
	SYSCTL->RCGCGPIO |= (1 << LCD_CTRL_PORT_RCGC_BIT);	// Enable clock for control GPIO port
	LCD_CTRL_PORT->DIR |= RS | RW | EN;									// Set pins to output
	LCD_CTRL_PORT->DEN |= RS | RW | EN;									// Digital Enable
	LCD_CTRL_PORT->AMSEL &= ~(RS | RW | EN);						// Disable analog function
	
	// Initialize LCD
	delay(100000);												// Initial delay to allow LCD to fully power up
	LCD_Write_Command(0x28);							// Configure to 4-bit data, 2 display lines, 5x8 dot font
	LCD_Write_Command(0x08 | DISPLAY_ON 	// Set display, cursor, and cursor blink bits
		| CURSOR_ON | BLINK_ON);
	LCD_Write_Command(0x01);							// Clear display
	LCD_Write_Command(0x06);							// Configure to cursor increment, no shift
	
	return 0;
}
