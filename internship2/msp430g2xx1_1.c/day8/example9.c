// her iki motorun aynı anda kontrolu ve ayrı ayrı kontrolu için gerekli kodlar 

#include <msp430.h>

#define PUL1_PIN BIT0  // P1.0 → Motor1 PUL+
#define DIR1_PIN BIT1  // P1.1 → Motor1 DIR+

#define PUL2_PIN BIT6  // P1.6 → Motor2 PUL+
#define DIR2_PIN BIT7  // P1.7 → Motor2 DIR+

void delay_us(unsigned int us) {
    while(us--) {
        __delay_cycles(1); // 1 MHz = 1 µs
    }
}

// İki motora aynı anda pulse üret
void dual_pulse(unsigned int steps, unsigned int speed_us) {
    unsigned int i;

    for (i = 0; i < steps; i++) {
        P1OUT |= PUL1_PIN | PUL2_PIN;    // Her iki motorun PUL HIGH
        delay_us(speed_us);
        P1OUT &= ~(PUL1_PIN | PUL2_PIN); // PUL LOW
        delay_us(speed_us);
    }
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Watchdog kapat
    BCSCTL1 = CALBC1_1MHZ;              // 1 MHz DCO ayarı
    DCOCTL = CALDCO_1MHZ;

    // Tüm pinleri çıkış yap
    P1DIR |= PUL1_PIN | DIR1_PIN | PUL2_PIN | DIR2_PIN;

    while (1) {
        // Her iki motoru sağa döndür
        P1OUT |= DIR1_PIN | DIR2_PIN;     // DIR HIGH (sağ)
        dual_pulse(50000, 5);             // 200 adım, 500 us
        __delay_cycles(1000000);          // 1 saniye bekle

        // Her iki motoru sola döndür
        P1OUT &= ~(DIR1_PIN | DIR2_PIN);  // DIR LOW (sol)
        dual_pulse(50000, 5);             // 200 adım
        __delay_cycles(1000000);          // 1 saniye bekle

        // Sağa dön x ekseni
        //P1OUT |= DIR1_PIN;       // DIR HIGH
        //pulse(20000, 5);        // 200 adım, 500 us hız
        //__delay_cycles(1000000); // 1 saniye bekle

        // Sola dön x ekseni
        //P1OUT &= ~DIR1_PIN;      // DIR LOW
        //pulse(20000, 5);        // 200 adım
        //__delay_cycles(1000000); // 1 saniye bekle

        // Sağa dön y ekseni
        //P1OUT |= DIR2_PIN;       // DIR HIGH
        //pulse(20000, 5);        // 200 adım, 500 us hız
        //__delay_cycles(1000000); // 1 saniye bekle

        // Sola dön y ekseni
        //P1OUT &= ~DIR2_PIN;      // DIR LOW
        //pulse(20000, 5);        // 200 adım
        //__delay_cycles(1000000); // 1 saniye bekle

    }
}
