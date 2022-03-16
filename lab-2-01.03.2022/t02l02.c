/**
Lab. 02 Tema 2
Titlul temei:  
    Să se scrie un program t02l02.c care prin mecanismul de fork crează o 
structură ierarhizată de procese de 2 nivele fiecare nivel avand N , respectiv 
M procese în pieptene in conformitate cu figura atasata. N și M vor fi date pe 
linia de comandă. Programul va controla dacă pe linia de comandă au fost date 
corect N și M și în caz de eroare va afișa un mesaj de eroare și se va opri. 
Structura ierarhizată de procese va fi pusă în evidență prin mesaje afișate pe 
stderr (fișier standard de eroare).

    Specificatii de realizare: se vor utiliza  apelul fork(), apelul 
wait(0)/waitpid(), apelul getpid(),  apelul getppid() și două cicluri for 
pentru N și M. Se va testa existenta pe linia de comanda la lui M si N si in 
caz ca exista daca ele sunt numerice.

Data livrarii: 13.03.2022
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
#include <ctype.h> // pentru isdigit()
#include <sys/types.h> // pentru pid_t
#include <unistd.h> // pentru fork()
#include <sys/wait.h> // pentru waitpid()


int main(int argc, char *argv[])
{   
    /*
        Se verifica daca a fost furnizat numarul corect de argumente pe linia
    de comanda. Daca e prea mare sau mic, se va afisa o eroare, iar programul va
    inceta sa ruleze.
    */
    if (argc < 3 || argc > 3)
    {
        fprintf(stderr, "EROARE: Numar incorect de argumente furnizate!\n");
        exit(1);
    }

    /*
        Convertesc argumentele primite din char * in int.
    */
    int n =  atoi(argv[1]), m = atoi(argv[2]), status;

    if (isdigit(*argv[1]) == 0)
    {
        /*
            Verific daca primul argument este un numar. Daca nu, programul
        afiseaza o eroare si va incheia executia.
        */
        fprintf(stderr, "EROARE: Valoarea pentru n nu e un numar intreg!\n");
        exit(1);
    }
    else if (n < 0)
    {
        /*
            Daca primul argument este un numar atunci verific daca este un 
        numar pozitiv. Daca nuprogramul afiseaza o eroare si va incheia 
        executia.
        */
        fprintf(stderr, "EROARE: Valoarea pentru n nu e un numar intreg "
                "pozitiv!\n");
        exit(1);
    }

    if (isdigit(*argv[2]) == 0)
    {
        /*
            Verific daca al doilea argument este un numar. Daca nu, programul
        afiseaza o eroare si va incheia executia.
        */
        fprintf(stderr, "EROARE: Valoarea pentru m nu e un numar intreg!\n");
        exit(1);
    }
    else if (m < 0)
    {
        /*
            Daca al doilea argument este un numar atunci verific daca este un 
        numar pozitiv. Daca nu, programul afiseaza o eroare si va incheia 
        executia.
        */
        fprintf(stderr, "EROARE: Valoarea pentru m nu e un numar intreg "
                "pozitiv!\n");
        exit(1);
    }
    
    fprintf(stderr, "n = %d\nm = %d\n", n, m);
    pid_t childpid1, childpid2;

    /*
        Incep crearea primului nivel de procese, cel care are n procese.
    */
    for (int i = 0; i < n; i++)
    {   
        // creez un proces nou
        childpid1 = fork();
        if (childpid1 == 0)
        {
            // childpid1 = 0 -> suntem in copil
            fprintf(stderr, "Copilul %d pe nivelul 1.    PID = %d  PPID = %d\n", 
                    i + 1, getpid(), getppid());

            // incep sa creez celelalte procese din copil pe nivelul m
            for (int j = 0; j < m; j++)
            {
                // creez un proces nou
                childpid2 = fork();
                if (childpid2 == 0)
                {
                    // childpid2 = 0 -> suntem in copil
                    fprintf(stderr, "Copilul %d pe nivelul 2.    PID = %d"  
                            "  PPID = %d\n", i + 1, getpid(), getppid());
                    // iesim din copil
                    break;
                }
                else if (childpid2 > 0)
                {
                    // childpid2 > 0 -> suntem in parinte
                    for (int k = 0; k < m; k++)
                    {   
                        // asteptam timp de m iteratii
                        wait(&status);
                    }
                }
                else if (childpid2 == -1)
                {   
                    // childpid2 = -1 -> eroare la fork()
                    fprintf(stderr, "EROARE: fork() a dat o eroare!\n");
                    exit(1);
                }
            }
            // am terminat cu crearea de procese, deci iesim din copil
            break;
        }
        else if (childpid1 > 0)
        {
            // childpid1 > 0 -> suntem in parinte            
            for (int k = 0; k < m; k++)
            {
                // asteptam timp de m iteratii
                wait(&status);
            }
            
        }
        else if (childpid1 == -1)
        {
            // childpid2 = -1 -> eroare la fork()
            fprintf(stderr, "EROARE: fork() a dat o eroare!\n");
            exit(1);
        }        
    }
}

/*
Specificatii de testare  (rezulta mai multe output-uri care trebuiesc livrate):

    1. Pe caz de eroare: M sau/si N neintrodus pe linia de comanda si  N si/sau
M nenumeric.

a) N/M lipsa
    $gcc t02l02.c -o t02l02                     (compilare/link-editare program)
    $./t02l02 2                                                 (rulare program)
                                                              (afisare rezultat)
    EROARE: Numar incorect de argumente furnizate!

    $gcc t02l02.c -o t02l02                     (compilare/link-editare program)
    $./t02l02                                                   (rulare program)
                                                              (afisare rezultat)
    EROARE: Numar incorect de argumente furnizate!

b) N/M nenumeric
    $gcc t02l02.c -o t02l02                     (compilare/link-editare program)
    $./t02l02 2 a                                               (rulare program)
                                                              (afisare rezultat)
    EROARE: Valoarea pentru m nu e un numar intreg!

    $gcc t02l02.c -o t02l02                     (compilare/link-editare program)
    $./t02l02 a a                                               (rulare program)
                                                              (afisare rezultat)
    EROARE: Valoarea pentru n nu e un numar intreg!

    2. Pe caz corect pentru N=2 și M=3 se va crea o ierarhie de procese conform 
figurii atasate, ierarhie ce va fi pusa in evidenta prin mesaje corespunzatoare.

    $gcc t02l02.c -o t02l02                     (compilare/link-editare program)
    $./t02l02 2 3                                               (rulare program)
                                                              (afisare rezultat)
    n = 2
    m = 3
    Copilul 1 pe nivelul 1.    PID = 10563  PPID = 10562
    Copilul 1 pe nivelul 2.    PID = 10564  PPID = 10563
    Copilul 1 pe nivelul 2.    PID = 10565  PPID = 10563
    Copilul 1 pe nivelul 2.    PID = 10566  PPID = 10563
    Copilul 2 pe nivelul 1.    PID = 10567  PPID = 10562
    Copilul 2 pe nivelul 2.    PID = 10568  PPID = 10567
    Copilul 2 pe nivelul 2.    PID = 10569  PPID = 10567
    Copilul 2 pe nivelul 2.    PID = 10570  PPID = 10567
*/