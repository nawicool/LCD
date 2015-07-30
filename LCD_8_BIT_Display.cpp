#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "LCD_8_BIT_Display.h"


void init_Pin_Ports(void){
	DDR_LCD = 0xFF;
	DDR_CONTROL |= (1 << REGISTER_SELECT) | (1 << READ_WRITE) | (1 << ENABLE);
	PORT_CONTROL &= ~((1 << REGISTER_SELECT) | (1 << READ_WRITE) | (1 << ENABLE));
}


void set_Unset_Enable_Bit(void){
	PORT_CONTROL |= (1 << ENABLE);
	_delay_us(60);
	PORT_CONTROL &= ~(1 << ENABLE);
	_delay_us(60);
}

void check_BusyFlag(void){

	DDR_LCD = 0x00;
	PORT_CONTROL &= ~(1 << REGISTER_SELECT);
	PORT_CONTROL |= (1 << READ_WRITE);

	while (PORT_LCD >= 0x80){

		set_Unset_Enable_Bit();
	}

	/*Change the Pins to Default mode
	ReadWrite =0 
	RS = Instruction Register
	*/
	DDR_LCD = 0xFF;
	PORT_CONTROL &= ~(1 << REGISTER_SELECT);
	PORT_CONTROL &= ~(1 << READ_WRITE);
}

void init_Display_Start(void){

	_delay_ms(10);
	init_Sub_Function();

	_delay_ms(5);
	init_Sub_Function();

	_delay_us(200);
	init_Sub_Function();

	_delay_ms(10);
	
	function_Set();
	display_ON();
	clear_Display();
	entry_Mode();
}

void init_Sub_Function(void){
	PORT_LCD = 0x30;
	set_Unset_Enable_Bit();
}
void function_Set(void){
	check_BusyFlag();
	PORT_LCD = 0x38;    // function set 
	set_Unset_Enable_Bit();
	_delay_ms(5);
}
void display_ON(void){
	check_BusyFlag();
	PORT_LCD = 0x0F;
	set_Unset_Enable_Bit();
	_delay_us(60);
}
void clear_Display(void){
	check_BusyFlag();
	PORT_LCD = 0x01;   //clear Display
	set_Unset_Enable_Bit();
	_delay_us(60);

}

void entry_Mode(void){

	check_BusyFlag();
	PORT_LCD = 0x06;  //Entry mode set
	set_Unset_Enable_Bit();
	_delay_us(60);
}

void return_Home(void){

	check_BusyFlag();
	PORT_LCD = 0x02;   //Cursor at Pos 0
	set_Unset_Enable_Bit();
	_delay_us(60);

}

void Cursor_Off(void){
	check_BusyFlag();
	PORT_LCD = 0x0C;
	set_Unset_Enable_Bit();
	_delay_us(60);
}

void go_To_Position(uint8_t row, uint8_t col){

	uint8_t row_pos = 0x00;
	uint8_t col_pos = 0x00;
	uint8_t final_Pos = 0x00;
	uint8_t col_pos_A[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,0x0A, 0x0B, 0x0C, 0x0D, 0x0F };


	if (row == 1){
		row_pos = 0x00;
		col_pos = col_pos_A[col];
		final_Pos = row_pos + col_pos;

	}
	else {
		row_pos = 0x40;
		col_pos = col_pos_A[col];
		final_Pos = row_pos + col_pos;
	}
	set_DDRAM_Adress(final_Pos);
	_delay_us(120); //
}

void set_DDRAM_Adress(uint8_t pos){

	check_BusyFlag();
	PORT_LCD = (0x80 + pos);
	PORT_CONTROL &= ~(1 << REGISTER_SELECT);
	PORT_CONTROL &= ~(1 << READ_WRITE);
	_delay_us(60);
	set_Unset_Enable_Bit();
	_delay_us(120); //
	PORT_LCD = 0x00;


}


void send_Data_Char(char charchter){

	check_BusyFlag();
	PORT_LCD = charchter;
	PORT_CONTROL |= 1 << REGISTER_SELECT;
	PORT_CONTROL &= ~(1 << READ_WRITE);
	_delay_us(60);
	set_Unset_Enable_Bit();
	_delay_us(120); //
	PORT_LCD = 0x00;
}


void send_Data_String_Default(char *stringP){

	char *pi = stringP;

	while (*pi != '\0'){


		send_Data_Char(*stringP++);
		_delay_ms(2);
		pi = stringP;
	}
	PORT_LCD = 0x00;
}

void send_Data_String_At_Position(char *stringP, uint8_t row, uint8_t col){


	char *stringLok = stringP;
	uint8_t row_i = row;
	uint8_t col_i = col;
	go_To_Position(row_i, col_i);


	while (*stringLok != '\0'){
		PORT_LCD = *stringLok++;
		PORT_CONTROL |= (1 << REGISTER_SELECT);
		PORT_CONTROL &= ~(1 << READ_WRITE);
		set_Unset_Enable_Bit();
		_delay_us(120); //
	}
	PORT_LCD = 0x00;
}

void send_Int_At_Position(int value, uint8_t row, uint8_t col){

	uint8_t row_i = row;
	uint8_t col_i = col;
	char buffer[10];
	itoa(value, buffer, 10);
	send_Data_String_At_Position(buffer, row_i, col_i);

	PORT_LCD = 0x00;
}

void send_Double_At_Position_Int(double value, uint8_t row, uint8_t col){

	uint8_t row_i = row;
	uint8_t col_i = col;
	char buffer[10];

	dtostrf(value, 6, 3, buffer);
	send_Data_String_At_Position(buffer, row_i, col_i);
	PORT_LCD = 0x00;
}

