/*
  Setup timer and serial.
  Poll ADC for "single conversion" every k ms and send over serial.
*/
#include<avr/io.h>
#include<avr/interrupt.h>
#define BAUD_PRESCALE (F_CPU / (9600 * 16UL)) - 1

volatile uint8_t m,h,d,s;
volatile int ms;
volatile uint8_t rFlag, wFlag, sFlag;

ISR (TIMER2_COMP_vect){
  ms += 2;
  sFlag = 1;
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
          d+1;//day is free-running, never resets!! except *hardware* reset
        }
      }
    }
  }
}

int port_w(int sz, int *data){
 while ((UCSRA & (1<<UDRE)) == 0) {;}
 UDR = sz;
 uint8_t i;
 for (i=0; i<2; i++){
   while ((UCSRA & (1<<UDRE)) == 0) {;}
   UDR = *(data)>>(i*8);
 }
 return 0;
}

int main(){
  int count = 0;
  char rx, tx;
  TCNT2 = 0;
  OCR2 = 0xF9; //2 msec (250 states)
  TIMSK |= 1<<OCIE2; //enable compare match with OCR2
  UCSRB |= (1<<RXEN) | (1<<TXEN);
  UCSRC |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
  UBRRH = (BAUD_PRESCALE >> 8);
  UBRRL = BAUD_PRESCALE;
  TCCR2 |= (1<<WGM21) | (1<<CS22) | (1<<CS20); //set CTC and 128 prescaler
  DDRB = 1<<5;
  //PORTB = 1<<5;
  sei();
  while (1){
    if (wFlag){
      port_w(2, &count);
      wFlag = 0;
    }
    if (rFlag){
      if (UCSRA & (1<<RXC)){
        if (UCSRA & (1<<FE) || UCSRA & (1<<DOR) || UCSRA & (1<<PE)) ;//resend pls!!
        else{
          //actual read instructions
          rx = UDR;
          //parse(rx);
          //count = 1024;
        }
      }
      rFlag=0;
    }
    if (sFlag){
      ADMUX |= (1<<REFS0); //Use AVcc as Vref
      ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
      //Enable and 128 prescaler
      ADCSRA |= (1<<ADSC); //Start conversion
      while (ADCSRA & (1<<ADSC));
      count = (ADCL + (ADCH<<8));
      wFlag = 1;
      sFlag = 0;
      ADCSRA &= ~(1<<ADEN); //turn it off
    }
  }
  return 0;
}
