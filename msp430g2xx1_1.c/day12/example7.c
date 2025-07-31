#include <msp430.h>

#define PUL1_PIN BIT0  // P1.0 → Motor1(x) PUL+
#define DIR1_PIN BIT1  // P1.1 → Motor1(x) DIR+
#define PUL2_PIN BIT6  // P1.6 → Motor2(y) PUL+
#define DIR2_PIN BIT7  // P1.7 → Motor2(y) DIR+

int currentX = 0;  // 0-47 arası (her biri 0.5 cm)
int currentY = 0;  // 0-28 arası (her biri 0.5 cm)

void delay_us(unsigned int us) {
    while(us--) {
        __delay_cycles(1); // 1 MHz = 1 µs
    }
}

void delay_ms(unsigned int ms) {
    while (ms--) {
        __delay_cycles(1000); // Yaklaşık 1ms @1MHz
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
void dual_pulse(unsigned int steps, unsigned int speed_us) {
    unsigned int i;
    for ( i = 0; i < steps; i++) {
        P1OUT |= PUL1_PIN | PUL2_PIN;
        delay_us(speed_us);
        P1OUT &= ~(PUL1_PIN | PUL2_PIN);
        delay_us(speed_us);
    }
}

void sag(int a){
    int tur;
    P1OUT &= ~DIR1_PIN;
    for (tur = 0; tur < a; tur++) {
        currentX += 1;
        if(currentX<47){
        pulse_motor1(40000, 5);
        }
        else {
            currentX-=1;
            break;
        }
        
    }
}

void sol(int b){
    int tur;
    P1OUT |= DIR1_PIN;
    for (tur = 0; tur < b; tur++) {
        
         pulse_motor1(40000, 5);
    }
}

void geri(int c){
    int tur;
    P1OUT |= DIR2_PIN; 
    for (tur = 0; tur < c; tur++){
        currentY -= 1;
        
        pulse_motor2(40000, 5);
    }
       
}


void ileri(int d){
    int tur;
    P1OUT &= ~DIR2_PIN;
    for (tur = 0; tur < d; tur++){
         currentY += 1;
         if (currentY<=50) {
            pulse_motor2(40000, 5);
         }
         
       
    }
}

void sagUst(int e){
    int tur;
    P1OUT &= ~(DIR1_PIN | DIR2_PIN);
    for (tur = 0; tur < e; tur++) {
        currentX += 1;
        currentY += 1;
        if (currentX<=45&&currentY<=35) {
        dual_pulse(40000, 1);
        }
        
    }
}

void sagAlt(int e){
    int tur;
    P1OUT &= ~DIR1_PIN;
    P1OUT |= DIR2_PIN;
    for (tur = 0; tur < e; tur++) {
        currentX += 1;
        currentY -= 1;
        if (currentX<=45&&currentY>=0) {
        dual_pulse(40000, 1);
        }
     
    }
}

void solUst(int e){
    int tur;
    P1OUT |= DIR1_PIN;
    P1OUT &= ~DIR2_PIN;
    for (tur = 0; tur < e; tur++) {
         currentX -= 1;
        currentY += 1;
        if (currentX>=0&&currentY<=40) {
            dual_pulse(40000, 1);
        }

        
       
    }
}

void solAlt(int e){
    int tur;
    P1OUT |= (DIR1_PIN | DIR2_PIN);
    for (tur = 0; tur < e; tur++) {
         currentX -= 1;
        currentY -= 1;
        if (currentX>=0&&currentY>=0) {
            dual_pulse(40000, 1);
        }
        
    }
}

void yukari(){
    // Z motoru yukarı (kalemi kaldır)
    P1OUT |= BIT2;
    P1OUT &= ~BIT3;
    delay_ms(200);

    P1OUT &= ~(BIT2 | BIT3);
    delay_ms(200);    
}

void asagi(){
    // Z motoru aşağı (kalemi indir)
    P1OUT &= ~BIT2;
    P1OUT |= BIT3;
    delay_ms(200);

    P1OUT &= ~(BIT2 | BIT3);
    delay_ms(200);  
}

void sifiraDon(){
    sol(currentX);    // X = 0
    currentX = 0;

    geri(currentY);   // Y = 0
    currentY = 0;
}

void main(void) {
    int tur, i;
    WDTCTL = WDTPW | WDTHOLD;           // Watchdog kapat
    BCSCTL1 = CALBC1_1MHZ;              // 1 MHz DCO ayarı
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= PUL1_PIN | DIR1_PIN | PUL2_PIN | DIR2_PIN | BIT2 | BIT3;
    P1OUT &= ~(BIT2 | BIT3);    // Başlangıçta Z motoru durduruluyor
    
    /*
    sag(10);
    ileri(6);
    asagi();
    asagi();
    asagi();
    sol(1);
    geri(1);
    sag(1);
    ileri(1);
    yukari();
    yukari();
    yukari();
    sifiraDon();
*/

asagi();
asagi();


while(1){
    geri(5);
    sag(5);
    ileri(5);
    sol(5);
}


    
/*

    yukari();
    sag(6);
    ileri(3);
    asagi();
    asagi();
    asagi();
    asagi();
    sag(1);
    geri(1);
    sol(1);
    ileri(1);
    yukari();
    yukari();
    yukari();
    sifiraDon();  // Sol alt köşeye dön
    */
}