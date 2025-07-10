// HC-SR04, ultrasonik mesafe sensörü ile bir cisime yakaştıkça led 1.0'ın sönesini sağlayan kod. 


#include <msp430.h>

#define TRIG BIT0   // P1.0
#define ECHO BIT1   // P1.1
#define LED  BIT2   // P1.2

void delay_us(unsigned int us) {
    while (us--) __delay_cycles(1); // 1 µs @ 1MHz
}

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= TRIG + LED;
    P1OUT &= ~(TRIG + LED);
    P1DIR &= ~ECHO;

    volatile unsigned int time, distance, timeout;

    while(1)
    {
        // 1. Tetikle
        P1OUT |= TRIG;
        delay_us(10);
        P1OUT &= ~TRIG;

        // 2. ECHO HIGH olana kadar bekle (timeout ile)
        timeout = 30000;
        while ((P1IN & ECHO) == 0 && timeout--) {
            __delay_cycles(1);
        }

        // 3. Eğer timeout sıfıra inerse sensör çalışmadı
        if (timeout == 0) {
            P1OUT &= ~LED;  // LED'i söndür
            continue;       // ölçüm atla, tekrar dene
        }

        // 4. ECHO süresi ölç
        time = 0;
        timeout = 30000;
        while ((P1IN & ECHO) != 0 && timeout--) {
            __delay_cycles(1);
            time++;
        }

        if (timeout == 0) {
            P1OUT &= ~LED;  // Süre çok uzadıysa yine söndür
            continue;
        }

        // 5. Mesafe hesapla
        distance = time / 58;

        // 6. Mesafe kontrol
        if (distance < 20)
            P1OUT |= LED;
        else
            P1OUT &= ~LED;

        __delay_cycles(50000); // kısa bekleme
    }
}
