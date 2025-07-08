#include <msp430.h>

void main(void){
  WDTCTL = WDTPW | WDTHOLD;
  P1DIR &=~BIT0;
  while (1) {
  P1DIR ^= BIT0;
  __delay_cycles(100000);
  }
}