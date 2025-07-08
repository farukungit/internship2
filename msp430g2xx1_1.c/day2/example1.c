//binary sayaç 1.0->1.6 
#include <msp430.h>

int example1(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Watchdog timer kapat

    P1DIR |= 0x7F;  // P1.0 - P1.6 çıkış (0x7F = 0111 1111)
    P1OUT = 0;      // LED’leri başta kapat

    unsigned int i;
    while(1)
    {
        for(i = 0; i < 128; i++)  // 0’dan 127’ye kadar say
        {
            P1OUT = i;           // Binary olarak LED’lere yaz
            __delay_cycles(500000); // Yarım saniye bekle (1MHz)
        }
    }
}