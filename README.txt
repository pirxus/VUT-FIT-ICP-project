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