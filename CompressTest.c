/*
can write only char data
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#define BAUD_PRESCALE (F_CPU / (9600 * 16UL)) - 1

volatile uint8_t m,h,d,s;
volatile int ms;
volatile uint8_t Flags, Enables = 7;
//xxxx xswr sense, write, read
//xxxx xswr

ISR (TIMER2_COMP_vect){
  ms += 2;
  if (ms==1000){
    ms = 0;
    Flags = Enables;
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

int port_w(uint8_t sz, int *data){
 while ((UCSRA & (1<<UDRE)) == 0) {;}
 UDR = sz;
 uint8_t i;
 for (i=0; i<sz; i++){
   //low byte first
   while ((UCSRA & (1<<UDRE)) == 0) {;}
   UDR = *(data)>>(i*8);
 }
 return 0;
}

void parse(char op){
  switch (op){
    case 'e':
      Enables &= ~(1<<1);
      break;
    case 's':
      Enables |= (1<<1);
      break;
  }
}

int main(){
  int data;
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
    if (Flags&(1<<1)){
      data = (h<<12)+(m<<6)+s;
      port_w(2, &data);
      Flags &= ~(1<<1);
    }
    if ( Flags&1 ){
      if (UCSRA & (1<<RXC)){
        if (UCSRA & (1<<FE) || UCSRA & (1<<DOR) || UCSRA & (1<<PE)) ;//resend pls!!
        else{
          //actual read instructions
          rx = UDR;
          parse(rx);
        }
      }
      Flags &= ~(1<<0);
    }
  }
  return 0;
}
