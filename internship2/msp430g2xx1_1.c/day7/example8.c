#include <msp430.h>

#define ON 1
#define OFF 0
#define CW 1        // Saat yönü
#define CCW 0       // Saat yönü tersi
#define FULL 1
#define HALF 0

int Delay = 25;
int Rotation = HALF;  // Yarım adım: daha yumuşak dönüş

void port1_init(void);
void timer(int n);
void spin_one_rev(int direction);  // Yön belirten fonksiyon

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    port1_init();

    while (1)
    {
        spin_one_rev(CCW);  // 1 tur sola
        __delay_cycles(100000);

        spin_one_rev(CW);   // 1 tur sağa
        __delay_cycles(100000);
    }
}

void port1_init(void)
{
    P1OUT = 0x00;
    P1SEL = 0x00;
    P1DIR = 0xFF;
    P1REN = 0x00;

    P1IES = 0x00;
    P1IFG = 0x00;
    P1IE = 0x00;
}

void timer(int n)
{
    while (n-- != 0)
        __delay_cycles(100);
}

void spin_one_rev(int direction)
{
    // Yarım adım dizileri (8 adım)
    const unsigned char half_cw[8]  = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08};
    const unsigned char half_ccw[8] = {0x08, 0x0C, 0x04, 0x06, 0x02, 0x03, 0x01, 0x09};

    int step;
    int i;
    
    // 4096 yarım adım = 360 derece dönüş (tam tur)
    for (i = 0; i < 4096; i++)
    {
        step = i % 8;  // 0-7 arası döngü
        if (direction == CW)
            P1OUT = half_cw[step];
        else
            P1OUT = half_ccw[step];

        timer(Delay);
    }

    P1OUT = 0x00;  // Motoru durdur
}