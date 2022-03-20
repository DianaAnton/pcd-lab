/**
Lab. 03 Tema 3.3
Titlul temei:
    Prin modificarea corespunzătoare a sursei aplicațiilor demonstrative din L03
să se:
    3. realizeze un program de utilizare a funcției execv() pentru executarea 
de comenzi shell.

Data livrarii: 20.03.2022
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

#include <stdio.h> // pentru functia fprintf()
#include <stdlib.h> // pentru functia exit()
#include <unistd.h> // pentru execv()
#include <string.h> // pentru strcat()

/*
    Exemplificare apelarea comenzii ps cu zero sau o optiune.
*/

int main(int argc, char *argv[])
{
    /*
        Se verifica daca a fost furnizat numarul corect de argumente pe linia
    de comanda. Daca e prea mare sau mic, se va afisa o eroare, iar programul va
    inceta sa ruleze.
    */
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "EROARE: Numar incorect de argumente furnizate!\n");
        exit(1);
    }

    if (argc == 2)
    {
        /*
            Daca a fost furnizata o comanda fara optiuni, se va apela aceasta.
        */
        char *cmd[] = { argv[1], NULL };
        
        // Formez calea catre fisierul ce va fi apelat pentru comanda transmisa
        char path[20] = "/bin/";
        strcat(path, argv[1]);
        
        if (execv(path, cmd) == -1)
        {
            /*
                Daca returneaza -1, inseamna ca a fost o eroare. Se va afisa un 
            mesaj la stderr si programul se va opri.
            */
            fprintf(stderr, "EROARE: Nu s-a putut executa comanda %s!\n", 
                    argv[1]);
            exit(1);
        }
    }
    else if (argc == 3)
    {
        /*
            Daca a fost furnizata o comanda cu o optiune, se va apela aceasta 
        impreuna cu optiunea dorita.
        */
        char *cmd[] = { argv[1], argv[2], NULL };
        
        // Formez calea catre fisierul ce va fi apelat pentru comanda transmisa
        char path[20] = "/bin/";
        strcat(path, argv[1]);
        
        if (execv(path, cmd) == -1)
        {
            /*
                Daca returneaza -1, inseamna ca a fost o eroare. Se va afisa un 
            mesaj la stderr si programul se va opri.
            */
            fprintf(stderr, "EROARE: Nu s-a putut executa comanda %s %s!\n", 
                argv[1], argv[2]);
            exit(1);
        }
    }
    return 0;    
}
/*
1. Executare comanda echo $HOME

    $gcc t03l03_3.c -o t03l03_3                 (compilare/link-editare program)
    $./t03l03_3 echo $HOME                                      (rulare program)
                                                              (afisare rezultat)

    /home/danton

2. Executare comanda ls

    $gcc t03l03_3.c -o t03l03_3                 (compilare/link-editare program)
    $./t03l03_3 ls                                              (rulare program)
                                                              (afisare rezultat)
    $./t03l03_3 ls
    t03l03_1     t03l03_2     t03l03_3     t03l03_4.c     PCD-UNIX-L03_exec.pdf 
    t03l03_1.c   t03l03_2.c   t03l03_3.c

3. Apelare cu numar invalid de argumente

    $gcc t03l03_3.c -o t03l03_3                 (compilare/link-editare program)
    $./t03l03_3                                                 (rulare program)
                                                              (afisare rezultat)
    EROARE: Numar incorect de argumente furnizate!                                                            
*/