#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAKS_KULLANICILAR 1000
#define KIRMIZI 0
#define SIYAH 1

typedef struct Kullanici {
    int id;
    int arkadasSayisi;
    struct Kullanici** arkadaslar;
} Kullanici;

typedef struct KSDugum {
    int id;
    Kullanici* kullanici;
    int renk;
    struct KSDugum* ebeveyn;
    struct KSDugum* sol;
    struct KSDugum* sag;
} KSDugum;

typedef struct Graf {
    Kullanici** kullanicilar;
    int kullaniciSayisi;
    KSDugum* ksKok;
} Graf;

KSDugum* BOS;
const char* DOSYA_ADI = "veriseti.txt";

// Fonksiyon prototipleri
Kullanici* kullaniciOlustur(int id);
KSDugum* ksDugumOlustur(int id, Kullanici* kullanici);
Graf* grafOlustur();
void solRotasyon(Graf* graf, KSDugum* x);
void sagRotasyon(Graf* graf, KSDugum* y);
void eklemeyiDuzelt(Graf* graf, KSDugum* k);
void ksDugumEkle(Graf* graf, int id, Kullanici* kullanici);
Kullanici* kullaniciAra(Graf* graf, int id);
void kullaniciEkle(Graf* graf, int id);
void arkadaslikEkle(Graf* graf, int id1, int id2);
void belirliMesafedeArkadaslariBul(Graf* graf, int baslangicId, int maksDerinlik);
void ortakArkadaslariBul(Graf* graf, int id1, int id2);
void topluluklariTespit(Graf* graf);
void etkiAlaniHesapla(Graf* graf, int kullaniciId);
void grafTemizle(Graf* graf);
void dosyayaKaydet(Graf* graf, const char* dosyaAdi);
int dosyadanOku(Graf* graf, const char* dosyaAdi);

// Ana fonksiyonlar
Kullanici* kullaniciOlustur(int id) {
    Kullanici* yeniKullanici = (Kullanici*)malloc(sizeof(Kullanici));
    if (yeniKullanici) {
        yeniKullanici->id = id;
        yeniKullanici->arkadasSayisi = 0;
        yeniKullanici->arkadaslar = NULL;
    }
    return yeniKullanici;
}

KSDugum* ksDugumOlustur(int id, Kullanici* kullanici) {
    KSDugum* yeniDugum = (KSDugum*)malloc(sizeof(KSDugum));
    if (yeniDugum) {
        yeniDugum->id = id;
        yeniDugum->kullanici = kullanici;
        yeniDugum->renk = KIRMIZI;
        yeniDugum->ebeveyn = BOS;
        yeniDugum->sol = BOS;
        yeniDugum->sag = BOS;
    }
    return yeniDugum;
}

Graf* grafOlustur() {
    Graf* graf = (Graf*)malloc(sizeof(Graf));
    if (graf) {
        graf->kullanicilar = (Kullanici**)calloc(MAKS_KULLANICILAR, sizeof(Kullanici*));
        graf->kullaniciSayisi = 0;
        graf->ksKok = BOS;
    }
    return graf;
}

void solRotasyon(Graf* graf, KSDugum* x) {
    KSDugum* y = x->sag;
    x->sag = y->sol;
    if (y->sol != BOS) y->sol->ebeveyn = x;
    y->ebeveyn = x->ebeveyn;
    
    if (x->ebeveyn == BOS) graf->ksKok = y;
    else if (x == x->ebeveyn->sol) x->ebeveyn->sol = y;
    else x->ebeveyn->sag = y;
    
    y->sol = x;
    x->ebeveyn = y;
}

void sagRotasyon(Graf* graf, KSDugum* y) {
    KSDugum* x = y->sol;
    y->sol = x->sag;
    if (x->sag != BOS) x->sag->ebeveyn = y;
    x->ebeveyn = y->ebeveyn;
    
    if (y->ebeveyn == BOS) graf->ksKok = x;
    else if (y == y->ebeveyn->sol) y->ebeveyn->sol = x;
    else y->ebeveyn->sag = x;
    
    x->sag = y;
    y->ebeveyn = x;
}

