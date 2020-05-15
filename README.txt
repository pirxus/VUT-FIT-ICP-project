ICP 2019/2020 - projekt

Autori:
 - Simon Sedlacek - xsedla1h
 - Radim Lipka - xlipka02


# Prelozeni a spusteni projektu

Projekt lze prelozit prikazem make run v korenovem adresari.
Produkty prekladu i spustitelny soubor se vygeneruji do slozky src.

Dokumentaci k projektu lze vytvorit prikazem make doxygen v korenovm adresari. Dokumentace se vygeneruje do slozky doc.

Pred zacatkem simulace je treba nacist zdrojove soubory ze slozky
examples.

# Zdrojove soubory projektu

V souboru streetList.csv se na kazdem radku nachazi jmeno a souradnice zacatku a konce ulice, ktera je soucasti mapy.

V souboru stopList.csv se na kazdem radku nachazi jmeno zastavky, jmeno ulice, na ktere se nachazi a pozice zastavky od zacatku ulice.

Soubor lineList.csv obsahuje informace o linkach a spojenich. 
Cislo linky, seznam ulic, po kterych dana linka jede, seznam zastavek, na kterych zastavuje a casy prijezdu jednotlivych spoju na zastavku.

# Ovladatelne prvky gui

Na leve strane aplikace se nachazi posuvnik a tlacitka na priblizovani a oddalovani sceny.

V nahore v pravem sloupci se naleza ukazatel casu s moznosti nastavit vlastni cas po kliknuti. Hned pod ukazatelem casu se nachazi tlacitko na spusteni a zastaveni simulace a posuvnik na zrychlovani nebo zpomalovani casu. 

Pri kliknuti na znacku spoje na mape se v pravém sloupci zobrazi itinerar daneho spoje.

Pro uzavreni ulice staci kliknout na ulici, ktera je urcena k uzavrani a pote nastavit objizdku.