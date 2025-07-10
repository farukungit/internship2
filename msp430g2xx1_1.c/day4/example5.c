// msp ile step motoru döndürme

#include <msp430.h>

#define IN1 BIT0
#define IN2 BIT1
#define IN3 BIT2
#define IN4 BIT3
unsigned char steps[8] = {
    0b00001001,  // IN1
    0b00000001,  // IN1 + IN2
    0b00000011,  // IN2
    0b00000010,  // IN2 + IN3
    0b00000110,  // IN3
    0b00000100,  // IN3 + IN4
    0b00001100,  // IN4
    0b00001000   // IN4 + IN1
};

int j;
void step_anticlockwise() {
    for (j = 7; j >= 0; j--) {
        P1OUT = (P1OUT & ~(IN1 | IN2 | IN3 | IN4)) | steps[j];
        __delay_cycles(2500);
    }
}

void example5(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= IN1 | IN2 | IN3 | IN4;
    P1OUT = 0;

    while (1) {
        step_anticlockwise();
    }
}