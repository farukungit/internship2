#include <msp430.h>

volatile unsigned int adc_value = 0;

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;           // Watchdog timer durdur
    BCSCTL1 = CALBC1_1MHZ;              // DCO = 1MHz
    DCOCTL = CALDCO_1MHZ;

    // --- LED için PWM ayarları (P1.6 - TA0.1) ---
    P1DIR |= BIT6;                      // P1.6 çıkış
    
    P1SEL |= BIT6;                      // P1.6 → Timer_A çıkışı (PWM)

    // Timer_A: Fast PWM (up mode)
    TA0CCR0 = 1023;                     // PWM periyodu (10 bit)
    TA0CCTL1 = OUTMOD_7;                // Reset/Set modu (PWM)
    TA0CCR1 = 0;                        // Başlangıçta parlaklık sıfır
    TA0CTL = TASSEL_2 + MC_1;           // SMCLK, up mode

    // --- ADC Ayarları ---
    ADC10CTL1 = INCH_0;                 // A0 seç (P1.0)
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON;

    while (1) {
        ADC10CTL0 |= ENC + ADC10SC;     // ADC ölçüm başlat
        while (ADC10CTL1 & ADC10BUSY);  // Bitene kadar bekle

        adc_value = ADC10MEM;           // ADC sonucunu al (0–1023)
        TA0CCR1 = adc_value;            // PWM duty cycle güncelle

        __delay_cycles(10000);          // Küçük bir bekleme
    }
    
}