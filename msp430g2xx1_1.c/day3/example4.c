#include <msp430.h>

static unsigned int randomSeed = 0;  // Rastgele tohum

void example4(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Watchdog kapat

    // LED pinlerini çıkış yap
    P1DIR |= 0xFF;              // P1.0 - P1.7 çıkış
    P1OUT &= ~0xFF;             // Hepsi sönük başla

    // Buton (P1.3) giriş ve pull-up
    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;

    while (1)
    {
        // Buton basılana kadar tohum üret (zaman geçtikçe artar)
        while ((P1IN & BIT3) != 0)
        {
            randomSeed++;
            __delay_cycles(1000);  // Biraz yavaşlat
        }

        __delay_cycles(20000);     // Debounce

        if ((P1IN & BIT3) == 0)    // Buton gerçekten basılıysa
        {
            unsigned int index = randomSeed % 8;  // 0–7 arasında sayı
            P1OUT &= ~0xFF;        // Tüm LED'leri kapat
            P1OUT |= (1 << index); // Rastgele LED'i yak

            // Buton bırakılana kadar bekle
            while ((P1IN & BIT3) == 0);
            randomSeed = 0; // Tohumu sıfırla
        }
    }
}
