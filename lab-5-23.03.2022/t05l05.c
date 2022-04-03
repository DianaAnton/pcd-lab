/**
Lab. 04 Tema 4.1
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
#include <getopt.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv) 
{
    char message[8000], final_message[8000];
    int n, fd[2], status, childpid, option;
    pid_t parent_pid = getpid();
    pid_t pid_msg;

    if (argc != 3) {
        fprintf(stderr, "EROARE! Numar insuficient de argumente\n");
        exit(1);
    }
    option = getopt(argc, argv, "n:");
    if (option == -1)
    {
        fprintf(stderr, "EROARE! Numar insuficient de argumente\n");
        exit(1);
    }

    if (optarg == NULL)
    {
        fprintf(stderr, "EROARE! Optiunea nu are argument\n");
        exit(1);
    }
    n = atoi(optarg);
    printf("N = %d\n", n);
    printf("Parintele: %d\n", parent_pid);

    // // deschid fd
    // printf("1\n");
    // if (pipe(fd) == -1) {
    //     fprintf(stderr, "EROARE!\nErrno:%d\n%s\n", errno, strerror(errno));
    //     printf("pipe\n");
    //     exit(1);
    // }
    // printf("2\n");
    // // Redirect de stdin in fd[0]
    // if (dup2(fd[0], 0) == -1) {
    //     fprintf(stderr, "EROARE!\nErrno:%d\n%s\n", errno, strerror(errno));
    //     printf("dup2\n");
    //     exit(1);
    // }
    // printf("3\n");
    // // Redirect de stdout in fd[1]
    // // dup2(fd[1], 1);
    // if (dup2(fd[1], 1) == -1) {
    //     fprintf(stderr, "EROARE!\nErrno:%d\n%s\n", errno, strerror(errno));
    //     printf("dup2\n");
    //     exit(1);
    // }
    // printf("4\n");
    // // inchid ambele fd
    // if (close(fd[0]) == -1) {
    //     fprintf(stderr, "EROARE!\nErrno:%d\n%s\n", errno, strerror(errno));
    //     printf("close\n");
    //     exit(1);
    // }
    // printf("5\n");
    // if (close(fd[1]) == -1) {
    //     fprintf(stderr, "EROARE!\nErrno:%d\n%s\n", errno, strerror(errno));
    //     printf("dup2\n");
    //     exit(1);
    // }
    // printf("6\n");
    if (pipe(fd) == -1) {
        fprintf(stderr, "Error at pipeing!\nErrno:%d\nReason:%s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* redirect the read-end of the pipe to STDIN. */
    if (dup2(fd[0], 0) < 0) {
        fprintf(stderr, "Error at redirecting read end of pipe to STDIN!\nErrno:%d\nReason:%s\n", errno,
                strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* redirect the write-end of the pipe to STDOUT. */
    if (dup2(fd[1], 1) < 0) {
        fprintf(stderr, "Error at redirecting write end of pipe to STDOUT!\nErrno:%d\nReason:%s\n", errno,
                strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* for now, close the read-end of the pipe. */
    
        if (close(fd[0]) == -1) {
            fprintf(stderr, "Error at closing pipe read-end!\nErrno:%d\nReason:%s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
    /* also, close it's write-end. */
    if (close(fd[1]) == -1) {
        fprintf(stderr, "Error at closing pipe write-end!\nErrno:%d\nReason:%s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
    {
        printf("For i = %d", i);
        // deschid fd
        if (pipe(fd) == -1) {
            fprintf(stderr, "EROARE!\nErrno:%d\n%s\n", errno, strerror(errno));
            exit(1);
        }
        childpid == fork();
        if (childpid == -1)
        {
            fprintf(stderr, "EROARE! Nu s-a putut face fokr()\n");
            exit(1);
        }
        else if (childpid == 0)
        {
            // copil
            // Redirect de stdout in fd[1]
            if (dup2(fd[1], 1) == -1) {
                fprintf(stderr, "EROARE!\nErrno:%d\n%s\n", errno, strerror(errno));
                exit(1);
            }
            if (i == 0)
            {
                // primul mesaj
                write(fd[1], "Salut...", strlen("Salut..."));
            }
            else if (i == n)
            {
                // ultimul mesaj
                write(fd[1], "___m-am intors", strlen("___m-am intors"));
                break;
            }
            else
            {
                // i > 0 && i < n
                pid_t pid = getpid();
                write(fd[1], &pid, sizeof(pid_t));
            }
            exit(0);
        }
        else
        {
            // parinte
            while (wait(&status) > 0) {};
            // Redirect de stdin in fd[0]
            if (dup2(fd[0], 0) == -1) {
                fprintf(stderr, "EROARE!\nErrno:%d\n%s\n", errno, strerror(errno));
                exit(1);
            }
            if (i == 0)
            {
                read(fd[0], &message, strlen("Salut..."));
                strcat(final_message, ">");
                strcat(final_message, message);

            }
            else if (i > 0 && i < n)
            {
                read(fd[0], &pid_msg, sizeof(int));
                strcat(message, ">");
                strcat(message, (char *) pid_msg);
            }
        }
        if (i != (n - 1) )
        {
            // inchid ambele fd
            if (close(fd[0]) == -1) {
                fprintf(stderr, "EROARE!\nErrno:%d\n%s\n", errno, strerror(errno));
                exit(1);
            }
            if (close(fd[1]) == -1) {
                fprintf(stderr, "EROARE!\nErrno:%d\n%s\n", errno, strerror(errno));
                exit(1);
            }
        }
        if (childpid)
        {
            break;
        }
        fprintf(stderr, "%s\n", final_message);
    }

    // parinte
    while (wait(&status) > 0) {};
    strcat(final_message, message);
    read(fd[0], &message, strlen("___m-am intors"));
    strcat(final_message, (char *) parent_pid);
    strcat(final_message, ">");
    strcat(final_message, message);
    fprintf(stderr, "%s", final_message);

    return 0;
}