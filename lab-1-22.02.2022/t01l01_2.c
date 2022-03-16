/**
Lab. 01 Tema 1
Titlul temei:   Analizarea argumentelor introduse pe linia de comandă prin apel 
        functie getopt_long_only().
Data livrarii: 06.03.2022
Student: Anton Diana-Ana-Maria
An 3 IA
----------
<
Programul a fost rulat sub Windows Subsystem for Linux cu:
    $gcc --version
    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0

    $clang --version
    clang version 10.0.0-4ubuntu1 
    Target: x86_64-pc-linux-gnu
    Thread model: posix
    InstalledDir: /usr/bin

    $lsb_release -a
    Description:    Ubuntu 20.04.2 LTS
    Release:        20.04
>
**/

#include <stdio.h> // pentru functiile printf(), fprintf()
#include <stdlib.h> // pentru functia exit()
#include <getopt.h> // pentru functia getopt_long_only(), optarg
/*
    Functia getopt_long_only() permite operarea doar cu optiuni lungi care pot
    fi prefixate atat cu - cat si cu --. Mai jos va fi exemplificata utilizarea.
*/
int main(int argc, char *argv[])
{
    int i = 0;
    int optiune;
    printf("Numar argumente = %d\n", argc);
    /*
            Se defineste o structura de tipul option in care se vor defini 
        optiunile lungi si se vor mapa la un caracter. De exemplu:

        -adresa va fi mapat la 'a'
        -blocheaza va fi mapat la 'b'

            Se face acest lucru deoarece getopt_long_only() returneaza un int 
        care este codul ASCII al caracterului din optiune, iar optiunile lungi 
        trebuie mapate la un caracter pentru a stii ce valoare ASCII sa
        returneze.
    */
    static struct option long_options[] = {
        {"adresa",    required_argument, 0, 'a'},
        {"blocheaza", no_argument,       0, 'b'},
        {0,           0,                 0,  0 }
    };

    if (argc == 1)
    {   /* 
            Daca nu au fost introduse optiuni pe linia de comanda, va fi 
        generata o eroare si programul va inceta sa ruleze, iesind cu exit 
        code 1.
        */
        fprintf(stderr, "EROARE: Nu au fost furnizate optiuni pe linia "
                        "de comanda!\n"); 
        exit(1);
    }

    for (i = 0; i < argc; i++)
    {
        // Se vor afisa toate optiunile primite
        printf("\targv[%d] = %s\n", i, argv[i]);
    }
    // Se declara un index care va fi populat cu pozitia optiunii lungi citite
    int long_index = 0;
    // Cat timp exista argumente, se citesc de pe linia de comanda
    while ( (optiune = getopt_long_only(argc, argv, "a:b", long_options, 
            &long_index)) != -1)
    {
        if (optiune != '?')
        {
            printf("Optiunea returnata de getopt_long_only() este: %s.\n", 
                    long_options[long_index].name);
        }

        switch (optiune)
        {
        case 'a': // Primeste un sir de caractere dupa optiunea --adresa sau 
                  // -adresa
                {
                    printf("\tPrelucrez optiunea --%s si argumentul %s. \n",
                             long_options[long_index].name, optarg);
                    break;
                }
        case 'b': // Nu primeste un sir de caractere dupa optiunea -blocheaza
                  // sau --blocheaza
                {
                    printf("\tPrelucrez optiunea --%s. \n", 
                            long_options[long_index].name);
                    break;
                }
        case '?': // Optiune necunoscuta
                {
                    fprintf(stderr, "EROARE! Optiunea %c nu exista pentru "
                                    "programul %s!\n", optiune, argv[0]);
                    break;
                }
        }
    }
    return 0;
}
/*
1. Rulare cu toate argumentele corecte

$gcc t01l01_2.c -o t01l01_2                     (compilare/link-editare program)
$./t01l01_2 -adresa 127.0.0.1 --blocheaza                       (rulare program)
                                                              (afisare rezultat)
    Numar argumente = 4
        argv[0] = ./t01l01_2
        argv[1] = -adresa
        argv[2] = 127.0.0.1
        argv[3] = --blocheaza
    Optiunea returnata de getopt_long_only() este: adresa.
            Prelucrez optiunea --adresa si argumentul 127.0.0.1. 
    Optiunea returnata de getopt_long_only() este: blocheaza.
            Prelucrez optiunea --blocheaza.

2. Rulare fara argumente

$gcc t01l01_2.c -o t01l01_2                     (compilare/link-editare program)
$./t01l01_2                                                     (rulare program)
                                                              (afisare rezultat)
    Numar argumente = 1
    EROARE: Nu au fost furnizate optiuni pe linia de comanda!

3. Rulare cu argumente necunoscute

$gcc t01l01_2.c -o t01l01_3                     (compilare/link-editare program)
$./t01l01_2 -f                                              (rulare program)
                                                              (afisare rezultat)
    Numar argumente = 2
        argv[0] = ./t01l01_2
        argv[1] = -f
    ./t01l01_2: unrecognized option '-f'
    EROARE! Optiunea ? nu exista pentru programul ./t01l01_2!

$./t01l01_2 -fail
Numar argumente = 2
        argv[0] = ./t01l01_2
        argv[1] = -fail
./t01l01_2: unrecognized option '-fail'
EROARE! Optiunea ? nu exista pentru programul ./t01l01_2!
*/