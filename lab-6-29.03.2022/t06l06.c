/**
Lab. 06 Tema 6
Titlul temei:
Se va livra programul t06l06,c
    1. Realizarea unui token-ring unidirecțional de N procese în lanț prin pipe-uri 
cu nume. N este citit de pe linia de comandă printr-o funcție din familia getopt___(). 
Procesul P1 se identifică prin PID și PPID (le afișează), afișează mesajul ”Salut”, pe 
care îl transmite prin pipe cu nume procesului P2, P2 se identifică prin PID și PPID 
(le afișează), recepționeză prin pipe cu nume mesajul de la P1, afișează mesajul
 recepționat și îl transmite prin pipe procesului P3, Procedura se repetă, 
 P3-->P4, P4-->P5, ...Pn-1-->Pn, Pn-->P1. P1 va afișa mesajul receptionat de la 
 Pn la care adaugă textul "__m-am intors", care atestă închiderea completă 
 a ringului în P1.

    Testarea se va face pentru N=5 și se va crea următorul token-ring, 
conectarea făcându-se prin pipe cu nume conform figurilor alăturate:

Indicații de realizare:
    Pentru crearea proceselor se va utiliza un ciclu de N iterații. Se va 
utiliza redirectarea intrării / ieșirii standard către pipe-ul cu nume. 
Toate afișările se vor face pe fișierul standard de eroare (stderr), prin apel 
funcție fprintf(stderr,...). Programul trebuie să conțină aşteptări asfel 
încât să nu apară procese orfan sau fenomene de deadlock. În principal se vor 
utiliza apelurile: getopt()- pentru citirea lui N, fprintf(stderr,...), 
mkfifo(), fork(), dup2(), open(), read(), write(), wait()/waitpid(), getpid() 
și getppid() (posibil și alte funcții de I/O low-level, ex. prinf(), gets(), etc).

Data livrarii: 10.04.2022
Student: Anton Diana-Ana-Maria
An 3 IA
----------
<
Programul a fost rulat sub Kali Linux in VirtualBox cu:
    $gcc --version
    gcc (Debian 11.2.0-13) 11.2.0

    $lsb_release -a
    Distributor ID: Kali
    Description:    Kali GNU/Linux Rolling
    Release:        2021.4
    Codename:       kali-rolling

    Din cauza modului in care e montat WSL-ul in windows, nu am putut 
folosi mkfifo si am rulat pe o distributie de Kali pe care o aveam setata
in VirtualBox.
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
#include <sys/types.h>
#include <sys/stat.h>

#define MAX 9000

int main(int argc, char **argv) 
{
    char message[MAX], final_message[MAX];
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

    // rw-r--r--
    mode_t fifo_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int fd_0, fd_1;

    // creez fisier de pipe tip fifo
    if (mkfifo("pipe_cu_nume", fifo_perms) == -1)
    {
        fprintf(stderr, "EROARE! Nu pot crea pipe_cu_nume\n %d: %s", errno, 
        strerror(errno));
        exit(1);
    }
    // deschid fisier pt scriere
    fd_0 = open("pipe_cu_nume", O_RDONLY | O_NONBLOCK);
    // deschid fisier pt scriere
    fd_1 = open("pipe_cu_nume", O_WRONLY);
    // write something
    //write(fd_1, "message", 30);
    // - citeste date din fișier FIFO (pipe cu nume)
    //read (fd_0, buffer, LMSG);

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
            { // primul mesa
                if (dup2(fd_1, 1) == -1)
                {
                    fprintf(stderr, "EROARE! Nu s-a putut redirecta STDOUT la fd_1\n");
                    exit(1);
                }
                close(fd_0);
                fprintf(stderr, "i = 0\n");
                
                char mess[MAX] = "";
                char tp[100] = "";
                pid_t pid = getpid(), ppid = getppid();
                strcat(mess, "Salut...");
                fprintf(stderr, "Mess %s\n", mess);
                strcat(mess, "PID = ");                
                fprintf(stderr, "Mess %s\n", mess);
                char c_pid = pid;
                strcat(mess, c_pid);
                fprintf(stderr, "Mess %s\n", mess);
                strcat(mess, " PPID = ");
                fprintf(stderr, "Mess %s\n", mess);
                strcat(mess, (char) ppid);
                fprintf(stderr, "Mess %s\n", mess);

                write(fd_1, mess, strlen(mess));
                close(fd_1);

                fprintf(stderr, "exit(0) child\n");
                fprintf(stderr, "FOR i = %d\n", i);
                exit(0);
            }
            else if (i == (n - 1))
            {
                if (dup2(fd_1, 1) == -1)
                {
                    fprintf(stderr, "EROARE! Nu s-a putut redirecta STDOUT la fd_1\n");
                    exit(1);
                }
                if (dup2(fd_0, 0) == -1)
                {
                    fprintf(stderr, "EROARE! Nu s-a putut redirecta STDIN la fd_0\n");
                    exit(1);
                }
                // ultimul mesaj
                fprintf(stderr, "i = n\n");
                char mess[MAX];
                read(fd_0, &mess, MAX);
                strcat(mess, "___m-am intors\n");
                write(fd_1, mess, strlen(mess));
                close(fd_0); close(fd_1);
                break;
            }
            else
            { // i > 0 && i < n
                fprintf(stderr, "i e  0 - n\n");
                if (dup2(fd_1, 1) == -1)
                {
                    fprintf(stderr, "EROARE! Nu s-a putut redirecta STDOUT la fd_1\n");
                    exit(1);
                }
                if (dup2(fd_0, 0) == -1)
                {
                    fprintf(stderr, "EROARE! Nu s-a putut redirecta STDIN la fd_0\n");
                    exit(1);
                }
                
                char mess[MAX];
                pid_t pid = getpid(), ppid = getppid();
                read(fd_0, &mess, MAX);
                strcat(mess, "PID = ");
                strcat(mess, pid);
                strcat(mess, "PPID = ");
                strcat(mess, ppid);
                strcat(mess, "\n");
                write(fd_1, mess, strlen(mess));
                close(fd_0); close(fd_1);
                
                // write(fd[1], &pid, sizeof(pid_t));
                fprintf(stderr, "exit(0) child\n");
                exit(0);
            }
        }
        else
        {
            // parinte
            fprintf(stderr, "parinte\n");
            // for (int j = 0; j < n; j++)
            // {
            //     wait(&status);
            // }
            while (waitpid(childpid, &status, NULL) != -1) {};
            // if (i == 0)
            // {   
            //     // primul mesaj
            //     strcat(final_message, " > ");
            //     read(fd[0], &message, strlen("Salut..."));                
            //     strcat(final_message, message);
            // }
            // else if (i > 0 && i < n)
            // {
            //     strcat(message, " > ");
            read(fd_0, &message, MAX);
            strcat(final_message, message);
            //     strcat(message, (char *) pid_msg);
            // }
        }
        fprintf(stderr, "final de for\n");
        // fprintf(stderr, "%s\n", final_message);
    }
    // inchid ambele fd
    // if (close(fd[0]) == -1) 
    // {
    //     fprintf(stderr, "EROARE! Nu s-a putut inchide fd[0]\n");
    //     exit(1);
    // }
    // if (close(fd[1]) == -1) 
    // {
    //     fprintf(stderr, "EROARE! Nu s-a putut inchide fd[1]\n");
    //     exit(1);
    // }
    // parinte ultimul mesaj
    // while (waitpid(childpid, &status, NULL) != -1) {};
    // strcat(final_message, message);
    // strcat(final_message, (char *) parent_pid);
    // strcat(final_message, " > ");
    // read(fd[0], &message, strlen("___m-am intors"));
    // strcat(final_message, message);
    // fprintf(stderr, "%s", final_message);

    if (dup2(fd_0, 0) == -1)
    {
        fprintf(stderr, "EROARE! Nu s-a putut redirecta STDIN la fd_0\n");
        exit(1);
    }
    close(fd_0); close(fd_1);            
    read(fd_0, &message, MAX);
    strcat(final_message, message);
    fprintf(stderr, "Text:\n %s", final_message);

    return 0;
}
/*

$gcc-11 t06l06.c -o t06l06                      (compilare/link-editare program)
$./t06l06 -n 5                                                  (rulare program)
                                                              (afisare rezultat)
                                  
    N = 5
    Parintele: 10205
    For i = 0
    child 10205
    i = 0
    Mess Salut...
    Mess Salut...PID = 
    child 10206
    i = 0
    Mess Salut...
    Mess Salut...PID = 
    zsh: segmentation fault  ./t06l06 -n 5
*/