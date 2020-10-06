#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

#define TAKIMADSINIR 25

struct ayarlar
{
	int takim_sayisi;
	int galibiyet;
	int beraberlik;
	int maglubiyet;
	int mac_sayisi;
}ayar;

/* Takımlar struct yapısı ile tanımlandı. */
struct takimlar
{
	char takim_ad[TAKIMADSINIR];
	char takma_kisaAd;
	int puan;
	int oynadigimacsayisi;
	int galibiyetsayisi;
	int beraberliksayisi;
	int maglubiyetsayisi;
	int attigigolsayisi;
	int yedigigolsayisi;
	int averaj;

}tkm[10]={
	{"",'A',0,0,0,0,0,0,0,0},
	{"",'B',0,0,0,0,0,0,0,0},
	{"",'C',0,0,0,0,0,0,0,0},
	{"",'D',0,0,0,0,0,0,0,0},
	{"",'E',0,0,0,0,0,0,0,0},
	{"",'F',0,0,0,0,0,0,0,0},
	{"",'G',0,0,0,0,0,0,0,0},
	{"",'H',0,0,0,0,0,0,0,0},
	{"",'I',0,0,0,0,0,0,0,0},
	{"",'J',0,0,0,0,0,0,0,0},
},temp;  /* temp değişkeni takım struct içerisine kaydedildi. */


/* Maclar struct yapısı ile tanımlandı. */
struct mac
{
	char evsahibi;
	char misafir;
	int evSkor;
	int misSkor;
}maclar[100];


/* Takımların uzun ismi dosyadan okundu */
void TakimGirisi()
{
	FILE *file;

	if((file=fopen("takimlar.txt", "r")) == NULL){
		printf("Dosya açılamadı!\n");
      	exit(1);
	}

	int i = 0;
	while(!feof(file))
	{
		fscanf(file, "%s", &tkm[i].takim_ad);
		//printf("Takım %d: %s\n",i+1, tkm[i].takim_ad);
		i++;
	}

	fclose(file);
}


/* Takım ,Galibiyet ,Beraberlik ,Mağlubiyet değerleri ayarlar.txt dosyasından okundu */
void AyarGirisi(){

	FILE *file;

	if((file=fopen("ayarlar.txt", "r")) == NULL){
		printf("Dosya açılamadı!\n");
      	exit(1);
	}

	fscanf(file, "%d %d %d %d", &ayar.takim_sayisi, &ayar.galibiyet, &ayar.beraberlik, &ayar.maglubiyet);

	fclose(file);
}


/* Struct yapısında Takımların kısa adlarının sayısal değerini bulmak için for döngüsü ile içerisinde gezindik ve her takımın numarasını eşleştirdik. */
int TakimBul(char takim_Char){
	int i;
	for(i=0;i<ayar.takim_sayisi;i++){
		if(takim_Char == tkm[i].takma_kisaAd){
			return i;
		}
	}
	return -1;
}


/* Ev sahibi ve misafir takım bir kez daha maç yapar ise uyarı veren fonksiyon tanımlandı. */
int MacKontrol(char takim1, int skor1, char takim2, int skor2){
	int i;
	for(i=0;i<ayar.mac_sayisi;i++){
		if(maclar[i].evsahibi == takim1 & maclar[i].misafir == takim2){
			return 0;  /* False durumu */
		}
	}
	maclar[ayar.mac_sayisi].evsahibi = takim1;
	maclar[ayar.mac_sayisi].misafir = takim2;
	maclar[ayar.mac_sayisi].evSkor = skor1;
	maclar[ayar.mac_sayisi].misSkor = skor2;
	ayar.mac_sayisi++;
	return 1;   /* True durumu */
}

