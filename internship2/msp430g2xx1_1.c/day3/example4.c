// lede gönderilen sinyalin şiddetini ve süresini kontrol etmek için bir örnek 
#include <msp430.h>

void example4(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // Watchdog Timer kapat

    // P1.6 çıkış olarak ayarlandı (TA0.1 çıkışı burada)
    P1DIR |= BIT6;
    P1SEL |= BIT6;   // P1.6 pinini Timer_A çıkışına yönlendir (PWM için)

    // Timer_A ayarları
    TA0CCR0 = 100000 - 1;             // PWM periodu (toplam zaman)
    TA0CCTL1 = OUTMOD_7;           // CCR1 reset/set modu (PWM için uygun)
    TA0CCR1 = 100;                 // Duty cycle (0-1000 arasında), 500 = %50 parlaklık
    TA0CTL = TASSEL_2 + MC_1;      // Timer_A SMCLK, up mode

    while (1)
    {
        // Döngü içinde istenirse duty cycle artırılabilir/azaltılabilir
        // Örneğin bir animasyon yapılabilir burada
    }
}