void eklemeyiDuzelt(Graf* graf, KSDugum* k) {
    KSDugum* u;
    while (k->ebeveyn->renk == KIRMIZI) {
        if (k->ebeveyn == k->ebeveyn->ebeveyn->sag) {
            u = k->ebeveyn->ebeveyn->sol;
            if (u->renk == KIRMIZI) {
                u->renk = SIYAH;
                k->ebeveyn->renk = SIYAH;
                k->ebeveyn->ebeveyn->renk = KIRMIZI;
                k = k->ebeveyn->ebeveyn;
            } else {
                if (k == k->ebeveyn->sol) {
                    k = k->ebeveyn;
                    sagRotasyon(graf, k);
                }
                k->ebeveyn->renk = SIYAH;
                k->ebeveyn->ebeveyn->renk = KIRMIZI;
                solRotasyon(graf, k->ebeveyn->ebeveyn);
            }
        } else {
            u = k->ebeveyn->ebeveyn->sag;
            if (u->renk == KIRMIZI) {
                u->renk = SIYAH;
                k->ebeveyn->renk = SIYAH;
                k->ebeveyn->ebeveyn->renk = KIRMIZI;
                k = k->ebeveyn->ebeveyn;
            } else {
                if (k == k->ebeveyn->sag) {
                    k = k->ebeveyn;
                    solRotasyon(graf, k);
                }
                k->ebeveyn->renk = SIYAH;
                k->ebeveyn->ebeveyn->renk = KIRMIZI;
                sagRotasyon(graf, k->ebeveyn->ebeveyn);
            }
        }
        if (k == graf->ksKok) break;
    }
    graf->ksKok->renk = SIYAH;
}

void ksDugumEkle(Graf* graf, int id, Kullanici* kullanici) {
    KSDugum* yeniDugum = ksDugumOlustur(id, kullanici);
    KSDugum* y = BOS;
    KSDugum* x = graf->ksKok;

    while (x != BOS) {
        y = x;
        x = (yeniDugum->id < x->id) ? x->sol : x->sag;
    }

    yeniDugum->ebeveyn = y;

    if (y == BOS) graf->ksKok = yeniDugum;
    else if (yeniDugum->id < y->id) y->sol = yeniDugum;
    else y->sag = yeniDugum;

    if (yeniDugum->ebeveyn == BOS) {
        yeniDugum->renk = SIYAH;
        return;
    }

    if (yeniDugum->ebeveyn->ebeveyn == BOS) return;

    eklemeyiDuzelt(graf, yeniDugum);
}

Kullanici* kullaniciAraKS(KSDugum* kok, int id) {
    if (kok == BOS) return NULL;
    if (kok->id == id) return kok->kullanici;
    return (id < kok->id) ? kullaniciAraKS(kok->sol, id) : kullaniciAraKS(kok->sag, id);
}

Kullanici* kullaniciAra(Graf* graf, int id) {
    return kullaniciAraKS(graf->ksKok, id);
}

void kullaniciEkle(Graf* graf, int id) {
    if (kullaniciAra(graf, id) != NULL) {
        printf("Kullanici %d zaten var\n", id);
        return;
    }

    Kullanici* yeniKullanici = kullaniciOlustur(id);
    if (yeniKullanici) {
        graf->kullanicilar[graf->kullaniciSayisi++] = yeniKullanici;
        ksDugumEkle(graf, id, yeniKullanici);
        printf("Kullanici %d eklendi\n", id);
    }
}

