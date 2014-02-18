/*
 * main.c
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void send_char(char i)
/*
 * put the char in send buffer and wait until it is transmitted
 */
{
	UDR0 = i;
//	while(!(UCSR0A&(1<<TXC0)));
}

void uart_init(unsigned int ubrr)
{
	{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
	}

}

ISR(USART_RX_vect)
{
	char a;
	a= UDR0;
	if (a == 'c')
	{
		PORTB &= ~(1<<PORTB0);
	}
	else
	{
		PORTB |= (1<<PORTB0);
	}
}

int main(void)
{
	DDRB = 0xFF;
	uart_init(MYUBRR);
	sei();

	send_char('h');
	send_char('e');
	send_char('l');
	send_char('l');
	send_char('o');

	while(1);

	return 0;
}

