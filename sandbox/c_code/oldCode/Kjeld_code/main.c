//============================================================================
// Name        : main.c
// Author      : Leon Bonde Larsen
// Version     : 1.0
// Copyright   : Open Source
// Description : Turn onboard LED on and off via USART, C Ansi-style
//============================================================================
//=====   INCLUDES   =====
#include    <avr/interrupt.h>       //er nødvendig for at benytte interrupts
#include    <avr/io.h>              //indeholder definitioner for ATmega88
//#include    "uart.h"                //indeholder praktiske funktioner til USART
//#include 	"led.h"

#include    "avr_serial.h"
#include 	"fmctl_def.h"	//indeholder praktiske funktioner til LED
int flag = 1;
//=====   MAIN ENTRY POINT   =====
int main()
{
	EXT_LED_INIT;
	EXT_LED_OFF;
	INT_LED_INIT;
	unsigned char input;
	serial_init();				//initialiserer USART
	unsigned int counter = 0;
	char c = 'c';                    //initialiserer lysdioden
	unsigned char s[7];
	sei();                          //starter interrupt funktionaliteten
	while(1) {
		//toggle_led();

		s[0] = 'h';
		s[1] = 'e';
		s[2] = 'l';
		s[3] = 'l';
		s[4] = 'o';
		s[5] = '\n';
		s[6] = '\0';

		for (int i = 0; i<1000;i++) {
			if (i == 999)
				counter++;
		}

//		input = serial_rx();
		serial_tx_string(s);
//		serial_tx_direct(c);
//		serial_tx_direct('\n');
//		serial_tx_direct('e');
////		serial_tx_direct('\n');
//		serial_tx('b');
//		serial_tx('\n');

		//    	    		if (serial_rx_avail()) {
		//    	    			EXT_LED_ON;
		////    	    			flag = 100;
		//    	    		}
		//INT_LED_OFF;
		if (counter > 1000)
		{
			//    		input = serial_rx();
//			if (input == 'g') {
//				EXT_LED_ON;
//				flag = 100;
//			}
//			serial_tx_direct(c);
			if (flag == 1) {
				INT_LED_OFF;
				flag = 0;
			}
			else if (flag == 0) {
				INT_LED_ON;
				flag = 1;
			}
			counter = 0;
		}
		//  	_delay_ms(1000);                       //fastholder microcontrolleren i en evighedsløkke
	}
}

/*ISR (USART_RX_vect) {
	flag = 100;
}*/

//=====   INTERRUPT SERVICE ROUTINES   =====
/*ISR (USART_RX_vect)                 //det program der afvikles ved interrupt
{
	//toggle_led();
    char rx= UDR0 - '0';//den modtagede karakter konverteret til tal
    serial_tx(rx);
    //led(rx);                        //tænder eller slukker lysdioden
}

ISR (USART_TX_vect)	//  Transmit interrupt
{

}


//End of file
 */

