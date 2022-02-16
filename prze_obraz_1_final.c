#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>

#define MAX 512            /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */

/************************************************************************************
 * Funkcja wczytuje obraz PGM z pliku do tablicy       	       	       	       	    *
 *										                                                              *
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM			                *
 * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany obraz	         	      *
 * \param[out] wymx szerokosc obrazka						                                    *
 * \param[out] wymy wysokosc obrazka						                                    *
 * \param[out] szarosci liczba odcieni szarosci					                            *
 * \return liczba wczytanych pikseli						                                    *
 ************************************************************************************/

int czytaj(FILE *plik_we,int obraz_pgm[][MAX],int *wymx,int *wymy, int *szarosci) {
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j;

  /*Sprawdzenie czy podano prawidłowy uchwyt pliku */
  if (plik_we==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien być P2 */
  if (fgets(buf,DL_LINII,plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || (buf[1]!='2') || koniec) {  /* Czy jest magiczne "P2"? */
    fprintf(stderr,"Blad: To nie jest plik PGM\n");
    return(0);
  }

  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(plik_we))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,plik_we)==NULL)  /* Przeczytaj ja do bufora                */
	koniec=1;                                 /* Zapamietaj ewentualny koniec danych    */
    }  else {
      ungetc(znak,plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(plik_we,"%d %d %d",wymx,wymy,szarosci)!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }

  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/
  for (i=0;i<*wymy;i++) {
    for (j=0;j<*wymx;j++) {
      if (fscanf(plik_we,"%d",&(obraz_pgm[i][j]))!=1) {
	fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	return(0);
      }
    }
  }
  return *wymx* *wymy;   /* Czytanie zakonczone sukcesem    */
}                        /* Zwroc liczbe wczytanych pikseli */





/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */
void wyswietl(char *n_pliku) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
}



/* Tutaj zaczyna się fragment programu, który pisałem bądź edytowałem */


/* Zapisanie obrazu do pliku o zadanej nazwie */
void zapis (FILE* plik,int wymx, int wymy, int szarosci, int tab[][MAX])
{
 fprintf (plik, "P2\n");                                        /* Wypisanie do pliku znaku magicznego w 1 linijce */
 fprintf (plik, "%d %d\n",wymx,wymy);                           /* Wypisanie do pliku wymiarów obrazka w 2 linijce */
 fprintf (plik, "%d\n",szarosci);                               /* Wypisanie do pliku liczby mówiącej o ilości odcieni szarości   */
 int i,j;
 for (i=0;i<wymy;i++)                                           /* Wypisywanie do pliku informacji o kolorze kolejnych pikseli,   */
 {                                                              /* za pomocą zwykłej pętli for */
    for (j=0; j<wymx; j++)                                      /* Bardzo ważna w tym miejscu jest spacja po '%d', gdyż bez niej  */
    fprintf (plik, "%d ", tab[i][j]);                           /* nie będzie odstępu pomiędzy kolejnymi informacjami o pikselach */
 }                                                              /* i obrazek nie wyświetli się poprawnie */
}


/* Negatyw */
void negatyw (int wymx, int wymy, int szarosci, int tab[][MAX])
{
 int i,j;                                                       /* Są tutaj dwie pętle for, które sprawiają, że każdy element zapisany w  */
 for (i=0; i<wymy; i++)                                         /* tablicy zmieni swoją wartość zgodnie z definicją tworzenia negatywu,   */
 for (j=0; j<wymx; j++)                                         /* a więc im 'bielszy' jest element tym będzie 'ciemniejszy' po wykonaniu */
  tab[i][j]=szarosci-tab[i][j];                                 /* funkcji. Ta zmiana jest zobrazowana w 4 linijce kodu funkji */ 
}


/* Progowanie */
void progowanie (int wymx, int wymy, int szarosci, int tab[][MAX],int prog)
{
 int i,j;
 for (i=0; i<wymy; i++)
 for (j=0; j<wymx; j++)
  if (tab[i][j]<=szarosci*prog/100.0)                           /* Ten if sprawdza, czy dany piksel jest pod czy nad podanym progiem w %     */
  {          
      tab[i][j]=0;                                              /* Każda wartość, która jest pod progiem dostaje wartość 0, a każda, która   */
  }                                                             /* jest nad progiem otrzymuje wartość równą maksymalnemu odcieniowi szarości */
  else
  {
      tab[i][j]=szarosci;
  }
}


