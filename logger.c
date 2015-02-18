/*
  Setup timer and serial.
  Poll ADC for "single conversion" every k ms and send over serial.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#define BAUD_PRESCALE (F_CPU / (9600 * 16UL)) - 1
#define Q_SIZE 10
#define CHANNELS 1

volatile uint8_t m,h,d,s;
volatile int ms;
volatile uint8_t rFlag, wFlag, sFlag;

struct _data{
  int data[CHANNELS];
};

struct _status{
  uint8_t day, hour, min;
};

void updateStatus(struct _status *sts){
  sts->day = d;
  sts->hour = h;
  sts->min = m;
}

struct _Queue{
  struct _data logData[Q_SIZE];
  struct _status lastSync;
  uint8_t sz, front;
};

void initQ(struct _Queue *logs){
  logs->front = 0;
  logs->lastSync = {-1, -1, -1};
  logs->sz = 0;
}

void Insert(struct _Queue *logs, struct _data *val){
  if (logs->sz < Q_SIZE){
    uint8_t index = logs->front + logs->sz;
    if (index > Q_SIZE-1) index -= Q_SIZE;
    logs->logData[index] = *val;
    logs->sz++;
  }
  else{
    logs->logData[logs->front] = *val;
    logs->front = (logs->front == Q_SIZE-1)? 0 : logs->front+1;
  }
}

struct _data *Remove(struct _Queue *logs){
  if (logs->sz){
    _data *retVal = logs->logData + logs->front;
    logs->front = (logs->front == Q_SIZE-1)? 0 : logs->front+1;
    logs->sz--;
    return retVal;
  }
  else
    return NULL;
}

void WriteQ(struct _Queue *logs){
  _data *dataPack;
  uint8_t i;
  while (logs->sz){
    dataPack = Remove(logs);
    for (i=0; i<CHANNELS; i++)
      if ( !port_w(2, dataPack->data+i) ) ;//error, do something
  }
  updateStatus(&(logs->lastSync));
}

int port_w(int sz, int *data){
 while ((UCSRA & (1<<UDRE)) == 0) {;}
 UDR = sz;
 uint8_t i;
 for (i=0; i<sz; i++){
   while ((UCSRA & (1<<UDRE)) == 0) {;}
   UDR = *(data)>>(i*8);
 }
 return 0;
}

ISR (TIMER2_COMP_vect){
  ms += 2;
  PORTB = 0;
  if (ms%500 == 0) sFlag = 1;
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

int main(){
  uint8_t sCnt = 0;
  char rx, tx;
  _data senseD;
  MCUCR |= 1<<SE;
  TCNT2 = 0;
  OCR2 = 0xF9; //2 msec (250 states)
  TIMSK |= 1<<OCIE2; //enable compare match with OCR2
  UCSRB |= (1<<RXEN) | (1<<TXEN);
  UCSRC |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
  UBRRH = (BAUD_PRESCALE)>>8;
  UBRRL = BAUD_PRESCALE;
  TCCR2 |= (1<<WGM21) | (1<<CS22) | (1<<CS20); //set CTC and 128 prescaler
  DDRB |= (1<<5);
  
  struct _Queue logs;
  initQ(&logs);
  
  sei();
  while (1){
    /*if (wFlag){
      WriteQ(&logs);
    }*/
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
      PORTB |= 1<<5;
      ADMUX |= (1<<REFS0); //Use AVcc as Vref
      ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
      //Enable and 128 prescaler
      ADMUX &= ~(1<<MUX0|1<<MUX1|1<<MUX2|1<<MUX3);
      ADCSRA |= (1<<ADSC); //Start conversion
      while (ADCSRA & (1<<ADSC));
      senseD.data[0] = (ADCL + (ADCH<<8));
    /*
      ADMUX &= ~(1<<MUX1|1<<MUX2|1<<MUX3);
      ADCSRA |= (1<<ADSC); //Start conversion
      while (ADCSRA & (1<<ADSC));
      senseD.data[1] = (ADCL + (ADCH<<8));
      .
      .
      other channels
    */
      ADCSRA &= ~(1<<ADEN); //turn it off
      Insert(&logs, &senseD);
      sCnt++;
      if (sCnt > 5){
        WriteQ(&logs);
        sCnt = 0;
      }
      sFlag = 0;
    }
  sleep_cpu();
  }
  return 0;
}
