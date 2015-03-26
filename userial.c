#define __AVR_LIBC_DEPRECATED_ENABLE__

/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Ripped apart 24/3/15 by arrow-

*/
#include "userial.h"
// this next line disables the entire HardwareSerial.cpp,
// this is so I can support Attiny series and any other chip without a uart
#if defined(UBRRH) || defined(UBRR0H) || defined(UBRR1H) || defined(UBRR2H) || defined(UBRR3H)
/*
 * on ATmega8, the uart and its bits are not numbered, so there is no "TXC0"
 * definition.
 */

/*
  ATmega8 [you need to use URSEL in UCSRC to be able to write to UCSRC]
          [refer datasheet. This is only with ATmega8 {as far as I know}]
  * UCSRA : RXC TXC UDRE FE DOR PE U2X MPCM
  * UCSRB : RXCIE TXCIE UDRIE RXEN TXEN UCSZ2 RXB8 TXB8
  * UCSRC : URSEL UMSEL UPM1 UPM0 USBS UCSZ1 UCSZ0 UCPOL
  * UBRRH, UBRRL

  ATmega640/1280/2560 have 4 ports. [This lib uses port2]
  * UCSRnA: RXCn TXCn UDREn FEn DORn UPEn U2Xn MPCMn
  * UCSRnB: RXCIEn TXCIEn UDRIEn RXENn TXENn UCSZn2 RXB8n TXB8n
  * UCSRnC: UMSELn1 UMSELn0 UPMn1 UPMn0 USBSn UCSZn1 UCSZn0 UCPOLn
  * UBRRHn, UBRRLn

  ATmega48/88/168 have 1 port [with n=0]
  * UCSR0A: RXC0 TXC0 UDRE0 FE0 DOR0 UPE0 U2X0 MPCM0
  * UCSR0B: RXCIE0 TXCIE0 UDRIE0 RXEN0 TXEN0 UCSZ02 RXB80 TXB80
  * UCSR0C: UMSEL01 UMSEL00 UPM01 UPM00 USBS0 UCSZ01 UCSZ00 UCPOL0
  * UBRRH0, UBRRL0
*/

void portInit(uint16_t baud_rate){
  uint16_t baud_prescale = F_CPU / (baud_rate * 16UL) - 1;
#if defined(TXC) && defined(UBRRH) //this is Atmega8
  UCSRB |= (1<<RXEN) | (1<<TXEN);
  UCSRC |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
  UBRRH = (baud_prescale >> 8);
  UBRRL = baud_prescale;

#elif defined(UBRR0H) && !defined(UBRR2H) //this could be 168
  UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
  UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
  UBRR0H = (baud_prescale >> 8);
  UBRR0L = baud_prescale;

#elif defined(UBRR2H) //this could be 2560
  UCSR2B |= (1<<RXEN2) | (1<<TXEN2);
  UCSR2C |= (1<<UCSZ20) | (1<<UCSZ21);
  UBRR2H = (baud_prescale >> 8);
  UBRR2L = baud_prescale;
#endif
}

int port_w(int sz, void *data){
  int i;
#if defined(UBRRH)
  for (i=0; i<sz; i++){
    while ((UCSRA & (1<<UDRE)) == 0) {;}
    UDR = *((uint8_t*)data+i);
  }
#endif
  return 0;
}
#endif // whole file

