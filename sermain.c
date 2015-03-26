#include <avr/io.h>
#include <avr/sleep.h>
#include "userial.h"

int main(){
  portInit(9600);
  char samba[] = "Sabka malik ek";
  port_w(14, samba);
  return 0;
}
