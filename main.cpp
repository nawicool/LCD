#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "LCD_8_BIT_Display.h"



int main(void){

	/*Start-Must Functions*/
	unsigned int Battery0[] = { 0x0E, 0x1B, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F };
	unsigned int Battery1[] = { 0x0E, 0x1B, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x1F };
	unsigned int Battery2[] = { 0x0E, 0x1B, 0x11, 0x11, 0x11, 0x1F, 0x1F, 0x1F };
	unsigned int Battery3[] = { 0x0E, 0x1B, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F };
	unsigned int Battery4[] = { 0x0E, 0x1B, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };
	unsigned int Battery5[] = { 0x0E, 0x1B, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };
	unsigned int Battery6[] = { 0x0E, 0x1B, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };

	int integer_Value = 12;

	init_Pin_Ports();
	_delay_ms(2);

	init_Display_Start();
	_delay_ms(50); 

	send_Data_Char('X');
	_delay_us(120); 


	send_Data_String_Default("STOP");
	_delay_us(120); 

	send_Data_String_At_Position("THE", 1, 8);
	_delay_us(120); 

	
	go_To_Position(2, 13);
	_delay_us(120); //

	send_Data_Char('B');
	_delay_us(120); 

	send_Int_At_Position(integer_Value, 2, 7);
	_delay_us(120); //

	/*Creating CGRAM DATA  between 0-6 from(0-7)*/
	create_CGRAM_Data(Battery0, 0);
	_delay_us(120); //
	create_CGRAM_Data(Battery1, 1);
	_delay_us(120); //
	create_CGRAM_Data(Battery2, 2);
	_delay_us(120); //
	create_CGRAM_Data(Battery3, 3);
	_delay_us(120); //
	create_CGRAM_Data(Battery4, 4);
	_delay_us(120); //
	create_CGRAM_Data(Battery5, 5);
	_delay_us(120); //
	create_CGRAM_Data(Battery6, 6);
	_delay_us(120); //

	

	//Display CGRAM Data at definate place

	show_CGRAM_Data_At_Row_Col(0, 2, 0);
	_delay_us(120); //
	show_CGRAM_Data_At_Row_Col(1, 2, 1);
	_delay_us(120); //
	show_CGRAM_Data_At_Row_Col(2, 2, 2);
	_delay_us(120); //
	show_CGRAM_Data_At_Row_Col(3, 2, 3);
	_delay_us(120); //
	show_CGRAM_Data_At_Row_Col(4, 2, 4);
	_delay_us(120); //
	show_CGRAM_Data_At_Row_Col(5, 2, 5);
	_delay_us(120); //
	//show_CGRAM_Data_At_Row_Col(6, 2, 6);
	//_delay_us(120); //
	
	/* Send String at Postion 9*/
	send_Data_String_At_Position("ENDI", 2, 9);
	_delay_us(120);

	Cursor_Off();
	while (1){
		//_delay_ms(300);
		//display_Shift();
		}

	return 0;

}