/* Takımlar bu fonksiyonda tanımlandıktan sonra ilgili hesaplamalar yapıldı. */
void TekMacGirisi(char takim1, int skor1, char takim2, int skor2){ // A 3 B 0
	int takim1Index = TakimBul(takim1); /* Girilen ev sahibi takımın sayısal değerini bulmak için TakimBul fonksiyonuna yolladık. */
	int takim2Index = TakimBul(takim2); /* Girilen misafir takımın sayısal değerini bulmak için TakimBul fonksiyonuna yolladık. */
	int i;

	/* Mevcut olmayan takım girişi yapıldığında uyarı verilir. */
	if(takim1Index == -1 | takim2Index == -1){
		printf("Takım Kısa Ad Girişi Hatalı!\n");
		return;
	}

	/* MacKontrol fonksiyonuna maç girişini yolladık daha önceden aynı ev sahibinde , aynı maçın oynanıp oynanmadığı kontrolü yapıldı. */
	if(MacKontrol(takim1, skor1, takim2, skor2) == 0){
		printf("Bu maç daha önceden eklenmiş!: Takım 1: %s - Takım 2: %s\n",tkm[takim1Index].takim_ad,tkm[takim2Index].takim_ad);
		return;
	}

	/* Puan tablosundaki değerlerin hesaplanmaları yapıldı */
	int averaji;
	if(skor1>skor2)
	{
		averaji = skor1-skor2; /* Maçın averajının hesaplanması */
		tkm[takim1Index].puan +=ayar.galibiyet; /* Kazanan takıma galibiyet puanı eklendi */
		tkm[takim2Index].puan +=ayar.maglubiyet; /* Kaybeden takıma mağlubiyet puanı eklendi */
		tkm[takim1Index].averaj += averaji; /* Kazanan takıma averaj puanı eklendi */
		tkm[takim2Index].averaj -= averaji; /* Kaybeden takımdan averaj puanı çıkartıldı */
		tkm[takim1Index].oynadigimacsayisi++; /* Ev sahibi takımın oynadığı maç sayısı arttırıldı */
		tkm[takim2Index].oynadigimacsayisi++; /*Misafir takımın oynadığı maç sayısı arttırıldı */
		tkm[takim1Index].galibiyetsayisi++; /* Ev Sahibi takımın galibiyet sayısı arttırıldı */
		tkm[takim2Index].maglubiyetsayisi++; /* Misafir takımın mağlubiyet sayısı arttırıldı */
		tkm[takim1Index].attigigolsayisi += skor1; /* Ev sahibi takımın attığı gol sayısı */
		tkm[takim2Index].attigigolsayisi += skor2; /* Misafir takımın attığı gol sayısı */
		tkm[takim1Index].yedigigolsayisi += skor2; /* Ev sahibi takımın yediği gol sayısı */
		tkm[takim2Index].yedigigolsayisi += skor1;	/* Misafir takımın yediği gol sayısı */

	}
	else if (skor2>skor1)
	{
		averaji = skor2-skor1;  /* Maçın averajının hesaplanması */
		tkm[takim2Index].puan +=ayar.galibiyet; /* Kazanan takıma galibiyet puanı eklendi */
		tkm[takim1Index].puan +=ayar.maglubiyet; /* Kaybeden takıma mağlubiyet puanı eklendi */
		tkm[takim2Index].averaj += averaji;  /* Kazanan takıma averaj puanı eklendi */
		tkm[takim1Index].averaj -=averaji;  /* Kaybeden takımdan averaj puanı çıkartıldı */
		tkm[takim2Index].oynadigimacsayisi++; /* Ev sahibi takımın oynadığı maç sayısı arttırıldı */
		tkm[takim1Index].oynadigimacsayisi++; /*Misafir takımın oynadığı maç sayısı arttırıldı */
		tkm[takim2Index].galibiyetsayisi++; /* Misafir takımın galibiyet sayısı arttırıldı */
		tkm[takim1Index].maglubiyetsayisi++; /* Ev sahibi takımın mağlubiyet sayısı arttırıldı */
		tkm[takim2Index].attigigolsayisi += skor2; /* Misafir takımın attığı gol sayısı */
		tkm[takim1Index].attigigolsayisi += skor1; /* Ev sahibi takımın attığı gol sayısı */
		tkm[takim2Index].yedigigolsayisi += skor1; /* Misafir takımın yediği gol sayısı */
		tkm[takim1Index].yedigigolsayisi += skor2;	/* Ev sahibi takımın yediği gol sayısı */
	}
	else if(skor1 == skor2)
	{
		tkm[takim1Index].puan += ayar.beraberlik; /* Ev sahibi takıma beraberlik puanı eklendi */
		tkm[takim2Index].puan += ayar.beraberlik; /* Misafir takıma beraberlik puanı eklendi */
		tkm[takim1Index].oynadigimacsayisi++; /* Ev sahibi takımın oynadığı maç sayısı arttırıldı */
		tkm[takim2Index].oynadigimacsayisi++; /* Misafir takımın oynadığı maç sayısı arttırıldı */
		tkm[takim1Index].beraberliksayisi++;  /* Ev sahibi takımın beraberlik sayısı arttırıldı */
		tkm[takim2Index].beraberliksayisi++;  /* Misafir takımın beraberlik sayısı arttırıldı */
		tkm[takim1Index].attigigolsayisi += skor1; /* Ev sahibi takımın attığı gol sayısı arttırıldı */
		tkm[takim2Index].attigigolsayisi += skor2; /* Misafir takımın attığı gol sayısı arttırıldı */
		tkm[takim1Index].yedigigolsayisi += skor2; /* Ev sahibi takımın yedigi gol sayısı arttırıdlı */
		tkm[takim2Index].yedigigolsayisi += skor1; /* Misafir takımın yediği gol sayısı arttırıldı */
	}

	printf("Bilgi Mesaji: %s %d - %d %s Verisi Kaydedildi.\n",tkm[takim1Index].takim_ad, skor1, skor2, tkm[takim2Index].takim_ad);

}

