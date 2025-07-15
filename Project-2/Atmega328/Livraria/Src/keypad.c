/*************************************************************************
	KEYPAD
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: all
Date:     27112022
*************************************************************************/
/*** Library ***/
#include "keypad.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*** Variable ***/
volatile uint8_t *keypad_DDR;
volatile uint8_t *keypad_PIN;
volatile uint8_t *keypad_PORT;

struct keydata{
	uint8_t line_1;
	uint8_t line_2;
	uint8_t line_3;
	uint8_t line_4;
}keypad_datai, keypad_dataf;

char keypadvalue[KEYPADLINES][KEYPADCOLUMNS] =
{
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', 35, 'D'}
};
uint8_t KEYPADSTRINGINDEX;
char KEYPAD_string[KEYPADSTRINGSIZE + 1];
keypadata data;
char KEYPAD_char;
char endstr[2] = "\0";
// can not assign something outside a function

/*** Procedure and Function declaration ***/
// getkey
char KEYPAD_getkey(void);
// read
void KEYPAD_read(void);
// get
keypadata KEYPAD_data(void);
// flush
void KEYPAD_flush(void);
// lh
uint8_t KEYPADlh(uint8_t xi, uint8_t xf);
// hl
uint8_t KEYPADhl(uint8_t xi, uint8_t xf);

/*** Handler ***/
KEYPAD keypad_enable(volatile uint8_t *ddr, volatile uint8_t *pin, volatile uint8_t *port)
{
	// LOCAL VARIABLE
	data.character = ' ';
	// ALLOCA��O MEMORIA PARA Estrutura
	KEYPAD keypad;
	// import parameters
	keypad_DDR = ddr;
	keypad_PIN = pin;
	keypad_PORT = port;
	// initialize variables
	*keypad_DDR = (1 << KEYPADLINE_1) | (1 << KEYPADLINE_2) | (1 << KEYPADLINE_3) | (1 << KEYPADLINE_4);
	*keypad_PORT = (1 << KEYPADDATA_1) | (1 << KEYPADDATA_2) | (1 << KEYPADDATA_3) | (1 << KEYPADDATA_4);
	keypad_datai.line_1 = keypad_dataf.line_1 = (1 << KEYPADDATA_1) | (1 << KEYPADDATA_2) | (1 << KEYPADDATA_3) | (1 << KEYPADDATA_4);
	keypad_datai.line_2 = keypad_dataf.line_2 = (1 << KEYPADDATA_1) | (1 << KEYPADDATA_2) | (1 << KEYPADDATA_3) | (1 << KEYPADDATA_4);
	keypad_datai.line_3 = keypad_dataf.line_3 = (1 << KEYPADDATA_1) | (1 << KEYPADDATA_2) | (1 << KEYPADDATA_3) | (1 << KEYPADDATA_4);
	keypad_datai.line_4 = keypad_dataf.line_4 = (1 << KEYPADDATA_1) | (1 << KEYPADDATA_2) | (1 << KEYPADDATA_3) | (1 << KEYPADDATA_4);
	KEYPADSTRINGINDEX = 0;
	// Vtable
	keypad.getkey = KEYPAD_getkey;
	keypad.read = KEYPAD_read;
	keypad.data = KEYPAD_data;
	keypad.flush = KEYPAD_flush;
	//
	*keypad_PORT |= (1 << KEYPADLINE_1) | (1 << KEYPADLINE_2) | (1 << KEYPADLINE_3) | (1 << KEYPADLINE_4);
	// Going to use pull down method.
	
	return keypad;
}

