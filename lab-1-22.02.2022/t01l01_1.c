/**
Lab. 01 Tema 1
Titlul temei:  Analizarea argumentelor introduse pe linia de comandă prin apel 
            functie getopt_long().
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
#include <getopt.h> // pentru functia getopt_long(), optarg

/*
    Functia getopt_long() permite operarea atat cu optiuni scurte, de tipul 
    -a -b ceva -c, cat si argumente lungi de tipul --aici --flag ceva. Mai jos 
    vor fi exemplificate ambele utilizari.
*/
int main(int argc, char *argv[])
{
    int i = 0;
    int optiune;
    printf("Numar argumente = %d\n", argc);
    /*
            Se defineste o structura de tipul option in care se vor defini 
        optiunile lungi si se vor mapa la un caracter. De exemplu:

        --adresa va fi mapat la 'a'
        --blocheaza va fi mapat la 'b'

            Se face acest lucru deoarece getopt_long() returneaza un int care 
        este codul ASCII al caracterului din optiune, iar optiunile lungi 
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
    while ( (optiune = getopt_long(argc, argv, "a:bcd:", long_options, 
            &long_index)) != -1)
    {
        if (optiune != '?')
        {
            printf("Optiunea returnata de getopt_long() este: %s.\n", 
                    long_options[long_index].name);
        }

        switch (optiune)
        {
        case 'c': // Nu primeste un sir de caractere dupa optiunea -c
                {
                    printf("\tPrelucrez optiunea -%s. \n", 
                            long_options[long_index].name);
                    break;
                }
        case 'd': // Primeste un sir de caractere dupa optiunea -d
                {
                    printf("\tPrelucrez optiunea -%s si argumentul %s. \n", 
                            long_options[long_index].name, optarg);
                    break;
                }
        case 'a': // Primeste un sir de caractere dupa optiunea --adresa
                {
                    printf("\tPrelucrez optiunea --%s si argumentul %s. \n",
                             long_options[long_index].name, optarg);
                    break;
                }
        case 'b': // Nu primeste un sir de caractere dupa optiunea 
                          // --blocheaza
                {
                    printf("\tPrelucrez optiunea --%s. \n", 
                            long_options[long_index].name);
                    break;
                }
        default: // Optiune necunoscuta
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

$gcc t01l01_1.c -o t01l01_1                     (compilare/link-editare program)
$./t01l01_1 --adresa 127.0.0.1 --blocheaza -c -d ceva           (rulare program)
                                                              (afisare rezultat)
    Numar argumente = 7
            argv[0] = ./t01l01_1
            argv[1] = --adresa
            argv[2] = 127.0.0.1
            argv[3] = --blocheaza
            argv[4] = -c
            argv[5] = -d
            argv[6] = ceva
    Optiunea returnata de getopt_long() este: a.
            Prelucrez optiunea --adresa si argumentul 127.0.0.1. 
    Optiunea returnata de getopt_long() este: b.
            Prelucrez optiunea --blocheaza. 
    Optiunea returnata de getopt_long() este: c.
            Prelucrez optiunea -c. 
    Optiunea returnata de getopt_long() este: d.
            Prelucrez optiunea -d si argumentul ceva.

2. Rulare fara argumente

$gcc t01l01_1.c -o t01l01_1                     (compilare/link-editare program)
$./t01l01_1                                                     (rulare program)
                                                              (afisare rezultat)
    Numar argumente = 1
    EROARE: Nu au fost furnizate optiuni pe linia de comanda!

3. Rulare cu argumente partiale

$gcc t01l01_1.c -o t01l01_1                     (compilare/link-editare program)
$./t01l01_1 --blocheaza -d ceva                                 (rulare program)
                                                              (afisare rezultat)

    Numar argumente = 4
            argv[0] = ./t01l01_1
            argv[1] = --blocheaza
            argv[2] = -d
            argv[3] = ceva
    Optiunea returnata de getopt_long() este: b.
            Prelucrez optiunea --blocheaza.
    Optiunea returnata de getopt_long() este: d.
            Prelucrez optiunea -d si argumentul ceva.

4. Rulare cu argumente necunoscute

$gcc t01l01_1.c -o t01l01_1                     (compilare/link-editare program)
$./t01l01_1 --help                                              (rulare program)
                                                              (afisare rezultat)
    Numar argumente = 2
            argv[0] = ./t01l01_1
            argv[1] = --help
    ./t01l01_1: unrecognized option '--help'
    Optiunea returnata de getopt_long() este: ?.
    EROARE! Optiunea ? nu exista pentru programul ./t01l01_1!
*/