void arkadaslikEkle(Graf* graf, int id1, int id2) {
    Kullanici* kullanici1 = kullaniciAra(graf, id1);
    Kullanici* kullanici2 = kullaniciAra(graf, id2);

    if (!kullanici1 || !kullanici2) {
        printf("Bir veya her iki kullanici bulunamadi\n");
        return;
    }

    for (int i = 0; i < kullanici1->arkadasSayisi; i++) {
        if (kullanici1->arkadaslar[i]->id == id2) {
            printf("%d ve %d zaten arkadas\n", id1, id2);
            return;
        }
    }

    kullanici1->arkadaslar = (Kullanici**)realloc(kullanici1->arkadaslar, (kullanici1->arkadasSayisi + 1) * sizeof(Kullanici*));
    kullanici1->arkadaslar[kullanici1->arkadasSayisi++] = kullanici2;

    kullanici2->arkadaslar = (Kullanici**)realloc(kullanici2->arkadaslar, (kullanici2->arkadasSayisi + 1) * sizeof(Kullanici*));
    kullanici2->arkadaslar[kullanici2->arkadasSayisi++] = kullanici1;

    printf("%d ve %d arkadas oldular\n", id1, id2);
}

void dfsAra(Kullanici* kullanici, int derinlik, int maksDerinlik, int* ziyaretEdildi) {
    if (derinlik > maksDerinlik) return;
    
    if (!ziyaretEdildi[kullanici->id]) {
        ziyaretEdildi[kullanici->id] = 1;
        if (derinlik > 0 && derinlik <= maksDerinlik) {
            printf("Derinlik %d: %d\n", derinlik, kullanici->id);
        }
        
        if (derinlik < maksDerinlik) {
            for (int i = 0; i < kullanici->arkadasSayisi; i++) {
                if (!ziyaretEdildi[kullanici->arkadaslar[i]->id]) {
                    dfsAra(kullanici->arkadaslar[i], derinlik + 1, maksDerinlik, ziyaretEdildi);
                }
            }
        }
    }
}

void belirliMesafedeArkadaslariBul(Graf* graf, int baslangicId, int maksDerinlik) {
    Kullanici* baslangicKullanici = kullaniciAra(graf, baslangicId);
    if (!baslangicKullanici) {
        printf("Baslangic kullanicisi bulunamadi\n");
        return;
    }

    int* ziyaretEdildi = (int*)calloc(MAKS_KULLANICILAR, sizeof(int));
    printf("%d icin %d derinligine kadar olan arkadaslar:\n", baslangicId, maksDerinlik);
    dfsAra(baslangicKullanici, 0, maksDerinlik, ziyaretEdildi);
    
    int sonucVar = 0;
    for (int i = 0; i < MAKS_KULLANICILAR; i++) {
        if (ziyaretEdildi[i] && i != baslangicId) {
            sonucVar = 1;
            break;
        }
    }
    
    if (!sonucVar) {
        printf("Bu derinlige kadar arkadas bulunamadi\n");
    }
    
    free(ziyaretEdildi);
}

void ortakArkadaslariBul(Graf* graf, int id1, int id2) {
    Kullanici* kullanici1 = kullaniciAra(graf, id1);
    Kullanici* kullanici2 = kullaniciAra(graf, id2);

    if (!kullanici1 || !kullanici2) {
        printf("Bir veya her iki kullanici bulunamadi\n");
        return;
    }
    
    printf("%d ve %d icin ortak arkadaslar:\n", id1, id2);
    int ortakSayi = 0;
    
    for (int i = 0; i < kullanici1->arkadasSayisi; i++) {
        int arkadas1Id = kullanici1->arkadaslar[i]->id;
        
        for (int j = 0; j < kullanici2->arkadasSayisi; j++) {
            if (arkadas1Id == kullanici2->arkadaslar[j]->id) {
                printf("- %d\n", arkadas1Id);
                ortakSayi++;
                break;
            }
        }
    }

    if (ortakSayi == 0) {
        printf("Ortak arkadas bulunamadi\n");
    } else {
        printf("Toplam %d ortak arkadas bulundu\n", ortakSayi);
    }
}

