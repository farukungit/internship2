using System;
using System.Collections.Generic;

namespace ConsoleApp9
{
    class Kitap
    {
        public string ISBN { get; set; }
        public string Ad { get; set; }
        public string Yazar { get; set; }
        public bool OduncteMi { get; set; }

        public Kitap(string isbn, string ad, string yazar)
        {
            ISBN = isbn;
            Ad = ad;
            Yazar = yazar;
            OduncteMi = false; // İlk eklendiğinde kitap müsaittir
        }
    }

    class Uye
    {
        public int UyeNo { get; set; }
        public string AdSoyad { get; set; }
        public List<Kitap> AldigiKitaplar { get; set; }

        public Uye(int uyeNo, string adSoyad)
        {
            UyeNo = uyeNo;
            AdSoyad = adSoyad;
            AldigiKitaplar = new List<Kitap>();
        }
    }

    class Kutuphane
    {
        public List<Kitap> Kitaplar = new List<Kitap>();
        public List<Uye> Uyeler = new List<Uye>();

        public void KitapEkle()
        {
            Console.Write("ISBN: ");
            string isbn = Console.ReadLine();
            Console.Write("Kitap Adı: ");
            string ad = Console.ReadLine();
            Console.Write("Yazar: ");
            string yazar = Console.ReadLine();

            Kitap yeniKitap = new Kitap(isbn, ad, yazar);
            Kitaplar.Add(yeniKitap);
            Console.WriteLine("Kitap başarıyla eklendi.");
        }

        public void KitaplariListele()
        {
            if (Kitaplar.Count == 0)
            {
                Console.WriteLine("Kütüphanede kayıtlı kitap yok.");
                return;
            }

            foreach (var k in Kitaplar)
            {
                string durum = k.OduncteMi ? "Ödünçte" : "Müsait";
                Console.WriteLine($"ISBN: {k.ISBN} | Ad: {k.Ad} | Yazar: {k.Yazar} | Durum: {durum}");
            }
        }

        public void KitapAra()
        {
            Console.Write("Aranacak kitap adı: ");
            string aranan = Console.ReadLine().ToLower();
            bool bulundu = false;

            foreach (var k in Kitaplar)
            {
                if (k.Ad.ToLower().Contains(aranan))
                {
                    string durum = k.OduncteMi ? "Ödünçte" : "Müsait";
                    Console.WriteLine($"ISBN: {k.ISBN} | Ad: {k.Ad} | Yazar: {k.Yazar} | Durum: {durum}");
                    bulundu = true;
                }
            }

            if (!bulundu)
                Console.WriteLine("Aranan isimde kitap bulunamadı.");
        }

        public void KitapSil()
        {
            Console.Write("Silinecek kitabın ISBN numarası veya Adı: ");
            string bilgi = Console.ReadLine().ToLower();
            Kitap silinecek = null;

            foreach (var k in Kitaplar)
            {
                if (k.ISBN.ToLower() == bilgi || k.Ad.ToLower() == bilgi)
                {
                    silinecek = k;
                    break;
                }
            }

            if (silinecek != null)
            {
                Kitaplar.Remove(silinecek);
                Console.WriteLine("Kitap silindi.");
            }
            else
            {
                Console.WriteLine("Kitap bulunamadı.");
            }
        }

        public void UyeEkle()
        {
            Console.Write("Üye No: ");
            int no = int.Parse(Console.ReadLine());
            Console.Write("Ad Soyad: ");
            string adSoyad = Console.ReadLine();

            Uyeler.Add(new Uye(no, adSoyad));
            Console.WriteLine("Üye başarıyla eklendi.");
        }

        public void UyeleriListele()
        {
            if (Uyeler.Count == 0)
            {
                Console.WriteLine("Kayıtlı üye yok.");
                return;
            }

            foreach (var u in Uyeler)
            {
                Console.WriteLine($"Üye No: {u.UyeNo} | Ad Soyad: {u.AdSoyad} | Aldığı Kitap Sayısı: {u.AldigiKitaplar.Count}");
            }
        }

        public void KitapOduncVer()
        {
            Console.Write("Üye No: ");
            int uyeNo = int.Parse(Console.ReadLine());
            Uye secilenUye = null;

            foreach (var u in Uyeler)
            {
                if (u.UyeNo == uyeNo)
                {
                    secilenUye = u;
                    break;
                }
            }

            if (secilenUye == null)
            {
                Console.WriteLine("Üye bulunamadı.");
                return;
            }

            Console.Write("Ödünç verilecek kitabın ISBN numarası: ");
            string isbn = Console.ReadLine();
            Kitap secilenKitap = null;

            foreach (var k in Kitaplar)
            {
                if (k.ISBN == isbn)
                {
                    secilenKitap = k;
                    break;
                }
            }

            if (secilenKitap == null)
            {
                Console.WriteLine("Kitap bulunamadı.");
            }
            else if (secilenKitap.OduncteMi)
            {
                Console.WriteLine("Bu kitap şu an başka bir üyede (ödünçte).");
            }
            else
            {
                secilenKitap.OduncteMi = true;
                secilenUye.AldigiKitaplar.Add(secilenKitap);
                Console.WriteLine($"{secilenKitap.Ad} kitabı {secilenUye.AdSoyad} adlı üyeye ödünç verildi.");
            }
        }

        public void OduncAlinanlariListele()
        {
            bool varMi = false;
            foreach (var k in Kitaplar)
            {
                if (k.OduncteMi)
                {
                    Console.WriteLine($"ISBN: {k.ISBN} | Ad: {k.Ad} | Yazar: {k.Yazar}");
                    varMi = true;
                }
            }

            if (!varMi)
                Console.WriteLine("Şu an ödünçte olan kitap yok.");
        }
    }

    internal class Program
    {
        static void Main(string[] args)
        {
            Kutuphane kutuphane = new Kutuphane();
            bool devam = true;

            while (devam)
            {
                Console.WriteLine("\n===== KÜTÜPHANE SİSTEMİ =====");
                Console.WriteLine("1. Yeni Kitap Ekle");
                Console.WriteLine("2. Tüm Kitapları Listele");
                Console.WriteLine("3. Kitap Ara (Ada göre)");
                Console.WriteLine("4. Kitap Sil");
                Console.WriteLine("5. Üye Ekle");
                Console.WriteLine("6. Üyeleri Listele");
                Console.WriteLine("7. Kitap Ödünç Ver");
                Console.WriteLine("8. Ödünç Alınan Kitapları Listele");
                Console.WriteLine("0. Çıkış");
                Console.WriteLine("=============================");
                Console.Write("Seçiminiz: ");

                try
                {
                    int secim = int.Parse(Console.ReadLine());
                    Console.WriteLine();

                    if (secim == 1) kutuphane.KitapEkle();
                    else if (secim == 2) kutuphane.KitaplariListele();
                    else if (secim == 3) kutuphane.KitapAra();
                    else if (secim == 4) kutuphane.KitapSil();
                    else if (secim == 5) kutuphane.UyeEkle();
                    else if (secim == 6) kutuphane.UyeleriListele();
                    else if (secim == 7) kutuphane.KitapOduncVer();
                    else if (secim == 8) kutuphane.OduncAlinanlariListele();
                    else if (secim == 0)
                    {
                        devam = false;
                        Console.WriteLine("Çıkış yapılıyor...");
                    }
                    else
                    {
                        Console.WriteLine("Geçersiz seçim! 0-8 arası bir sayı girin.");
                    }
                }
                catch
                {
                    Console.WriteLine("Hatalı giriş! Lütfen sayısal bir değer girin.");
                }
            }
        }
    }
}
