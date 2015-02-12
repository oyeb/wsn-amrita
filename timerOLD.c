/*
System Clock 16MHz sourced from external Crystal Oscillator
  which is on-board.
Counter updated with SYS_clock(16MHz) prescaled by 128
  Freq_TIMER2 = 16MHz/128 = 125kHz
TIMER2 interrupts after every 250 clock pulses,
  1 pulse = 1/125kHz = 8 usec
  250 pulses         = 2 msec
ms, s, m, h, d are global TIME variables.
If need be, will implement a TIME struct in future.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
USES:
  PIN 2-7(PORTD2-7), 13(PORTB5)
  >>>as O/P
==========================================================
For visual response, 13th Arduino Digital I/O pin is set
as output to drive on-board LED. LED flashes briefly every
second and for 1 second every 60 secs.
If LEDs are connected to PIN2 - PIN7, one can see real time
value of variale 's' (PIN2 is LSB).
*/
#include<avr/io.h>
#include<avr/sleep.h>
#include<avr/interrupt.h>

volatile uint8_t m,h,d,s;
volatile int ms;

ISR (TIMER2_COMP_vect){
  ms += 2;
  if (s != 0 )
    PORTB = 0;
  if (ms==1000){
    ms = 0;
    s++;
    PORTD = s<<2;//update portD every second
    PORTB = 1<<5;
    if (s==60){
      s = 0;
      m++;
      if (m==60){
        m = 0;
        h++;
        if (h==24){
          h=0;
          d+=1;//day is free-running, never resets!!
        }
      }
    }
  }
}  

int main(){
  //sleep-mode == idle mode
  MCUCR |= 1<<SE;
  //setup TIMER2 (8bit) in CTC
  TCNT2 = 0;
  OCR2 = 0xF9; //2 msec (250 states)
  TIMSK |= 1<<OCIE2; //enable compare match with OCR2
  TCCR2 |= (1<<WGM21) | (1<<CS22) | (1<<CS20); //set CTC and 128 prescaler
  PORTB = 1<<5;//LED initial on
  //timer has started!
  DDRB = 1<<5;
  DDRD = 0xFC;
  sei();
  while (1){
    sleep_cpu();//always
  }
}