void bfsTopluluklari(Kullanici* baslangicKullanici, int toplulukId, int* ziyaretEdildi) {
    Kullanici** kuyruk = (Kullanici**)malloc(MAKS_KULLANICILAR * sizeof(Kullanici*));
    int on = 0, arka = 0;

    kuyruk[arka++] = baslangicKullanici;
    ziyaretEdildi[baslangicKullanici->id] = toplulukId;
    printf("Topluluk %d: %d", toplulukId, baslangicKullanici->id);

    while (on < arka) {
        Kullanici* kullanici = kuyruk[on++];
        
        for (int i = 0; i < kullanici->arkadasSayisi; i++) {
            Kullanici* arkadas = kullanici->arkadaslar[i];
            if (!ziyaretEdildi[arkadas->id]) {
                ziyaretEdildi[arkadas->id] = toplulukId;
                printf(", %d", arkadas->id);
                kuyruk[arka++] = arkadas;
            }
        }
    }

    printf("\n");
    free(kuyruk);
}

void topluluklariTespit(Graf* graf) {
    int* ziyaretEdildi = (int*)calloc(MAKS_KULLANICILAR, sizeof(int));
    int toplulukSayisi = 0;

    printf("Topluluk tespiti basliyor...\n");
    
    for (int i = 0; i < graf->kullaniciSayisi; i++) {
        Kullanici* kullanici = graf->kullanicilar[i];
        if (kullanici && !ziyaretEdildi[kullanici->id]) {
            toplulukSayisi++;
            bfsTopluluklari(kullanici, toplulukSayisi, ziyaretEdildi);
        }
    }

    printf("Toplam %d topluluk tespit edildi\n", toplulukSayisi);
    free(ziyaretEdildi);
}

void bfsEtkiAlani(Kullanici* baslangicKullanici, int* ziyaretEdildi, int* uzaklik) {
    Kullanici** kuyruk = (Kullanici**)malloc(MAKS_KULLANICILAR * sizeof(Kullanici*));
    int on = 0, arka = 0;

    kuyruk[arka++] = baslangicKullanici;
    ziyaretEdildi[baslangicKullanici->id] = 1;
    uzaklik[baslangicKullanici->id] = 0;

    while (on < arka) {
        Kullanici* kullanici = kuyruk[on++];
        
        for (int i = 0; i < kullanici->arkadasSayisi; i++) {
            Kullanici* arkadas = kullanici->arkadaslar[i];
            if (!ziyaretEdildi[arkadas->id]) {
                ziyaretEdildi[arkadas->id] = 1;
                uzaklik[arkadas->id] = uzaklik[kullanici->id] + 1;
                kuyruk[arka++] = arkadas;
            }
        }
    }

    free(kuyruk);
}

void etkiAlaniHesapla(Graf* graf, int kullaniciId) {
    Kullanici* kullanici = kullaniciAra(graf, kullaniciId);
    if (!kullanici) {
        printf("Kullanici bulunamadi\n");
        return;
    }

    int* ziyaretEdildi = (int*)calloc(MAKS_KULLANICILAR, sizeof(int));
    int* uzaklik = (int*)malloc(MAKS_KULLANICILAR * sizeof(int));
    
    for (int i = 0; i < MAKS_KULLANICILAR; i++) {
        uzaklik[i] = -1;
    }

    bfsEtkiAlani(kullanici, ziyaretEdildi, uzaklik);

    int maksUzaklik = 0;
    int ulasabilenKullanicilar = 0;
    int toplamUzaklik = 0;

    for (int i = 0; i < graf->kullaniciSayisi; i++) {
        Kullanici* digerKullanici = graf->kullanicilar[i];
        if (digerKullanici && digerKullanici->id != kullaniciId && uzaklik[digerKullanici->id] != -1) {
            ulasabilenKullanicilar++;
            toplamUzaklik += uzaklik[digerKullanici->id];
            if (uzaklik[digerKullanici->id] > maksUzaklik) {
                maksUzaklik = uzaklik[digerKullanici->id];
            }
        }
    }

    float ortUzaklik = ulasabilenKullanicilar > 0 ? (float)toplamUzaklik / ulasabilenKullanicilar : 0;

    printf("\n%d icin etki alani analizi:\n", kullaniciId);
    printf("- Direkt arkadas sayisi: %d\n", kullanici->arkadasSayisi);
    printf("- Ulasabilenler: %d\n", ulasabilenKullanicilar);
    printf("- Maksimum mesafe: %d\n", maksUzaklik);
    printf("- Ortalama mesafe: %.2f\n", ortUzaklik);

    float etki = kullanici->arkadasSayisi * (ulasabilenKullanicilar > 0 ? (float)ulasabilenKullanicilar / maksUzaklik : 0);
    printf("- Etki puani: %.2f\n", etki);

    free(ziyaretEdildi);
    free(uzaklik);
}