void display_Shift(void){

	check_BusyFlag();
	PORT_CONTROL &= ~(1 << REGISTER_SELECT);
	PORT_CONTROL &= ~(1 << READ_WRITE);
	PORT_LCD = 0x18;
	set_Unset_Enable_Bit();
	_delay_us(120); //
	PORT_LCD = 0x00;

}


void create_CGRAM_Data(unsigned int *arrayTo_Store, uint8_t at_Pos_in_CGRAM){

	//unsigned int Celcius[] = { 0x6, 0x9, 0x9, 0x6, 0x0, 0, 0, 0 }; //Celcius
	//unsigned int StickMan[] = { 0x0E,0x11,0x0E,0x04,0x1F,0x04,0x0A,0x11 }; //StickMan
	//unsigned int Battery[] = { 0x0E, 0x1B, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F };
	//unsigned int Battery1[] = { 0x0E, 0x1B, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F };
	//unsigned int Battery2[] = { 0x0E, 0x1B, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x1F };
	//unsigned int Battery3[] = { 0x0E, 0x1B, 0x11, 0x11, 0x11, 0x1F, 0x1F, 0x1F };
	//unsigned int Battery4[] = { 0x0E, 0x1B, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F };
	//unsigned int Battery5[] = { 0x0E, 0x1B, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };
	//unsigned int Battery6[] = { 0x0E, 0x1B, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };
	//unsigned int Battery7[] = { 0x0E, 0x1B, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };

	unsigned int *customCharPtr = arrayTo_Store; //Give the Write string
	
	
	uint8_t pos_i = at_Pos_in_CGRAM;
	uint8_t i;
	at_Pos_in_CGRAM = pos_i;
	/*Limit the Positions up to 7 positions*/
	if (pos_i == 0) {
		pos_i = at_Pos_in_CGRAM;
	}
	else if (pos_i > 7){
		pos_i = 7 * 0x08;
	}
	else {
		pos_i = pos_i * 0x08;
	}

	check_BusyFlag();
	PORT_CONTROL &= ~(1 << REGISTER_SELECT);
	PORT_CONTROL &= ~(1 << READ_WRITE);
	PORT_LCD = (0x40 + pos_i);	
	set_Unset_Enable_Bit();
	_delay_us(120);
	for (i = 0; i < 8; i++){
		check_BusyFlag();
		PORT_CONTROL |= (1 << REGISTER_SELECT);
		PORT_CONTROL &= ~(1 << READ_WRITE);
		switch (i)
		{
		case 0: PORT_LCD = *customCharPtr++;
			set_Unset_Enable_Bit();
			_delay_us(120);
			PORT_LCD = 0x00;
			_delay_us(30);
			break;

		case 1: PORT_LCD = *customCharPtr++;
			set_Unset_Enable_Bit();
			_delay_us(120);
			PORT_LCD = 0x00;
			_delay_us(30);
			break;

		case 2: PORT_LCD = *customCharPtr++;
			set_Unset_Enable_Bit();
			_delay_us(120);
			PORT_LCD = 0x00;
			_delay_us(30);
			break;

		case 3: PORT_LCD = *customCharPtr++;
			set_Unset_Enable_Bit();
			_delay_us(120);
			PORT_LCD = 0x00;
			_delay_us(30);
			break;

		case 4: PORT_LCD = *customCharPtr++;
			set_Unset_Enable_Bit();
			_delay_us(120);
			PORT_LCD = 0x00;
			_delay_us(30);
			break;

		case 5: PORT_LCD = *customCharPtr++;
			set_Unset_Enable_Bit();
			_delay_us(120);
			PORT_LCD = 0x00;
			_delay_us(30);
			break;

		case 6: PORT_LCD = *customCharPtr++;
			set_Unset_Enable_Bit();
			_delay_us(120);
			PORT_LCD = 0x00;
			_delay_us(30);
			break;

		case 7: PORT_LCD = *customCharPtr++;
			set_Unset_Enable_Bit();
			_delay_us(120);
			PORT_LCD = 0x00;
			_delay_us(30);
			break;

		default:

			PORT_LCD = 0x00;
			set_Unset_Enable_Bit();
			_delay_us(120);
			PORT_LCD = 0x00;
			_delay_us(30);
			break;
		}
		
	}
	//return_Home();
	//_delay_ms(200);
	//go_To_Position(2, 0);
	//_delay_ms(300);
	//send_Data_Char(0x00);
	//_delay_ms(300);
	/*_delay_ms(200);
	go_To_Position(2, 0);
	_delay_ms(200);
	check_BusyFlag();
	PORT_LCD = at_Pos;
	PORT_CONTROL |= (1 << REGISTER_SELECT);
	PORT_CONTROL &= ~(1 << READ_WRITE);
	set_Unset_Enable_Bit();
	_delay_ms(100);*/
}
void show_CGRAM_Data_At_Row_Col(char from_Pos, uint8_t which_row, uint8_t which_col){
	
	uint8_t row_i = which_row;
	uint8_t col_i = which_col;
	uint8_t pos_i = from_Pos;
	if ((pos_i < 0) || (pos_i>7)){
		pos_i = 0;
	}

	_delay_us(120);
	go_To_Position(row_i, col_i);
	_delay_us(120);

	send_Data_Char(from_Pos);
	_delay_us(120);
	/*
	check_BusyFlag();
	PORT_LCD = 0x08;
	PORT_CONTROL |= (1 << REGISTER_SELECT);
	PORT_CONTROL &= (1 << READ_WRITE);
	set_Unset_Enable_Bit();

	*/
		
	
	
}