/*** Procedure and Function definition ***/
char KEYPAD_getkey(void)
{
	uint8_t HL;
	char c = 0;
	uint8_t keypad_option;
	for(keypad_option = 0; keypad_option < KEYPADLINES; keypad_option++){
		switch (keypad_option)
		{
			case 0: // line 1 index 0
				*keypad_DDR |= (1 << KEYPADLINE_1);
				*keypad_PORT &= ~(1 << KEYPADLINE_1);
				keypad_dataf.line_1 = *keypad_PIN & ((1 << KEYPADDATA_1) | (1 << KEYPADDATA_2) | (1 << KEYPADDATA_3) | (1 << KEYPADDATA_4));
				HL = KEYPADhl(keypad_datai.line_1, keypad_dataf.line_1);
				keypad_datai.line_1 = keypad_dataf.line_1;
				if(HL){
					// decode index line one column what ?
					if(HL == (1 << KEYPADDATA_1))
						c = keypadvalue[0][0];
					if(HL == (1 << KEYPADDATA_2))
						c = keypadvalue[0][1];
					if(HL == (1 << KEYPADDATA_3))
						c = keypadvalue[0][2];
					if(HL == (1 << KEYPADDATA_4))
						c = keypadvalue[0][3];
				}
				*keypad_DDR &= ~(1 << KEYPADLINE_1);
				*keypad_PORT |= (1 << KEYPADLINE_1);
			break;
			case 1: // line 2 index 1
				*keypad_DDR |= (1 << KEYPADLINE_2);
				*keypad_PORT &= ~(1 << KEYPADLINE_2);
				keypad_dataf.line_2 = *keypad_PIN & ((1 << KEYPADDATA_1) | (1 << KEYPADDATA_2) | (1 << KEYPADDATA_3) | (1 << KEYPADDATA_4));
				HL = KEYPADhl(keypad_datai.line_2, keypad_dataf.line_2);
				keypad_datai.line_2 = keypad_dataf.line_2;
				if(HL){
					// decode index line two column what ?
					if(HL == (1 << KEYPADDATA_1))
						c = keypadvalue[1][0];
					if(HL == (1 << KEYPADDATA_2))
						c = keypadvalue[1][1];
					if(HL == (1 << KEYPADDATA_3))
						c = keypadvalue[1][2];
					if(HL == (1 << KEYPADDATA_4))
						c = keypadvalue[1][3];
				}
				*keypad_DDR &= ~(1 << KEYPADLINE_2);
				*keypad_PORT |= (1<<KEYPADLINE_2);
			break;
			case 2: // line 3 index 2
				*keypad_DDR |= (1 << KEYPADLINE_3);
				*keypad_PORT &= ~(1 << KEYPADLINE_3);
				keypad_dataf.line_3 = *keypad_PIN & ((1 << KEYPADDATA_1) | (1 << KEYPADDATA_2) | (1 << KEYPADDATA_3) | (1 << KEYPADDATA_4));
				HL = KEYPADhl(keypad_datai.line_3, keypad_dataf.line_3);
				keypad_datai.line_3 = keypad_dataf.line_3;
				if(HL){
					// decode index line three column what ?
					if(HL == (1 << KEYPADDATA_1))
						c = keypadvalue[2][0];
					if(HL == (1 << KEYPADDATA_2))
						c = keypadvalue[2][1];
					if(HL == (1 << KEYPADDATA_3))
						c = keypadvalue[2][2];
					if(HL == (1 << KEYPADDATA_4))
						c = keypadvalue[2][3];
				}
				*keypad_DDR &= ~(1 << KEYPADLINE_3);
				*keypad_PORT |= (1 << KEYPADLINE_3);
			break;
			case 3: // line 4 index 3
				*keypad_DDR |= (1 << KEYPADLINE_4);
				*keypad_PORT &= ~(1 << KEYPADLINE_4);
				keypad_dataf.line_4 = *keypad_PIN & ((1 << KEYPADDATA_1) | (1 << KEYPADDATA_2) | (1 << KEYPADDATA_3) | (1 << KEYPADDATA_4));
				HL = KEYPADhl(keypad_datai.line_4, keypad_dataf.line_4);
				keypad_datai.line_4 = keypad_dataf.line_4;
				if(HL){
					// decode index line four column what ?
					if(HL == (1 << KEYPADDATA_1))
						c = keypadvalue[3][0];
					if(HL == (1 << KEYPADDATA_2))
						c = keypadvalue[3][1];
					if(HL == (1 << KEYPADDATA_3))
						c = keypadvalue[3][2];
					if(HL == (1 << KEYPADDATA_4))
						c = keypadvalue[3][3];
				}
				*keypad_DDR &= ~(1 << KEYPADLINE_4);
				*keypad_PORT |= (1 << KEYPADLINE_4);
			break;
			default:
			break;
		}
	}
	return c;
}
// read
void KEYPAD_read(void)
{
	char c;
	c = KEYPAD_getkey(); // returns null ('\0') all the time when no entry
	if(c){
		data.character = c;
		if(c == KEYPADENTERKEY){
			KEYPAD_string[KEYPADSTRINGINDEX] = '\0';
			KEYPADSTRINGINDEX = 0;
			data.print = endstr;
			data.string = KEYPAD_string; // shift output
		}else{
			if(KEYPADSTRINGINDEX < KEYPADSTRINGSIZE){
				KEYPAD_string[KEYPADSTRINGINDEX] = c;
				KEYPADSTRINGINDEX++;
				KEYPAD_string[KEYPADSTRINGINDEX] = '\0';
				data.print = KEYPAD_string;
				data.string = endstr; // clear output
			}else{
				KEYPADSTRINGINDEX = 0;
				KEYPAD_string[KEYPADSTRINGINDEX] = c;
				KEYPADSTRINGINDEX++;
				KEYPAD_string[KEYPADSTRINGINDEX] = '\0';
				data.print = KEYPAD_string;
				data.string = endstr; // clear output
			}
		}
	}
}
// read
keypadata KEYPAD_data(void)
{
	return data;
}
// flush
void KEYPAD_flush(void)
{
	KEYPADSTRINGINDEX = 0;
	data.character = ' ';
	data.print = endstr;
	data.string = endstr;
}
// lh
uint8_t KEYPADlh(uint8_t xi, uint8_t xf)
{
	uint8_t i;
	// printf("KEYPADlh\n");
	i = xf ^ xi;
	i &= xf;
	return i;
}
// hl
uint8_t KEYPADhl(uint8_t xi, uint8_t xf)
{
	uint8_t i;
	// printf("KEYPADhl\n");
	i = xf ^ xi;
	i &= xi;
	return i;
}

/*** EOF ***/