void dosyayaKaydet(Graf* graf, const char* dosyaAdi) {
    FILE* dosya = fopen(dosyaAdi, "w");
    if (!dosya) {
        printf("Dosya acilamadi\n");
        return;
    }

    fprintf(dosya, "# Kullanicilar\n");
    for (int i = 0; i < graf->kullaniciSayisi; i++) {
        fprintf(dosya, "USER %d\n", graf->kullanicilar[i]->id);
    }

    fprintf(dosya, "\n# Arkadaslik iliskileri\n");
    for (int i = 0; i < graf->kullaniciSayisi; i++) {
        Kullanici* kullanici = graf->kullanicilar[i];
        for (int j = 0; j < kullanici->arkadasSayisi; j++) {
            if (kullanici->id < kullanici->arkadaslar[j]->id) {
                fprintf(dosya, "FRIEND %d %d\n", kullanici->id, kullanici->arkadaslar[j]->id);
            }
        }
    }

    fclose(dosya);
    printf("Veriler '%s' dosyasina kaydedildi\n", dosyaAdi);
}

int dosyadanOku(Graf* graf, const char* dosyaAdi) {
    FILE* dosya = fopen(dosyaAdi, "r");
    if (!dosya) {
        return 0;
    }

    char satir[256];
    while (fgets(satir, sizeof(satir), dosya)) {
        if (satir[0] == '#' || satir[0] == '\n') continue;

        char komut[10];
        int id1, id2;

        if (sscanf(satir, "%9s %d %d", komut, &id1, &id2) >= 2) {
            if (strcmp(komut, "USER") == 0) {
                kullaniciEkle(graf, id1);
            } else if (strcmp(komut, "FRIEND") == 0 && sscanf(satir, "%9s %d %d", komut, &id1, &id2) == 3) {
                arkadaslikEkle(graf, id1, id2);
            }
        }
    }

    fclose(dosya);
    return 1;
}

void grafTemizle(Graf* graf) {
    if (graf) {
        for (int i = 0; i < graf->kullaniciSayisi; i++) {
            if (graf->kullanicilar[i]) {
                free(graf->kullanicilar[i]->arkadaslar);
                free(graf->kullanicilar[i]);
            }
        }
        free(graf->kullanicilar);
        free(graf);
    }
}