/* Zmiana poziomów */
void zmiana_poziomow (int wymx, int wymy, int szarosci, int tab[][MAX],int czern,int biel)
{
int i,j;
int cz,bie;

cz=szarosci*czern/100.0;                                        /* Przedstawienie danych w % */
bie=szarosci*biel/100.0;
                                                                /* W podanym warunku są rozważone wszystkie przypadki tworzenia nowych poziomów  */
for (i=0; i<wymy; i++)                                          /* bieli i czerni w programie, podane w pliku z informacjami do tego zadania     */
for (j=0; j<wymx; j++)
  {
    if (tab[i][j]<=cz)                                             
    {                                                           /* Wartości poniżej nowej wartości czerni otrzymują wartość 0 */
      tab[i][j]=0;                                                
    }     
    else if (cz<tab[i][j] && tab[i][j]<bie)                     /* Wartości pośrednie są rozciągane na wszystkie poziomy szarości zawarte */
    {                                                           /* w obrazie */
      tab[i][j]=(tab[i][j]-cz)*(szarosci/(bie-cz));
    }
    else                                                        /* Wartości powyżej nowej wartości bieli otrzymają wartość */
      tab[i][j]=szarosci;                                       /* maksymalną dla danego programu */
  }
}


/* Konturowanie */
void konturowanie (int wymx, int wymy, int szarosci, int tab[][MAX])       /* Wyostrzanie konturów obrazu */
{
int i,j;
for (i=0; i<wymy-1; i++)                                                   /* W tym miejscu '-1' w pętli, bo w przeciwnym wypadku przejdziemy poza zakres    */
for (j=0; j<wymx-1; j++)                                                   /* tablicy (segmentation fault), a 1 piksel doda tylko ramkę o grubości 1 piksela */
    tab[i][j]=abs(tab[i+1][j]-tab[i][j])+abs(tab[i][j+1]-tab[i][j]);       /* 'abs' z biblioteki matematycznej to moduł */
}


/* DODATKOWE - Rozciąganie hitogramu */
void histogram (int wymx, int wymy, int szarosci, int tab[][MAX])           /* Może być nie zawsze widoczne, bo ta operacja ma sens gdy nie w pełni jest */
{                                                                           /* wykorzystywany zakres odcieni szarości obrazka */
int i,j;
int min=tab[0][0],max=tab[0][0];                                            /* Inicjuję w ten sposób, aby potem porównywać z każdym innym elementem */
                                                                            /* W ten sposób unikam błędów wartości skrajnych */
for (i=0; i<wymy; i++)
for (j=0; j<wymx; j++)
{
    if (min>tab[i][j])                                                      /* Szukanie wartości minimalnej i maksymalnej odcieni szarości w całym obrazku */
        min=tab[i][j];

    if (max<tab[i][j])
        max=tab[i][j];
}
for (i=0; i<wymy; i++)
for (j=0; j<wymx; j++)
    tab[i][j]=(tab[i][j]-min)*(szarosci/(max-min));                         /* Rozciąganie wartości wszystkich pikseli (o ile ma to sens) do wartości bardziej */
}                                                                           /* wyśrodkowanych; w pełni wykorzystujących zadeklarowane możliwości obrazka */





