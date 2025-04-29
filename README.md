# SOSYAL AG ANALIZ SISTEMI
 Bir sosyal ağ grafiğini ağaç tabanlı veri yapıları kullanarak modellenmesi. Veri yapıları ve algoritmaları ödevi

Bu proje, C dilinde yazılmış bir sosyal ağ simülasyonudur. Kullanıcılar Red-Black Tree veri yapısında saklanır ve aralarındaki arkadaşlık ilişkileri bir grafik yapısı (adjacency list) ile temsil edilir. Sistem, kullanıcı ekleme, arkadaşlık oluşturma, kullanıcı silme, kullanıcıları listeleme, kullanıcı arama ve etki alanı hesaplama gibi işlemleri destekler.

# Özellikler
* Yeni kullanıcı ekleme
* İki kullanıcı arasında arkadaşlık kurma
* Belirli derinlikteki arkadaşları bulma
* Ortak arkadaşları bulma
* Toplulukları (bağlantılı bileşenleri) tespit etme
* Bir kullanıcının etki alanını hesaplama 
* Veri setinden yükleme ve dosyaya kaydetme

# Dosya İçeriği
* main.c: Programın ana dosyası, tüm işlemleri içerir.
* kirmizi_siyah_agac.h/.c: Red-Black Tree implementasyonu.
* graf.h/.c: Arkadaşlık ilişkilerini yöneten grafik yapısı.
* kullanici.h/.c: Kullanıcı bilgilerini tanımlayan yapılar ve işlemler.

# FONKSİYONLAR
# Kullanıcı Yönetimi:
* Yeni kullanıcılar ekleyin.
* Mevcut kullanıcıları arayın.
* Arkadaşlık Yönetimi:
* Kullanıcılar arasında arkadaşlıklar ekleyin.
* İki kullanıcı arasındaki ortak arkadaşları listeleyin.

# Topluluk Tespiti:
* Birbirine bağlı kullanıcı gruplarını (toplulukları) keşfedin.
* Sosyal Ağ Metriği:
* Bir kullanıcının arkadaş sayısına ve ağdaki etkisine göre bir etki puanı hesaplayın.

# Graf Görselleştirme (BFS/DFS ile):
* Bir kullanıcının belirli bir derinlikteki veya mesafedeki arkadaşlarını gösterin.

# Veri Sürekliliği:
* Kullanıcı verilerini ve arkadaşlıkları dosyaya kaydedin.
* Dosyadan veri yükleyerek grafiği yeniden oluşturun.


 # Kullanılan Veri Yapıları
# 1-Red-Black Ağaç:
* Kullanıcı verilerinin yönetilmesi için Red-Black Ağaç yapısı kullanılır. Bu yapı, hızlı arama, ekleme ve silme işlemleri sağlar.

# 2-Graf Yapısı:
* Kullanıcılar ve arkadaşlıkları temsil etmek için grafik veri yapısı kullanılır. Bu grafik, yönsüz bir grafik olup, her kullanıcı bir düğüm (node) olarak temsil edilir ve arkadaşlıklar kenarlar (edge) olarak eklenir.  
