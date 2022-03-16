/** Fisier: gopt.c **/ 
/* Functia getopt() pentru a analiza mai multe optiuni si argumente de pe linia de comanda */ 
#include <stdio.h> /* pentru functiile printf(), fprintf() */ 
#include <stdlib.h> /* pentru functia exit() */ 
#include <unistd.h> /* pentru functia getopt(), optarg */ 
int main(int argc, char *argv[]) { 
    int optiune; int i;
    printf("argc = %d\n", argc);
    if (argc == 1) { 
        // Un prim control al linei de comanda. 
        fprintf(stderr, "Eroare: lipsa optiuni pe linia de comanda\n"); 
        exit(1); 
    } 
    /* Afisare argv */ 
    for (i=0; i<argc; i++) 
        printf("arg[%d] = \"%s\"\n", i, argv[i]);

    /* Interpretare optiuni argv */
    while((optiune = getopt(argc, argv, "ab:cd:")) != -1) 
    // un al doilea control al linei de comanda 
    // a = optiune -a , c = optiune -c 
    // b: = optiune -b urmata de un argument pozitionat in optarg 
    // d: = optiune -d urmata de un argument pozitionat in optarg 
    { 
        printf("Optiuniunea citita de pe linia de comanda este: %c \n", optiune); 
        switch(optiune) { case 'a' : // NU primeste un sir de caractere dupa optiunea -a //se executa ceva printf("\tPrelucrez optiune %c \n", optiune); break; case 'b' : // primeste un sir de caractere dupa optiunea -b //se executa ceva printf("\tPrelucrez optiune %c si argumentul %s \n", optiune, optarg); break; case 'c' : // NU primeste un sir de caractere dupa optiunea -c //se executa ceva printf("\tPrelucrez optiune %c \n", optiune); break; case 'd' : // primeste un sir de caractere dupa optiunea -d //se executa ceva printf("\tPrelucrez optiune %c si argumentul %s \n", optiune, optarg); break; case '?' : // optiune nerecunoscuta fprintf(stderr, "Utilizare: %s cu optiuni sau argumente eronate\n", argv[0]); exit(EXIT_FAILURE); } } exit(0); }