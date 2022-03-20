/**
Lab. 03 Tema 3.2
Titlul temei:
    Prin modificarea corespunzătoare a sursei aplicațiilor demonstrative din L03
să se:
    2. realizeze un program de utilizare a funcției execle() pentru executarea 
de comenzi shell

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
#include <unistd.h> // pentru execle()
#include <string.h> // pentru strcat()

/*
    Exemplificare apelarea comenzii ps cu zero sau o optiune.
*/

int main(int argc, char *argv[])
{
    char *env[] = { "HOME=/home/diana", "LOGNAME=diana", "PWD=/danton/home/lab-3-09.03.2022", NULL };
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

        // Formez calea catre fisierul ce va fi apelat pentru comanda transmisa      
        char path[20] = "/bin/";
        strcat(path, argv[1]);
    
        if (execle(path, argv[1], NULL, env) == -1)
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
        
        // Formez calea catre fisierul ce va fi apelat pentru comanda transmisa
        char path[20] = "/bin/";
        strcat(path, argv[1]);
        
        if (execle(path, argv[1], argv[2], NULL, env) == -1)
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
1. Executare comanda ps -faux

    $gcc t03l03_2.c -o t03l03_2                 (compilare/link-editare program)
    $./t03l03_2 ps -faux                                        (rulare program)
                                                              (afisare rezultat)

    $./t03l03_2 ps -faux

    USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
    root         1  0.0  0.0    904   504 ?        Sl   Mar16   0:00 /init
    root         8  0.0  0.0    904    80 ?        Ss   Mar16   0:00 /init
    root         9  0.0  0.0    904    84 ?        S    Mar16   0:00  \_ /init
    danton      10  0.0  0.0  10716  5908 pts/0    Ss+  Mar16   0:00      \_ -bash
    message+    89  0.0  0.0   7376  3496 ?        Ss   Mar16   0:00      \_ /usr/bin/dbus-daemo
    danton    4239  0.0  0.3 498172 42824 pts/0    Sl   Mar18   0:00      \_ /usr/bin/python3 /u
    root      4386  0.0  0.0    912    88 ?        Ss   15:21   0:00 /init

2. Executare comanda ps

    $gcc t03l03_2.c -o t03l03_2                 (compilare/link-editare program)
    $./t03l03_2 ps                                              (rulare program)
                                                              (afisare rezultat)
    PID TTY          TIME CMD
    4800 pts/4    00:00:00 bash
    6217 pts/4    00:00:00 ps

3. Apelare cu numar invalid de argumente

    $gcc t03l03_2.c -o t03l03_2                 (compilare/link-editare program)
    $./t03l03_2                                                 (rulare program)
                                                              (afisare rezultat)
    EROARE: Numar incorect de argumente furnizate!                                                            
*/