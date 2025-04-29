# SOSYAL AG ANALIZ SISTEMI
 Bir sosyal ağ grafiğini ağaç tabanlı veri yapıları kullanarak modellenmesi. Veri yapıları ve algoritmaları ödevi

Bu proje, C dilinde yazılmış bir sosyal ağ simülasyonudur. Kullanıcılar Red-Black Tree veri yapısında saklanır ve aralarındaki arkadaşlık ilişkileri bir grafik yapısı (adjacency list) ile temsil edilir. Sistem, kullanıcı ekleme, arkadaşlık oluşturma, kullanıcı silme, kullanıcıları listeleme, kullanıcı arama ve etki alanı hesaplama gibi işlemleri destekler.

# Özellikler
*Yeni kullanıcı ekleme
*İki kullanıcı arasında arkadaşlık kurma
*Belirli derinlikteki arkadaşları bulma
*Ortak arkadaşları bulma
*Toplulukları (bağlantılı bileşenleri) tespit etme
*Bir kullanıcının etki alanını hesaplama 
*Veri setinden yükleme ve dosyaya kaydetme

# Dosya İçeriği
* main.c: Programın ana dosyası, tüm işlemleri içerir.
* kirmizi_siyah_agac.h/.c: Red-Black Tree implementasyonu.
* graf.h/.c: Arkadaşlık ilişkilerini yöneten grafik yapısı.
* kullanici.h/.c: Kullanıcı bilgilerini tanımlayan yapılar ve işlemler.
