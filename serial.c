/*
System Clock 16MHz sourced from external Crystal Oscillator
  which is on-board.
Implements USART (transit only) in the following Data Packet Format:
  <size>		sz	1 byte
  <data0>		data0	1 byte
  .		.	.
  .		.	.
  <data{sz-1}>	.	.
Thus totally {sz+1} bytes are required.
This format could be easily changed to *fixed size* format, if needed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
USES:
  PORTD 0,1 (PORTD0-1){RX and TX}
  >>>>as USART pins
----------------------------------------------------------------------
The program writes 'count' every 2msec (when wFlag is set) and
decrements it.
*/
#include<avr/io.h>
#include<avr/interrupt.h>
#define BAUD_PRESCALE (F_CPU / (9600 * 16UL)) - 1

volatile uint8_t m,h,d,s,wFlag;
volatile int ms;

ISR (TIMER2_COMP_vect){
  ms += 2;
  wFlag = 1;
  if (ms==1000){
    ms = 0;
    s++;
    if (s==60){
      s = 0;
      m++;
      if (m==60){
        m = 0;
        h++;
        if (h==24){
          h=0;
          d+1;//day is free-running, never resets!!
        }
      }
    }
  }
}

int main(){
  int count=1024; // default int size is 2 bytes
  TCNT2 = 0;
  OCR2 = 0xF9; //2 msec (250 states)
  TIMSK |= 1<<OCIE2; //enable compare match with OCR2
  UCSRB |= (1<<RXEN) | (1<<TXEN); //enable USART
  UCSRC |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1); // set data packet sz to 8bits
  UBRRH = (BAUD_PRESCALE >> 8); // set communication bit(baud) rate
  UBRRL = BAUD_PRESCALE;
  TCCR2 |= (1<<WGM21) | (1<<CS22) | (1<<CS20); //set CTC and 128 prescaler
  DDRB = 1<<5;
  PORTB = 1<<5;
  sei();
  while (1){
    if (wFlag == 1){
      while ((UCSRA & (1<<UDRE)) == 0) {;} // wait till UDR is ready
      UDR = 2;
      /* UDR is the Usart Data Register
      */
      while ((UCSRA & (1<<UDRE)) == 0) {;}
      UDR = count;
      while ((UCSRA & (1<<UDRE)) == 0) {;}
      UDR = count>>8;
      count--;
      wFlag=0; // reset flag
    }
  }
  return 0;
}
