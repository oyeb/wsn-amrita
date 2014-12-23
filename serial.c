/*
continuously writes an int variable
*/
#include<avr/io.h>
#include<avr/interrupt.h>
#define BAUD_PRESCALE (F_CPU / (9600 * 16UL)) - 1

volatile uint8_t m,h,d,s,wFlag;
volatile int ms,;

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
  int count=1024;
  TCNT2 = 0;
  OCR2 = 0xF9; //2 msec (250 states)
  TIMSK |= 1<<OCIE2; //enable compare match with OCR2
  UCSRB |= (1<<RXEN) | (1<<TXEN);
  UCSRC |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
  UBRRH = (BAUD_PRESCALE >> 8);
  UBRRL = BAUD_PRESCALE;
  TCCR2 |= (1<<WGM21) | (1<<CS22) | (1<<CS20); //set CTC and 128 prescaler
  DDRB = 1<<5;
  PORTB = 1<<5;
  sei();
  while (1){
    if (wFlag == 1){
      while ((UCSRA & (1<<UDRE)) == 0) {;}
      UDR = 2;
      while ((UCSRA & (1<<UDRE)) == 0) {;}
      UDR = count;
      while ((UCSRA & (1<<UDRE)) == 0) {;}
      UDR = count>>8;
      count--;
      wFlag=0;
    }
  }
  return 0;
}
