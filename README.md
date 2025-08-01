# internship2

## Giriş

Bu depo, MSP430 mikrodenetleyicisi üzerine odaklanmış bir gömülü sistem stajının çıktılarından oluşmaktadır. Staj boyunca temel donanım-yazılım etkileşimlerinden başlayarak ileri düzey motor kontrolü, sensör entegrasyonu ve bellek optimizasyonu gibi birçok konu pratik olarak ele alınmıştır. Amaç, gerçek zamanlı gömülü sistemler tasarımı konusunda sağlam bir temel oluşturmaktır.

---

## 1. Hafta: MSP430 Temelleri ve Temel Uygulamalar

- **Mikrodenetleyici Tanıtımı:**
  - MSP430G2231: 16-bit CPU, 2KB Flash, 128 byte RAM
- **LED Uygulamaları:**
  - GPIO ile LED yakma/söndürme
  - Karaşimşek efekti, Mors kodu ile LED animasyonları
- **Buton Girişi:**
  - P1.3 pininden okuma, LED tetikleme
- **PWM:**
  - Timer A kullanılarak LED parlaklık kontrolü
- **Step Motor Kontrolü:**
  - 28BYJ-48 motor + ULN2003A sürücü
  - Tam ve yarım adım kontrol
- **Ultrasonik Sensör:**
  - HC-SR04 ile mesafe ölçümü
  - Motor hızının mesafeye göre ayarlanması
- **Versiyon Kontrolü:**
  - Git/GitHub kullanımı (clone, commit, push vs.)

---

## 2. Hafta: Gelişmiş Step Motor Kontrolü

- **İki Eksenli Step Motor Sistemi:**
  - CWD556 sürücüler + 2 step motor (X/Y)
  - Motor1: P1.0 → PUL+, P1.1 → DIR+
  - Motor2: P1.6 → PUL+, P1.7 → DIR+
- **Lojik Seviye Uyumluluğu:**
  - 3.3V (MSP430) → 5V (CWD556) için NPN C547B transistörlü yükseltici
  - ENA pininin aktif hale getirilmesi
- **Kalibrasyon:**
  - 4096 adım = 1 tam dönüş
  - _delay_us() ile hız kontrolü
- **Fonksiyonlar:**
  - `pulse_motor1()`, `pulse_motor2()`, `dual_pulse()` gibi modüler yapı

---

## 3. Hafta: Bellek Optimizasyonu, Çizim Algoritmaları ve G-Code

- **Sorun Giderme:**
  - Donanımsal titreme/sarsıntı sorunları testlerle çözüldü
- **Koordinat Tabanlı Hareket:**
  - X/Y ekseninde cm cinsinden pozisyonlama
  - 1 adım ≈ 0.5 cm dönüşüm
- **Çizim Algoritmaları:**
  - DDA ve Bresenham algoritmaları (çizgi çizimi)
  - Bellek sınırı (2KB) nedeniyle optimizasyon ihtiyacı
- **Optimizasyon Teknikleri:**
  - Sabit nokta matematiği
  - `uint16_t` gibi bellek dostu veri tipleri
  - Kod boyutu 4.3 KB → 3.6 KB (yine de 2KB’a sığmıyor)
- **DC Motor Kontrolü:**
  - MX1508 sürücü + IR sensör + LED
  - G-code komutları için altyapı araştırması

---

## Donanım Listesi

- MSP430G2231 Mikrodenetleyici
- 2x CWD556 Dijital Sürücü
- Step Motor: 57BHH82G00E21E
- ULN2003A, MX1508, HC-SR04, IR sensör, LED
- NPN C547B Transistör
- 20V ve 25V Güç Kaynakları

### Bağlantılar

- **Motor X:**
  - PUL+ → P1.0, DIR+ → P1.1, GND → DIR- (→ PUL-)
- **Motor Y:**
  - PUL+ → P1.6, DIR+ → P1.7, GND ortak
- **Güç:**
  - Motor X: 20V (şarj adaptörü)
  - Motor Y: 25V (güç kaynağı)

---

## Katkıda Bulunanlar

- Faruk Musa ([@farukungit](https://github.com/farukungit))

---

## Lisans

Bu proje MIT lisansı ile lisanslanmıştır. Detaylar için `LICENSE` dosyasına bakabilirsiniz.
