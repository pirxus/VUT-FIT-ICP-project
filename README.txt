ICP 2019/2020 - projekt

Autori:
 - Simon Sedlacek - xsedla1h
 - Radim Lipka - xlipka02

## Prelozeni a spusteni projektu

Projekt lze prelozit prikazem make run v korenovem adresari projektu.
Produkty prekladu i spustitelny soubor se vygeneruji do slozky src - doporucujeme projekt spoustet pomoci prikazu 'make run'.

Dokumentaci k projektu lze vytvorit prikazem make doxygen v korenovm adresari. Dokumentace se vygeneruje do slozky doc.

Pred zacatkem simulace je treba nacist zdrojove soubory ze slozky examples - vyuzijte k
tomu menu File, ktere se nachazi v leve casti okna.

## Vstupni soubory projektu - mapa, zastavky, linky

- V souboru streetList.csv se na kazdem radku nachazi jmeno a souradnice zacatku a konce ulice, ktera je soucasti mapy.
- V souboru stopList.csv se na kazdem radku nachazi jmeno zastavky, jmeno ulice, na ktere se nachazi a pozice zastavky od zacatku ulice.
- Soubor lineList.csv obsahuje informace o linkach a spojenich. Cislo linky, seznam ulic, po kterych dana linka jede, seznam zastavek, na kterych zastavuje a casy prijezdu jednotlivych spoju na zastavku. Jednotlive linky jsou od sebe oddeleny prazdnym radkem.

## Ovladatelne prvky gui

- V levem hornim rohu lze nalezt tlacitka na nacteni dat ze souboru.
- Na leve strane aplikace se nachazi posuvnik a tlacitka na priblizovani a oddalovani sceny.
- V nahore v pravem sloupci se naleza ukazatel casu s moznosti nastavit vlastni cas po kliknuti. Hned pod ukazatelem casu se nachazi tlacitko na spusteni a zastaveni simulace a posuvnik na zrychlovani nebo zpomalovani casu. 
- Pri kliknuti na znacku spoje na mape se v pravém sloupci zobrazi itinerar daneho spoje.
- Pro uzavreni ulice dvakrat kliknete na ulici, ktera je urcena k uzavreni a pote nastavte vyberte ulice, kudy ma vest objizdka. Tyto ulice je nutne specifikovat ve spravnem poradi, avsak nezalezi na smeru.

## Filosofie navrhu

Strukturu programu jsme rozdelili na cast obsahujici pouze programovou logiku (tj.
nacitani vstupu, reprezentace spoju a mapy, pocitani tras a pozic spoju atd..) a na cast
view, ktera se stara o zobrazeni techto 'simulovanych' dat.

Kazda vyznacna trida backendove casti je na prvni pohled odlisitelna nazvem modulu, ktery
zacina vzdy velkym pismenem (napr. Line.h, Connection.h, aj.). Tyto moduly vytvareji
abstrakci realneho systemu hromadne dopravy - kazda linka ma seznam spoju, pricemz kazdy
spoj ma svuj vlastni jizdni rad, zpozdeni.

Hlavnim modulem one backendove casti je modul s tridou PublicTransport, ktera uchovava
vsechna nactena data a ridi simulaci spoju. Tato trida ma jako dva hlavni atributy 
mapu - nactena soustava ulic a zastavek - a lines - coz je vektor nactenych linek.

Na tento hlavni modul se pak primo napojuje view - centralni tridou view je trida
MainWindow. Podle interakce s uzivatelem posila view pozadavky tride PublicTransport
a aktualni stav dopravy jsou v hlavnim okne zobrazovany jednak v graficke scene a na
postrannich listach.

Zobrazeni jednotlivych abstrakci programu (spoje, ulice, ...) jsou rizeny soustavou trid
ViewStreet, ViewStop a ViewConnection. Tyto tridy jsou odvozene od bazovych grafickych
trid poskytovanych Qt toolkitem (pro ulici QGraphicsLineItem, pro spoje
QGraphicsEllipseItem, atd..) - podle toho, jakym zpusobem maji byt v graficke scene
reprezentovany. Tyto tridy si navic uchovavaji ukazatele primo na objekty, ktere maji
reprezentovat. Kdyz tedy trida PublicTransport aktualizuje pozice vsech spoju na mape,
informuje o tom MainWindow, ktere graficke scene preda pozadavek na prekresleni vsech
zobrazovanych elementu - tyto elementy (ViewStreet, ViewConnection,.. ) zmeni sve
vlastnosti v graficke scene podle aktualizovanych parametru objektu, ktere maji graficky
reprezentovat (napriklad nova pozice spoje na mape).

Tyto grafice tridy nam take umoznuji zprostredkovat interakci uzivatele s jednotlivymi
grafickymi elementy ve scene - pokud se napriklad uzivatel rozhodne uzavrit ulici, dany
objekt o tom informuje tridu MainWindow, ktera sesbira dalsi informace a pote posle
tride PublicTransport pozadavek na uzavreni ulice a vytvoreni objizdky. Je tedy dulezite
rici, ze tyto graficke tridy nikdy 'nezapisuji' do objektu, ktery reprezentuji - jejich
ucelem je pouze zobrazovat simulovana data a zprostredkovani interakce.
