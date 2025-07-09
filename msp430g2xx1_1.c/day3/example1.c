#include <msp430.h>

void example1(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    P1DIR |= BIT0;      // LED çıkış
    P1DIR &= ~BIT3;     // Buton giriş
    P1REN |= BIT3;      // Pull-up/down aktif
    P1OUT |= BIT3;      // Pull-up

    while (1)
    {
        if ((P1IN & BIT3) == 0)   // Butona basıldı
        {
            __delay_cycles(20000);   // Debounce (zıplamayı önle)
            if ((P1IN & BIT3) == 0)
            {
                P1OUT ^= BIT0;       // LED toggle
                while ((P1IN & BIT3) == 0); // Buton bırakılana kadar bekle
            }
        }
    }
}
