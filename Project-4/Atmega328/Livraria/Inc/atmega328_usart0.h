/************************************************************************
	ATMEGA 328 USART0
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ATmega 328
Update:   01/07/2025
************************************************************************/
#ifndef _ATMEGA328_USART0_H_
	#define _ATMEGA328_USART0_H_

/*** Library ***/
#include "atmega328.h"
/***
Note F_CPU value is correct !
Note If Interrupt enabled make sure respective interrupt function is defined !
***/
/*** Constant & Macro ***/
#ifndef UART0_RX_BUFFER_SIZE
	#define UART0_RX_BUFFER_SIZE 32
#endif

#define UARTvar char

// Parity choices
#define NONE 0
#define EVEN 2
#define ODD 3

#if ( ( UART0_RX_BUFFER_SIZE ) >= ( SRAMEND ) - SRAMSTART )
	#error "size of UART0_RX_BUFFER_SIZE larger than size of SRAM"
#endif

#define UART_FRAME_ERROR		0x0800              /* Framing Error by UART        */
#define UART_OVERRUN_ERROR		0x0400              /* Overrun condition by UART    */
#define UART_BUFFER_OVERFLOW	0x0200              /* receive ring buffer overflow */
#define UART_NO_DATA			0x0100              /* no receive data available    */

/*** Handler ***/
typedef struct {
	// prototype pointers
	UARTvar (*read)(void);
	UARTvar (*getch)(void);
	UARTvar* (*gets)(void);
	void (*rxflush)(void);
	void (*write)(UARTvar data);
	void (*putch)(UARTvar c);
	void (*puts)(UARTvar* s);
}USART0;

USART0 usart0_enable( uint32_t baud, unsigned int FDbits, unsigned int Stopbits, unsigned int Parity );
USART0* usart0(void);

char* usart0_messageprint(USART0* uart, char* oneshot, char* msg, const char* endl);

#endif
/*** EOF ***/

