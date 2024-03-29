/**
Lab. 05 Tema 5
Titlul temei:
Crearea unui TokenRing unidirectional intre  N procese in lant conform imaginilor atasate
P1->P2->P3->...etc...-Pn-1->Pn->P1

 Procesele realizeaza o comunicare tokenring prin intermediul unor buffer-e  de comunicatie anonime (pipes)
 Procesul P1 trimite prin pipe mesajul "Salut..." + PID proces catre P2.
                     ex. de mesaj emis  catre P2"Salut...>1234  unde 1234" este PID proces
 Procesul P2 trimite prin pipe mesajul receptionat  de la P2 la care adauga  ">"+ PID proces catre P3
                     ex. de mesaj emis  catre P3 "Salut...>1234>5678"  unde 5678 este PID proces
 Procesul P3 trimite prin pipe mesajul receptionat  

de la P2 la care adauga   ">"+ PID proces   catre P4
 ......
 Procesul Pn trimite prin pipe mesal  

de la Pn-1 la care adauga   ">"+ PID catre proces   P1  
//  Procesul P1 afiseaza mesalul receptionat de la Pn 

la care adauga   "___m-am intors" si  ringul este inchis (P1 se termina)
                     ex. mesaj receptionat  de catre P1 "Salut...>1234>5678......>54321___m-am intors"
 Fiecare proces creat afișează
   - identificatorul de proces  (Process ID) – PID
   - identificatorul procesului părinte (parent process ID) – PPID.
   - numarul procesului (1...N).
   - mesajul receptionat de la procesul anterior.
       Programul trebuie să conțină aşteptări asfel încât să nu apară procese orfan sau fenomene de deadlock.

Numarul de procese  pentru exemplificare este N=5  si este citit de pe linia de comandă printr-o funcție din familia getopt___(). Se face testul de existenta si numericitate pentru N  si se afiseaza N.
Procesele Pi se identifică prin PID și PPID (le afișează), afiseză I, care reprezintă al câtâlea proces creat este
si transmit prin pipe mesajul catre  Pi+1 
Procedura se repetă, P1-->P2, P2-->P3, P3-P4-->  P4-->P5, P5-->P1 care va afișa mesajul receptionat la care adaugă textul "__m-am intors", care atestă închiderea completă a ringului startat  în P1.
Programul trebuie să conțină aşteptări asfel încât să nu apară procese orfan sau fenomene de deadlock.

Data livrarii: 03.04.2022
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
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv) 
{
    char message[8000], final_message[8000];
    int n, fd[2], status, childpid, option;
    pid_t parent_pid = getpid();
    pid_t pid_msg;

    if (argc != 3) 
    {
        fprintf(stderr, "EROARE! Numar insuficient de argumente\n");
        exit(1);
    }
    option = getopt(argc, argv, "n:");
    if (option == -1)
    {
        fprintf(stderr, "EROARE! Numar insuficient de argumente\n");
        exit(1);
    }

    n = atoi(optarg);
    fprintf(stderr, "N = %d\n", n);
    fprintf(stderr, "Parintele: %d\n", parent_pid);

    // deschid fd
    // fprintf(stderr, "1\n");
    if (pipe(fd) == -1) 
    {
        fprintf(stderr, "EROARE! Nu s-a putut face pipe\n");
        fprintf(stderr, "pipe\n");
        exit(1);
    }
    // fprintf(stderr, "2\n");
    // Redirect de stdin in fd[0]
    if (dup2(fd[0], 0) == -1) 
    {
        fprintf(stderr, "EROARE! Nu s-a putut face dup2 pe stdin\n");
        fprintf(stderr, "dup2\n");
        exit(1);
    }
    // fprintf(stderr, "3\n");
    // Redirect de stdout in fd[1]
    if (dup2(fd[1], 1) == -1) 
    {
        fprintf(stderr, "EROARE! Nu s-a putut face dup2 pe stdout\n");
        fprintf(stderr, "dup2\n");
        exit(1);
    }
    // fprintf(stderr, "4\n");
    // fprintf(stderr, "6\n");
    for (int i = 0; i < n; i++)
    {
        fprintf(stderr, "For i = %d\n", i);
        childpid == fork();
        if (childpid == -1)
        {
            fprintf(stderr, "EROARE! Nu s-a putut face fokr()\n");
            exit(1);
        }
        else if (childpid == 0)
        {
            // copil
            fprintf(stderr, "child %d\n", getpid());
            if (i == 0)
            {
                // primul mesaj
                fprintf(stderr, "i = 0\n");
                write(fd[1], "Salut...", strlen("Salut..."));
                fprintf(stderr, "exit(0) child\n");
                
                fprintf(stderr, "FOR i = %d\n", i);
                exit(0);
            }
            else if (i == (n - 1))
            {
                // ultimul mesaj
                fprintf(stderr, "i = n\n");
                write(fd[1], "___m-am intors", strlen("___m-am intors"));
            }
            else
            {
                // i > 0 && i < n
                fprintf(stderr, "i e  0 - n\n");
                pid_t pid = getpid();
                write(fd[1], &pid, sizeof(pid_t));
                fprintf(stderr, "exit(0) child\n");
                exit(0);
            }
        }
        else
        {
            // parinte
            fprintf(stderr, "parinte\n");
            while (waitpid(childpid, &status, NULL) != -1) {};
            if (i == 0)
            {   
                // primul mesaj
                strcat(final_message, " > ");
                read(fd[0], &message, strlen("Salut..."));                
                strcat(final_message, message);
            }
            else if (i > 0 && i < n)
            {
                strcat(message, " > ");
                read(fd[0], &pid_msg, sizeof(int));                
                strcat(message, (char *) pid_msg);
            }
        }
        fprintf(stderr, "final de for\n");
        fprintf(stderr, "%s\n", final_message);
    }
    // inchid ambele fd
    if (close(fd[0]) == -1) 
    {
        fprintf(stderr, "EROARE! Nu s-a putut inchide fd[0]\n");
        exit(1);
    }
    if (close(fd[1]) == -1) 
    {
        fprintf(stderr, "EROARE! Nu s-a putut inchide fd[1]\n");
        exit(1);
    }
    // parinte ultimul mesaj
    while (waitpid(childpid, &status, NULL) != -1) {};
    strcat(final_message, message);
    strcat(final_message, (char *) parent_pid);
    strcat(final_message, " > ");
    read(fd[0], &message, strlen("___m-am intors"));
    strcat(final_message, message);
    fprintf(stderr, "%s", final_message);

    return 0;
}
/*

$gcc t05l05.c -o t05l05                         (compilare/link-editare program)
$./t05l05 -n 3                                                  (rulare program)
                                                              (afisare rezultat)
    N = 3
    Parintele: 8076
    For i = 0
    child 8076
    i = 0
    exit(0) child
    child 8077
    FOR i = 0
    i = 0
    exit(0) child
    FOR i = 0
*/