/* Ev sahibi takım ve skoru , Misafir takım ve skoru klavyeden elle girilmesi */
void MacGirisleriElle()
{
	int i;
	int macsayisi;
	int skor1;
	int skor2;
	char takim1,takim2;

	printf("Yapilacak mac sayisini giriniz: "); scanf("%d",&macsayisi);

	for(i=0; i<macsayisi; i++)
	{
		printf("%d. Mac skoru: ", i+1); scanf(" %c %d %c %d",&takim1,&skor1,&takim2,&skor2);
		TekMacGirisi(takim1, skor1, takim2, skor2); /* Maç değerleri TekMacGirisi fonksiyonuna aktarıldı. */
	}

}

char* buyukYaz(char* yazi){

	char* yazi2;
	int i = 0;
	int offset = 'a' - 'A';

	while(yazi[i] != '\0')
    {
    	if(yazi[i] >= 'a' || yazi[i] < 0)
    	{
    		yazi2[i] = yazi[i] - offset;
		}
		i++;
	}

	return yazi2;
}


/* Puan tablosu puantablosu.txt dosyasına yazıldı. */
void DosyayaPuanTablosuYaz()
{
	/*puantablosu.txt dosyası oluşturulup yeni bir dosya açıldı ve içerisine puan tablosunun güncellenmiş hali geldi.*/
	FILE *file;
	int i;

	if ((file = fopen ("puantablosu.txt", "w")) == NULL)
	{
   		printf("Dosya açma hatası!\n");
      	exit(1);
  	}

	fprintf(file ," |KısaAd| |Ad| |OM| |G| |B| |M| |AG| |YG| |A|  |P|\n");

  	for(i=0; i<ayar.takim_sayisi; i++)
  	{
  		fprintf(file , "%c  %s  %d   %d   %d   %d    %d      %d      %d      %d\n",tkm[i].takma_kisaAd,tkm[i].takim_ad,tkm[i].oynadigimacsayisi,
		  tkm[i].galibiyetsayisi,tkm[i].beraberliksayisi,	tkm[i].maglubiyetsayisi,tkm[i].attigigolsayisi,tkm[i].yedigigolsayisi,tkm[i].averaj,tkm[i].puan);

	}
	fclose(file);
}


/* Maç sonuçları dosyadan okunup puan tablosuna yansıtıldı. */
void MacGirisleriOku(){

	char takim1, takim2;
	int skor1, skor2;

	FILE *file;

	if((file=fopen("maclar.txt", "r")) == NULL){
		printf("Dosya açılamadı!\n");
      	exit(1);
	}

	while(!feof(file))
	{
		fscanf(file, " %c %d %c %d", &takim1,&skor1,&takim2,&skor2);
		TekMacGirisi(takim1,skor1,takim2,skor2);
	}

	fclose(file);
}


/* Puana sırasına göre takımların sıralaması yapıldı */
void PuanSirala(){

	/* Bubble sort sıralama algoritması kullanıldı */
	char takim_kisaAd[20] = "|TakimKisaAd|";
	char takim_adi[15] = "|Takim adi|";
	int i;
	int j;

		for(i=0;i<ayar.takim_sayisi-1;i++){
		 for(j=0;j<ayar.takim_sayisi-i-1;j++){
				if(tkm[j].puan<tkm[j+1].puan || tkm[j].puan==tkm[j+1].puan && tkm[j].averaj<tkm[j+1].averaj) /* Puanlar eşit ise averajı yüksek olan takım üst sıraya yükselir */
				{
				 	temp = tkm[j+1];  /* NOT : temp değişkeni struct yapısında tanımlandı. */
				 	tkm[j+1] = tkm[j];
				 	tkm[j] = temp;
				}
			}
		}

	/* Takımların puan sırasına göre sıralanmış hali puan tablosuna yazıldı. */
 	printf("%15s %15s |OM|  |G| |B| |M| |AG| |YG| |A| |Puan|\n",takim_kisaAd, takim_adi);

	for(i=0;i<ayar.takim_sayisi;i++)
	{
		printf("%13c  %15s   %d     %d   %d   %d   %d     %d   %d    %d\n",tkm[i].takma_kisaAd,tkm[i].takim_ad,tkm[i].oynadigimacsayisi,tkm[i].galibiyetsayisi,tkm[i].beraberliksayisi,
		tkm[i].maglubiyetsayisi,tkm[i].attigigolsayisi,tkm[i].yedigigolsayisi,tkm[i].averaj,tkm[i].puan);
	}

}


