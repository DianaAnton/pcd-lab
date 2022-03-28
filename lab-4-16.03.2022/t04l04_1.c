/**
Lab. 04 Tema 4.1
Titlul temei:
    1. Realizarea unui program de utilizare a tehnici IPC de sincronizare prin 
semafoare.  /  program t04l04_1.c
    Sfaturi de urmat:  
Implementarea este "de voie", fara obligativitati de urmat

Data livrarii: 27.03.2022
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
#include <unistd.h> // pentru execlp()
#include <pthread.h> // pthread_t, pthread_create(), pthread_join()
#include <semaphore.h> // sem_t, sem_wait(), sem_post(), sem_destroy()

// Declaram o variabila semafor si un contor
sem_t semaphore;
int variable = 0;

void *print_status(void *args)
{   
    // Blocheaza semaforul
    if (sem_wait(&semaphore) == -1)
    {
        fprintf(stderr, "ERROR! Nu s-a putut bloca semaforul!");
        exit(1);
    }

    // Afiseaza un mesaj
    printf("Eu sunt thread-ul cu numarul %d.\n", variable);
    variable++;

    // Deblocheaza semaforul
    if (sem_post(&semaphore) == -1)
    {
        fprintf(stderr, "ERROR! Nu s-a putut debloca semaforul!");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    /*
        Se initializeaza semaforul, setand pe 0 al doilea argument pentru a 
    specifica ca va fi folosit intre firele de executie al unui proces si al 
    treilea argument e setat pe 1 pentru a specifica valoarea initala.
    */
    if (sem_init(&semaphore, 0, 1) == -1)
    {
        fprintf(stderr, "ERROR! Nu s-a putut initializa semaforul!");
        exit(1);
    }

    // Declaram threadurile ce vor fi folosite.
    pthread_t th1, th2, th3, th4, th5;

    /*
        Cream fiecare thread in parte, primind un pointer la functia declarata 
    mai sus fara argumente si neavand atribute setate.
    */
    if (pthread_create(&th1, NULL, print_status, NULL) != 0)
    {
        fprintf(stderr, "ERROR! Nu s-a putut crea threadul 1!");
        exit(1);
    }
    if (pthread_create(&th2, NULL, print_status, NULL) != 0)
    {
        fprintf(stderr, "ERROR! Nu s-a putut crea threadul 2!");
        exit(1);
    }
    if (pthread_create(&th3, NULL, print_status, NULL) != 0)
    {
        fprintf(stderr, "ERROR! Nu s-a putut crea threadul 3!");
        exit(1);
    }
    if (pthread_create(&th4, NULL, print_status, NULL) != 0)
    {
        fprintf(stderr, "ERROR! Nu s-a putut crea threadul 4!");
        exit(1);
    }
    if (pthread_create(&th5, NULL, print_status, NULL) != 0)
    {
        fprintf(stderr, "ERROR! Nu s-a putut crea threadul 5!");
        exit(1);
    }
    
    // Pornesc fiecare thread.
    if (pthread_join(th1, NULL) != 0)
    {
        fprintf(stderr, "ERROR! Threadul 1 nu a putut da join!");
        exit(1);
    }
    if (pthread_join(th2, NULL) != 0)
    {
        fprintf(stderr, "ERROR! Threadul 2 nu a putut da join!");
        exit(1);
    }if (pthread_join(th3, NULL) != 0)
    {
        fprintf(stderr, "ERROR! Threadul 3 nu a putut da join!");
        exit(1);
    }if (pthread_join(th4, NULL) != 0)
    {
        fprintf(stderr, "ERROR! Threadul 4 nu a putut da join!");
        exit(1);
    }if (pthread_join(th5, NULL) != 0)
    {
        fprintf(stderr, "ERROR! Threadul 5 nu a putut da join!");
        exit(1);
    }

    // Distrug semaforul
    if (sem_destroy(&semaphore) == -1)
    {
        fprintf(stderr, "ERROR! Semaforul nu a putut fi distrus!");
        exit(1);
    }    
    return 0;    
}
/*

$gcc t04l04_1.c -o t04l04_1 -lpthread           (compilare/link-editare program)
$./t04l04_1                                                     (rulare program)
                                                              (afisare rezultat)
Eu sunt thread-ul cu numarul 0.
Eu sunt thread-ul cu numarul 1.
Eu sunt thread-ul cu numarul 2.
Eu sunt thread-ul cu numarul 3.
Eu sunt thread-ul cu numarul 4.                                     
*/