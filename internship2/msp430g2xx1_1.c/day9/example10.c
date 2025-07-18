/*
motor x ve motor y var bunların her birine farklı güç kaynağı ile güç ver (min 20v max 50v) 
sonra p1.0 pul+ p1.1 dır+ bağla x motoru için 
y motoru için p1.6 pul+ p1.7 dır+ bağla
biz motor1 (x)'e 20v verdik 
motor2 (y)'ye ise 25v
breadboard üzerinden, msp430'dan çıkan gnd ucunu her iki motora ortak bağladık(dır- ve pul-, ama dır-'den pul-'ye bir ara kablo var, o yüzden sadece dır-'ye bağlarsak her ikisine gider)
cwd556 motoru için her 200 step 1.8 derece yapıyor, motorun tam tur(360 derece) atması için 40000 step atması lazım ve hızını 5 delay_us yaptık 
for kullanacaksak i<tur_sayisi yapmamız gerekli, kaç tur dönmesini istersek o sayısı tur_sayisi yerine manuel yazalım 
1 Mhz =1000000 cycle = 1 saniye --> ayarlanabilir
crtl+alt+(aşağı veya yukarı)--> birden fazla satırdaki yerleri değiştirme kısa yolu
*/

#include <msp430.h>

#define PUL1_PIN BIT0  // P1.0 → Motor1(x) PUL+
#define DIR1_PIN BIT1  // P1.1 → Motor1(x) DIR+
#define PUL2_PIN BIT6  // P1.6 → Motor2(y) PUL+
#define DIR2_PIN BIT7  // P1.7 → Motor2(y) DIR+

void delay_us(unsigned int us) {
    while(us--) {
        __delay_cycles(1); // 1 MHz = 1 µs
    }
}

// Motor2 için pulse üret
void pulse_motor2(unsigned int steps, unsigned int speed_us) {
    unsigned int i;
    for ( i = 0; i < steps; i++) {
        P1OUT |= PUL2_PIN;
        delay_us(speed_us);
        P1OUT &= ~PUL2_PIN;
        delay_us(speed_us);
    }
}

// Motor1 için pulse üret
void pulse_motor1(unsigned int steps, unsigned int speed_us) {
    unsigned int i;
    for ( i = 0; i < steps; i++) {
        P1OUT |= PUL1_PIN;
        delay_us(speed_us);
        P1OUT &= ~PUL1_PIN;
        delay_us(speed_us);
    }
}



// İki motora aynı anda pulse üret 
// her 200 step 1.8 derece yapar bir tam tur(360) için 200*200 = 40000 step gerekir
void dual_pulse(unsigned int steps, unsigned int speed_us) {
    unsigned int i;
    for ( i = 0; i < steps; i++) {
        P1OUT |= PUL1_PIN | PUL2_PIN;
        delay_us(speed_us);
        P1OUT &= ~(PUL1_PIN | PUL2_PIN);
        delay_us(speed_us);
    }
}

void main(void) {
    int tur, i;
    WDTCTL = WDTPW | WDTHOLD;           // Watchdog kapat
    BCSCTL1 = CALBC1_1MHZ;              // 1 MHz DCO ayarı
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= PUL1_PIN | DIR1_PIN | PUL2_PIN | DIR2_PIN;

    //while (1) { //sonsuz döngü için for'ları kapat, while aç


        // Örnek: İki motoru aynı anda sola döndür
        //P1OUT &= ~(DIR1_PIN | DIR2_PIN);  
        //dual_pulse(40000, 5);
        //__delay_cycles(1000000); //çalıştıktan sonra 1sn bekler





        // Örnek: Sadece Motor1 saat yönü döner
        //P1OUT |= DIR1_PIN;
        //for (i = 0; i <2 ; i++) {
        //pulse_motor1(40000, 5);}
        //__delay_cycles(1000000); //çalıştıktan sonra 1sn bekler





        // Örnek: Sadece Motor1 ters saat döner
        //P1OUT &= ~DIR1_PIN;
        //for (tur = 0; tur < 5; tur++) {
        //pulse_motor1(40000, 5);}
        //__delay_cycles(1000000); //çalıştıktan sonra 1sn bekler





        // Örnek: Sadece Motor2 sağa döner
        P1OUT |= DIR2_PIN; 
        for (tur = 0; tur <8; tur++){
        pulse_motor2(40000, 5); }
        //__delay_cycles(1000000); //çalıştıktan sonra 1sn bekler




        //Örnek: Sadece Motor2 sola döner
        //P1OUT &= ~DIR2_PIN;
        //for (tur = 0; tur <2; tur++){
        //pulse_motor2(40000, 5);}
        //__delay_cycles(1000000);} //çalıştıktan sonra 1sn bekler

       
        
}