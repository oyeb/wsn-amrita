#include "timer.h"

void _initTimer(){
  //setup TIMER2 (8bit) in CTC
  TCNT2 = 0;
  OCR2 = 0xF9; //2 msec (250 states)
  TIMSK |= 1<<OCIE2; //enable compare match with OCR2
  TCCR2 |= (1<<WGM21) | (1<<CS22) | (1<<CS20); //set CTC and 128 prescaler
}

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
          d+1;//day is free-running, never resets!!
        }
      }
    }
  }
}
