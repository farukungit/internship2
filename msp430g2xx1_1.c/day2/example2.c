//kum saati 8 ledli


#include <msp430.h>

// Yaklaşık 1 saniye gecikme sağlayan fonksiyon
// Mikrodenetleyicinin saat hızına göre sürenin doğruluğu değişebilir.
void delay1s()
{
    __delay_cycles(1000000); // 1 milyon döngü bekle
}

int example2(void)
{
    // Watchdog Timer'ı kapat. Programın istenmeyen yeniden başlatılmasını önler.
    WDTCTL = WDTPW | WDTHOLD;

    // P1 portundaki tüm pinleri (P1.0 – P1.7) çıkış olarak ayarla.
    P1DIR |= 0xFF;

    // Sonsuz döngü: Program yüklendikten sonra sürekli çalışır.
    while (1)
    {
        // 1. Başlangıç Durumu: İlk 4 LED'i (P1.0-P1.3) yak.
        P1OUT = 0x0F; // Binary: 00001111 (P1.0, P1.1, P1.2, P1.3 yanar)
        delay1s();    // Bir süre bu durumda kal
        int i;
        // 2. Sağa Doğru Kayma (Kum Saati Akışı - P1.0-P1.3'ten P1.4-P1.7'ye)
        for (i = 0; i < 4; i++)
        {
            // Soldaki LED'i söndür (P1.0 -> P1.1 -> P1.2 -> P1.3)
            P1OUT &= ~(1 << i);
            // Sağdaki LED'i yak (P1.4 -> P1.5 -> P1.6 -> P1.7)
            P1OUT |= (1 << (i + 4));
            delay1s(); // Her adımda 1 saniye bekle
        }

        delay1s(); // Sağdaki 4 LED yandıktan sonra bir süre bekle

        // 3. Sola Doğru Geri Kayma (Kum Saati Geri Akışı - P1.7-P1.4'ten P1.3-P1.0'a)
       
        for (i = 0; i < 4; i++)
        {
            // Sağdaki LED'i söndür (P1.4 -> P1.5 -> P1.6 -> P1.7)
            P1OUT &= ~(1 << (i + 4));
            // Soldaki LED'i yak (P1.0 -> P1.1 -> P1.2 -> P1.3)
            P1OUT |= (1 << i);
            delay1s(); // Her adımda 1 saniye bekle
        }

        delay1s(); // Başlangıç pozisyonuna geri geldikten sonra bir süre bekle

        // 4. Tüm LED'leri Söndür (Döngüye başlamadan önce)
        P1OUT = 0x00;
        delay1s(); // Tüm LED'ler söndükten sonra bir süre bekle
    }
}