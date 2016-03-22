/*
  Setup timer and serial.
  Poll ADC for "single conversion" every k ms and send over serial.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#define BAUD_PRESCALE (F_CPU / (9600 * 16UL)) - 1
#define Q_SIZE 10 //Q_SIZE * 2 * CHANNELS + 3 + 1 ~ Q_SIZE * 2
#define CHANNELS 1

volatile uint8_t m,h,d,s;
volatile int ms;
volatile uint8_t rFlag, wFlag, sFlag;

struct _data{
  /*
   * Sensed data packet. Currently using only 1 channel so it's trivial
  */
  int data[CHANNELS];
};

struct _status{//its h,m,s for testing
  uint8_t day, hour, min;
};

void updateStatus(struct _status *sts){
  sts->day = h;//its h,m,s for testing
  sts->hour = m;
  sts->min = s;
}

struct _Queue{
  struct _data logData[Q_SIZE];
  struct _status lastSync, lastSense;
  uint8_t sz, front;
};

void initQ(struct _Queue *logs){
  logs->front = 0;
  logs->lastSync = {-1, -1, -1};
  logs->lastSense = {-1, -1, -1};
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
  updateStatus(&(logs->lastSense));
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
  /*
   * write the whole queue and then send timestamp of most recent reading,
   * the "logs.lastSense".
   * Note when this transaction happened in "logs.lastSync"
  */
  _data *dataPack;
  uint8_t i;
  while (logs->sz){
    dataPack = Remove(logs);
    for (i=0; i<CHANNELS; i++)
      if ( !port_w(2, dataPack->data+i) ) ;//error, do something
  }
  uint8_t a[3] = {logs->lastSense.day, logs->lastSense.hour, logs->lastSense.min};
  port_w(3, a);
  updateStatus(&(logs->lastSync));
}

int port_w(int sz, void *data){
  while ((UCSRA & (1<<UDRE)) == 0) {;}
  UDR = sz;
  uint8_t i;
  if (sz == 2){
    while ((UCSRA & (1<<UDRE)) == 0) {;}
    UDR = *((int *)data);
    while ((UCSRA & (1<<UDRE)) == 0) {;}
    UDR = *((int *)data)>>8;
  }
  else
    for (i=0; i<sz; i++){
      while ((UCSRA & (1<<UDRE)) == 0) {;}
      UDR = *((uint8_t*)data+i);
    }
  return 0;
}

void parse(char *rx){
  switch (*rx){
  case 'd': //user wants to download LOG
    wFlag = 1;
    break;
  }
}

ISR (TIMER2_COMP_vect){
  ms += 2;
  PORTB = 0;
  rFlag = 1;
  //if (ms%500 == 0) sFlag = 1;
  if (ms==1000){
    sFlag = 1;
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
  char rx, tx;
  _data senseD;
  //set sleep mode to idle
  MCUCR |= 1<<SE;
  //setup timer
  TCNT2 = 0;
  OCR2 = 0xF9; //2 msec (250 states)
  TIMSK |= 1<<OCIE2; //enable compare match with OCR2
  //setup serial intf.
  UCSRB |= (1<<RXEN) | (1<<TXEN);
  UCSRC |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
  UBRRH = (BAUD_PRESCALE)>>8;
  UBRRL = BAUD_PRESCALE;
  TCCR2 |= (1<<WGM21) | (1<<CS22) | (1<<CS20); //set CTC and 128 prescaler
  //PORTB LED for visual feedback
  DDRB |= (1<<5);

  struct _Queue logs;
  initQ(&logs);

  sei();
  while (1){
    if (wFlag){
      WriteQ(&logs);
      wFlag = 0;
    }
    if (rFlag){
      if (UCSRA & (1<<RXC)){
        if (UCSRA & (1<<FE) || UCSRA & (1<<DOR) || UCSRA & (1<<PE)) ;//resend pls!!
        else{
          //actual read instructions
          rx = UDR;
          parse(&rx);
        }
      }
      rFlag=0;
    }
    if (sFlag){
      PORTB |= 1<<5;
      ADMUX |= (1<<REFS0); //Use AVcc as Vref
      ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
      //Enable and 128 prescaler
      ADMUX &= ~(1<<MUX0|1<<MUX1|1<<MUX2|1<<MUX3); //select ADC0 pin
      while (ms < 500); //give time
      ADCSRA |= (1<<ADSC); //Start conversion
      while (ADCSRA & (1<<ADSC)); //wait till its done
      senseD.data[0] = (ADCL + (ADCH<<8)); //pack it in senseD
    /*
      ADMUX &= ~(1<<MUX1|1<<MUX2|1<<MUX3); //select ADC1 pin
      ADCSRA |= (1<<ADSC); //Start conversion
      while (ADCSRA & (1<<ADSC)); //wait till its done
      senseD.data[1] = (ADCL + (ADCH<<8)); //pack it in senseD
      .
      .
      other channels
    */
      ADCSRA &= ~(1<<ADEN); //turn ADC off
      Insert(&logs, &senseD); //store in log
      sFlag = 0;
    }
  sleep_cpu();
  }
  return 0;
}