/* Funkcja główna */
int main() 
{
  int obraz[MAX][MAX];              /* do maksymalnych wymiarów obrazka                                   */
  int wymx,wymy;                    /* szerokosc i wysokosc obrazka                                       */
  int odcieni;                      /* ilość odcieni szarości                                             */
  int prog;                         /* zmienna potrzebna do ustalania progu w progowaniu                  */
  char menu;                        /* zmienna pomocnicza do menu                                         */
  int odczytano=0;                  /* zmienna potrzebna do czytania obrazka oraz do sprawdzania błędów   */
  FILE *plik;                       /* zmienna wskaźnikowa dzięki której czytamy i zapisujemy obrazek     */
  char nazwa[100];                  /* tablica przechowywująca nazwę obrazka                              */
  int biel,czern;                   /* dwie zmienne potrzebne do zmiany poziomów czerni i bieli           */
  


  system("clear");
while(1)
{
  printf("Program do edycji obrazkow PGM\nOpcje programu:\n\n");
  printf("Wykonujac operacje zmiany obrazka bazowego nalezy za kazdym razem wczytywac obrazek, \n");
  printf("bo w przeciwnym wypadku bedzie sie wykonywac, np progowanie na obrazku po konturowaniu.\n\n");
  /* menu */
  printf("Kliknij 'o' aby wczytac plik\n");        
  printf("Kliknij 'z' aby zapisac plik\n");
  printf("Kliknij 'w' aby wyswietlic plik bez zapisu\n");
  printf("Kliknij 'k' aby wykonac konturowanie\n");
  printf("Kliknij 'n' aby wykonac negatyw\n");
  printf("Kliknij 'd' aby wykonac zmiane poziomow\n");
  printf("Kliknij 'p' aby wykonac progowanie\n");
  printf("Kliknij 'h' aby wykonac rozciaganie histogramu\n");
  printf("Kliknij 'x' aby zakonczyc\n\n");

  printf("Co chcesz zrobić: ");
  scanf ("%c",&menu);

  switch(menu)                                                  /* Menu najprościej robi się za pomocą switch'a */
  {
    case 'o'://odczyt
      printf("Podaj nazwe pliku: ");
      scanf("%s",nazwa);//&nazwa[0]
      plik=fopen(nazwa,"r");                                    /* Otworzenie pliku do 'r', a zatem do odczytu */

      if(plik!=NULL)
      {
        odczytano=czytaj (plik,obraz,&wymx,&wymy,&odcieni);     /* Wywołanie funkcji czytaj. To co zwraca ta funkcja jest przypisywane do zmiennej odczytano */ 
        fclose(plik);                                           /* Zamknięcie pliku */
        printf("Wykonano poprawnie :)");
      }
      else
        fprintf(stderr,"BLAD, podales bledna nazwe\n");         /* Komunikat w razie błędu na standardowe wyjście */
    break;

    case 'z'://zapis
      if (odczytano!=0)
        {
         printf("Podaj nazwe pliku do zapisu: ");
         scanf("%s",nazwa);//&nazwa[0]
         plik=fopen(nazwa,"w");                                   /* Otworzenie pliku do pisania w nim     */
         zapis (plik,wymx,wymy,odcieni,obraz);                    /* Wywołanie funkcji: zapis              */
         fclose(plik);                                            /* Zamknięcie pliku                      */
         printf("Wykonano poprawnie :)");
        }
      else
        fprintf(stderr,"BLAD: najpierw wczytaj plik \n");         /* Komunikat w razie błędu na standardowe wyjście */
    break;

    case 'w'://wyświetlenie
      if (odczytano!=0)                                           /* Sprawdzenie czy został wczytany plik  */
        wyswietl (nazwa);                                         /* Wywołanie funkcji: wyświetl           */
      else
        fprintf(stderr,"BLAD: najpierw wczytaj plik \n");         /* Komunikat w razie błędu na standardowe wyjście */
    break;

    case 'k'://konturowanie
      if (odczytano!=0)
      {
        konturowanie (wymx,wymy,odcieni,obraz);                   /* Wywołanie funkcji: konturowanie       */
        plik=fopen("tmp","w");                                    /* Otworzenie pliku do pisania w nim     */
        zapis(plik,wymx,wymy,odcieni,obraz);                      /* Wywołanie funkcji: zapis              */
        fclose(plik);                                             /* Zamknięcie pliku                      */
        wyswietl("tmp");                                          /* Wywołanie funkcji: wyświetl           */
      }
      else
        fprintf(stderr,"BLAD: najpierw wczytaj plik \n");         /* Komunikat w razie błędu na standardowe wyjście */
    break;

    case 'n'://negatyw
      if (odczytano!=0)
      {
        negatyw (wymx,wymy,odcieni,obraz);                        /* Wywołanie funkcji: negatyw            */
        plik=fopen("tmp","w");                                    /* Otworzenie pliku do pisania w nim     */
        zapis(plik,wymx,wymy,odcieni,obraz);                      /* Wywołanie funkcji: zapis              */
        fclose(plik);                                             /* Zamknięcie pliku                      */
        wyswietl("tmp");                                          /* Wywołanie funkcji: wyświetl           */
      }
      else
        fprintf(stderr,"BLAD: najpierw wczytaj plik \n");         /* Komunikat w razie błędu na standardowe wyjście */
    break;

    case 'p'://progowanie
      if (odczytano!=0)                 
      {
        printf("Podaj prog od 0 do 100: ");
        scanf("%d",&prog);                                          /* Podanie progu */
        if (prog<=100 && prog>=0)                                   /* Warunek wynikający z podawania progu w procentach */
        {                                                         
          progowanie (wymx,wymy,odcieni,obraz,prog);                /* Wywołanie funkcji: progowanie         */
          plik=fopen("tmp","w");                                    /* Otworzenie pliku do pisania w nim     */
          zapis(plik,wymx,wymy,odcieni,obraz);                      /* Wywołanie funkcji: zapis              */
          fclose(plik);                                             /* Zamknięcie pliku                      */
          wyswietl("tmp");                                          /* Wywołanie funkcji: wyświetl           */
        }
        else 
          fprintf(stderr,"BLAD: prog moze byc od 0 do 100 \n");
      }
    else
        fprintf(stderr,"BLAD: najpierw wczytaj plik \n");           /* Komunikat w razie błędu na standardowe wyjście */
    break;

    case 'd'://zmiana poziomów
      if (odczytano!=0)
      {
       printf("Pamietaj, ze czern < biel!\n\n");                    /* Podanie w % zmian poziomów dla bieli i czerni */
       printf("Podaj czern od 0 do 100: ");
       scanf("%d",&czern);
       printf("Podaj biel od 0 do 100: ");
       scanf("%d",&biel);

        if ((czern>=0 && czern<=100) && (biel>=0 && biel<=100) && czern<biel)
        {                                                           /* Warunki wynikające z własności zmiany poziomów */
          zmiana_poziomow (wymx,wymy,odcieni,obraz,czern,biel);     /* Wywołanie funkcji: zmiana_poziomow    */
          plik=fopen("tmp","w");                                    /* Otworzenie pliku do pisania w nim     */
          zapis(plik,wymx,wymy,odcieni,obraz);                      /* Wywołanie funkcji: zapis              */
          fclose(plik);                                             /* Zamknięcie pliku                      */
          wyswietl("tmp");                                          /* Wywołanie funkcji: wyświetl           */
        }
        else 
          fprintf(stderr,"\nBLAD: nie spelnasz co najmniej jednego z warunkow:\n1. czern i biel naleza do przedzialu (0,100)\n2. czern < biel \n"); 
      }
      else
        fprintf(stderr,"BLAD: najpierw wczytaj plik \n");         /* Komunikat w razie błędu na standardowe wyjście */
    break;


    case 'h'://rozciąganie histogramu
      if (odczytano != 0)
      {
        histogram (wymx,wymy,odcieni,obraz);                      /* Wywołanie funkcji: histogram          */
        plik=fopen("tmp","w");                                    /* Otworzenie pliku do pisania w nim     */
        zapis(plik,wymx,wymy,odcieni,obraz);                      /* Wywołanie funkcji: zapis              */
        fclose(plik);                                             /* Zamknięcie pliku                      */
        wyswietl("tmp");                                          /* Wywołanie funkcji: wyświetl           */
      }
      else
        fprintf(stderr,"BLAD: najpierw wczytaj plik \n");         /* Komunikat w razie błędu na standardowe wyjście */
    break;

    case 'x':
      printf("KONIEC\n");                                         /* Zakończenie programu      */
      system("rm tmp");                                           /* Usuwa tymczasowy plik tmp */ 
      return 0;
    break;

    default:
      fprintf(stderr,"BLAD: zly znak \n");                        /* Komunikat w razie wpisania znaku nie będącego na liście */

    }
    scanf ("%c",&menu);                                           /* Wprowadzenie zmiennej, aby getchar() działał */
    //sleep (1);                                                  /* Ewentualna możliwość zastosowania funkcji sleep() zamiast getchar()          */
    printf("\n\nAby przejsc dalej kliknij 'Enter'");              /* Polecenie sleep(1) czeka 1 sekundę i przechodzi dalej w programie            */
    getchar();                                                    /* Polecenie getchar() czeka na wprowadzenie jakiejś danej aby przejść dalej    */
    system("clear");                                              /* Polecenie system("") pozwala na użycie poleceń terminala w naszym programie, */
}                                                                 /* w tym przypadku umożliwia korzystanie z 'clear' */

return odczytano;
}