int main() {
    BOS = (KSDugum*)malloc(sizeof(KSDugum));
    BOS->renk = SIYAH;
    BOS->sol = NULL;
    BOS->sag = NULL;
    BOS->ebeveyn = NULL;

    Graf* graf = grafOlustur();
    char girdi[100];
    int secim, kullaniciId, arkadasId, maksDerinlik;

    // Dosyadan veri oku
    dosyadanOku(graf, DOSYA_ADI);

    // Kullanýcý ekleme
    while (1) {
        printf("Eklemek istediginiz kullanici ID (cikmak icin 'cikis'): ");
        if (fgets(girdi, sizeof(girdi), stdin) == NULL) break;
        if (strcmp(girdi, "cikis\n") == 0) break;
        
        int id;
        if (sscanf(girdi, "%d", &id) != 1) {
            printf("Gecersiz ID. Sayi girin\n");
            continue;
        }
        kullaniciEkle(graf, id);
    }

    // Arkadaþlýk iliþkisi ekleme
    printf("\nSimdi arkadas baglarini ekleyin:\n");
    while (1) {
        printf("Arkadas bagi (ID1 ID2), cikmak icin 'cikis': ");
        if (fgets(girdi, sizeof(girdi), stdin) == NULL) break;
        if (girdi[0] == '\n') continue;
        if (strcmp(girdi, "cikis\n") == 0) break;
        
        int id1, id2;
        if (sscanf(girdi, "%d %d", &id1, &id2) != 2) {
            printf("Gecersiz format. 'ID1 ID2' formatinda girin\n");
            continue;
        }
        
        Kullanici* kullanici1 = kullaniciAra(graf, id1);
        Kullanici* kullanici2 = kullaniciAra(graf, id2);

        if (!kullanici1) {
            printf("HATA: %d bulunamadi\n", id1);
            continue;
        }
        if (!kullanici2) {
            printf("HATA: %d bulunamadi\n", id2);
            continue;
        }

        arkadaslikEkle(graf, id1, id2);
    }
    
    // Verileri dosyaya kaydet
    dosyayaKaydet(graf, DOSYA_ADI);
    
    // Menü sistemi
    do {
        printf("\n===== MENU =====\n");
        printf("1. Belirli mesafedeki arkadaslari bul (DFS)\n");
        printf("2. Ortak arkadas analizi\n");
        printf("3. Topluluk tespiti\n");
        printf("4. Etki alani hesapla\n");
        printf("5. Cikis\n");
        printf("Seciminiz: ");
        
        if (scanf("%d", &secim) != 1) {
            while (getchar() != '\n');
            printf("Gecersiz secim\n");
            continue;
        }
        
        switch (secim) {
            case 1:
                printf("Baslangic kullanici ID: ");
                if (scanf("%d", &kullaniciId) != 1) {
                    printf("Gecersiz ID\n");
                    while (getchar() != '\n');
                    break;
                }
                printf("Istenen derinlik: ");
                if (scanf("%d", &maksDerinlik) != 1) {
                    printf("Gecersiz derinlik\n");
                    while (getchar() != '\n');
                    break;
                }
                belirliMesafedeArkadaslariBul(graf, kullaniciId, maksDerinlik);
                break;
                
            case 2:
                printf("Birinci kullanici ID: ");
                if (scanf("%d", &kullaniciId) != 1) {
                    printf("Gecersiz ID\n");
                    while (getchar() != '\n');
                    break;
                }
                printf("Ikinci kullanici ID: ");
                if (scanf("%d", &arkadasId) != 1) {
                    printf("Gecersiz ID\n");
                    while (getchar() != '\n');
                    break;
                }
                ortakArkadaslariBul(graf, kullaniciId, arkadasId);
                break;
                
            case 3:
                topluluklariTespit(graf);
                break;
                
            case 4:
                printf("Etki alani hesaplanacak kullanici ID: ");
                if (scanf("%d", &kullaniciId) != 1) {
                    printf("Gecersiz ID\n");
                    while (getchar() != '\n');
                    break;
                }
                etkiAlaniHesapla(graf, kullaniciId);
                break;
                
            case 5:
                printf("Programdan cikiliyor...\n");
                break;
                
            default:
                printf("Gecersiz secim\n");
                break;
        }
        
        while (getchar() != '\n'); // Stdin buffer temizleme
        
    } while (secim != 5);
    
    // Bellek temizleme
    grafTemizle(graf);
    free(BOS);
    
    return 0;
}