void HarfSirasi()
{
	char takim_kisaAd[20] = "|TakimKisaAd|";
	char takim_adi[15] = "|Takim adi|";
	int i;
	int j;
	int k;



		 for(i=0;i<ayar.takim_sayisi;i++){
		 for(j=i+1;j<ayar.takim_sayisi;j++){
				 for(k=0; k<TAKIMADSINIR; k++){
				 	if(tkm[i].takim_ad[k] > tkm[j].takim_ad[k])
					 {
					 	temp = tkm[j];  /* NOT : temp değişkeni struct yapısında tanımlandı. */
					 	tkm[j] = tkm[i];
					 	tkm[i] = temp;
					 	break;
					 }
					 else if(tkm[i].takim_ad[k] < tkm[j].takim_ad[k]) break;
				 }
			}
		 }

	/* Takımların alfabetik sırasına göre sıralanmış hali puan tablosuna yazıldı. */
 	printf("%15s %15s |OM|  |G| |B| |M| |AG| |YG| |A| |Puan|\n",takim_kisaAd, takim_adi);

	for(i=0;i<ayar.takim_sayisi;i++)
	{
		printf("%13c  %15s   %d     %d   %d   %d   %d     %d   %d    %d\n",tkm[i].takma_kisaAd,tkm[i].takim_ad,tkm[i].oynadigimacsayisi,tkm[i].galibiyetsayisi,tkm[i].beraberliksayisi,
		tkm[i].maglubiyetsayisi,tkm[i].attigigolsayisi,tkm[i].yedigigolsayisi,tkm[i].averaj,tkm[i].puan);
	}

}

/* Puan tablosunun güncellenmiş hali tabloya yansıtıldı. */
void PuanTablosuGoster(){

	char takim_kisaAd[20] = "|TakimKisaAd|";
	char takim_adi[15] = "|Takim adi|";

	printf("%15s %15s |OM|  |G| |B| |M| |AG| |YG| |A| |Puan|\n",takim_kisaAd, takim_adi);
	int i;
	for(i=0; i<ayar.takim_sayisi; i++)
	{
		printf("%13c  %15s   %d     %d   %d   %d   %d     %d   %d    %d\n",tkm[i].takma_kisaAd,tkm[i].takim_ad,tkm[i].oynadigimacsayisi,tkm[i].galibiyetsayisi,tkm[i].beraberliksayisi,
		tkm[i].maglubiyetsayisi,tkm[i].attigigolsayisi,tkm[i].yedigigolsayisi,tkm[i].averaj,tkm[i].puan);
	}
}

/* Menüden yapılacak işlemi seçmek için switch-case yapısı kullanıldı */
void Menu(){

	 int secim;
	 while(secim!=8)
	 {
	 	 printf("\n------------------------\n");
 		 printf("[1] Skor Gir\n");
		 printf("[2] Skor Dosyadan Oku\n");
 		 printf("[3] Takma Isme Gore Puan Durumu Tablosu Sirala\n");
 		 printf("[4] Puan Durumu Tablosunu Puan Sirasina Gore Sirala\n");
 		 printf("[5] Puan Durumu Tablosunu Dosyaya Yaz\n");
 		 printf("[6] Puan Durumu Tablosunu Harf Sirasina Gore Sirala Yaz\n");
 		 printf("[7] Puan Durumu Tablosunu Buyuk Harflerle Yaz\n");
 		 printf("[8] Programi kapat\n\n");
   	 	 printf("Islemi seciniz: "); scanf("%d",&secim);

   	 	switch(secim)
   		{
   			case 1:
			   	printf("1. islemi sectiniz \n") ;
			   	MacGirisleriElle();
				break;

   			case 2:
			   printf("2. islemi sectiniz \n");
			   MacGirisleriOku();
			   break;

   			case 3:
			   printf("3. islemi sectiniz \n");
			   PuanTablosuGoster();
			   break;

   			case 4:
			   printf("4. islemi sectiniz \n");
			   PuanSirala();
			   break;

			case 5:
			   printf("5. islemi sectiniz \n");
			   DosyayaPuanTablosuYaz();
			   break;

			case 6:
			   printf("6. islemi sectiniz \n");
			   HarfSirasi();
			   break;

			case 7:
			   printf("7. islemi sectiniz \n");
			   char yazi[11] = "asçöiı";
			   printf(buyukYaz(yazi));
			   break;

   			case 8:
			   exit(1);
			   break;

			default:
			   printf("\n******EN BUYUK TRABZONSPOR****** \n");
			   exit(1);
			   break;
		}
	}
}



int main()
{
	setlocale(LC_ALL, "Turkish"); // Türkçe karakter tanımlaması yapıldı

	ayar.mac_sayisi = 0;
	AyarGirisi();
	TakimGirisi();
	Menu();

	return 0;
}
