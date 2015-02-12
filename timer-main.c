#include <avr/io.h>
#include <avr/sleep.h>
#include "utimer.h"

int main(){
  //sleep-mode == idle mode
  MCUCR |= 1<<SE;
  _initTimer();
  //timer has started!
  PORTB = 1<<5;//LED initial on
  DDRB = 1<<5;
  DDRD = 0xFC;
  sei();
  while (1){
    sleep_cpu();//always
  }
}
