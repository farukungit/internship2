#include <msp430.h>

#define PUL_PIN BIT0  // P1.0 → PUL+
#define DIR_PIN BIT1  // P1.1 → DIR+

void delay_us(unsigned int us) {
    while(us--) {
        __delay_cycles(1); // 1 MHz'de 1 µs gecikme
    }
}
void pulse(unsigned int steps, unsigned int speed_us) {
    unsigned int i;

    for ( i = 0; i < steps; i++) {
        P1OUT |= PUL_PIN;       // PUL HIGH
        delay_us(speed_us);     // pulse width
        P1OUT &= ~PUL_PIN;      // PUL LOW
        delay_us(speed_us);     // pulse interval
    }
}

void example8(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Watchdog'u durdur
    BCSCTL1 = CALBC1_1MHZ;      // 1 MHz saat
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= PUL_PIN | DIR_PIN; // P1.0 ve P1.1 çıkış

    while (1) {
        // Sağa dön
        //P1OUT |= DIR_PIN;       // DIR HIGH
        //pulse(20000, 5);        // 200 adım, 500 us hız
        //__delay_cycles(1000000); // 1 saniye bekle

        // Sola dön
        P1OUT &= ~DIR_PIN;      // DIR LOW
        pulse(20000, 5);        // 200 adım
        //__delay_cycles(1000000); // 1 saniye bekle
    }
}
