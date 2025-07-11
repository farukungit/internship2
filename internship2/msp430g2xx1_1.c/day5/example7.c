#include <msp430.h>

#define TRIG BIT4
#define ECHO BIT5

int Delay = 25;  // Sabit hız

void port1_init(void);
void spin(void);
void timer(int n);
unsigned int measure_distance(void);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    port1_init();

    while (1)
    {
        unsigned int distance = measure_distance();  // cm cinsinden ölç

        if (distance <= 10)
        {
            spin();  // Cisim yakınsa motor dönsün
        }
        else
        {
            P1OUT = 0x00;  // Motoru durdur
        }
    }
}

void port1_init(void)
{
    P1DIR |= 0x0F;         // P1.0–P1.3 motor çıkışı
    P1OUT &= ~0x0F;

    P1DIR |= TRIG;         // TRIG çıkış
    P1DIR &= ~ECHO;        // ECHO giriş
    P1OUT &= ~TRIG;

    P1SEL = 0x00;
    P1REN = 0x00;
    P1IE = 0x00;
}

void timer(int n)
{
    while (n-- != 0)
        __delay_cycles(100);
}

unsigned int measure_distance(void)
{
    unsigned int time = 0;

    P1OUT |= TRIG;
    __delay_cycles(10);        // 10 µs TRIG
    P1OUT &= ~TRIG;

    while ((P1IN & ECHO) == 0);  // ECHO HIGH olana kadar bekle
    while ((P1IN & ECHO) != 0)
    {
        __delay_cycles(1);
        time++;
        if (time > 60000) break;  // timeout önlemi
    }

    return time / 58;   // Mikro saniye → cm dönüşümü
}

void spin(void)
{
    const unsigned char half_cw[8] = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08};
    unsigned int step;

    for (step = 0; step < 8; step++)
    {
        P1OUT = half_cw[step];
        timer(Delay);  // Sabit hızla döndür
    }

    P1OUT = 0x00;  // Dönüş bitince dur
}
