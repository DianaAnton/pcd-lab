/**
Lab. 07 Tema 7
Titlul temei:
    1. Să se scrie un program, care conform figurii atașate (a se studia figura 
pentru a constata modul de rezolvare al temei), prin mecanismul de gestiue a 
semnalelor realizează comunicarea dintre un proces părinte (P1) și un proces 
copil (P2) pe baza a două semnale SIGUSR2 și respectiv SIGINT. Procesul copil 
va emite un semnal SIGUSR2 către procesul părinte. La rândul său procesul 
părinte va trimite semnalul SIGINT către procesul copil. Tratarea semnalului 
SIGUSR2 se face într-un handler de semnal definit în procesul părinte, iar 
tratarea semnalului SIGINT este lăsată în seama sistemului de operare.

Indicații de realizare:
    În principal se vor utiliza apelurile: fork(), getpid(), getppid(), 
sigsuspend(), sigprocmask, sigfillset(), sigaction(), kill(), wait/waitpid(). 
Pentru ieșirile pe ecran se va utiliza apelul: fprintf(stderr,...) - ieșire pe
fișierul standard de eroare.

Data livrarii: 17.04.2022
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

#include <unistd.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> // pt. exit() 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <errno.h> 
int main() 
{ 
    pid_t pid; // memoreaza PID copil 
    sigset_t sigset, sigoldmask;

    if ((pid = fork()) == 0) 
    { /* cod copil */ 
        struct sigaction actiune_SIGUSR1; 
        int TratareSemnalSIGUSR1();

        struct sigaction actiune_SIGINT; 
        int TratareSemnalSIGINT(); 
        
        /* salvare masca semnal curenta in sigoldmask */ 
        sigprocmask(SIG_SETMASK, NULL, &sigoldmask); // probabil in sigildmask SIGINT nu e blocat 
        /* setare masca semnal curenta in sigset */ 
        sigfillset(&sigset); // introduc in sigset toate semnalele 
        sigprocmask(SIG_SETMASK, &sigset, NULL); 
        fprintf(stderr, "C: Intru in zona critica - blochez toate semnalele "
                        "inclusiv SIGINT si SIGUSR1 \n"); 

        /* ++++++++++++ Zona critica se blocheaza toate semnalele ++++++++++++ */ 

        /* 
            in aceasta zona Parintele nu trebuie sa emita semnale. Acum 
        Parintele sta suspendat prin apel in pause() pentru ca Copilul nu e 
        pregatit sa primeasca semnale de la el 
        */ 
        sigprocmask(SIG_BLOCK, &sigset, NULL); // se blocheaza toate semnalele,

        /* in copil se creaza signal handler*/ 
        //SIGUSR1 
        actiune_SIGUSR1.sa_flags = 0; 
        /*set noul handler semnal SIGUSR1*/; 
        actiune_SIGUSR1.sa_handler = (void (*))(int)TratareSemnalSIGUSR1;

        if (sigaction(SIGUSR1, &actiune_SIGUSR1, NULL) == -1) // instalez actiune SIGUR1
        { 
            perror("Eroare: sigaction"); 
            exit(1); 
        }

        //SIGINT 
        actiune_SIGINT.sa_flags = 0; 
        /*set noul handler semnal SIGINT*/
        actiune_SIGINT.sa_handler = (void (*))(int)TratareSemnalSIGINT; 
        if (sigaction(SIGINT, &actiune_SIGINT, NULL) == -1) // instalez actiune SIGINT
        {
            perror("Eroare: sigaction"); 
            exit(1); 
        }

        fprintf(stderr, "C: deblochez SIGUSR1 \n"); 
        sigdelset(&sigset, SIGUSR1); // se deblocheaza numai semnal SIGUSR1/ 

        /* ------------------ Sfarsit zona critica ------------------ */


        fprintf(stderr, "C: emit SIGUSR2 catre parinte pentru a incepe sa "
                        "emita semnale \n"); 
        kill(getppid(), SIGUSR2); // emit semnal SIGUSR2 catre parinte - reluare parinte suspendat 
        /* astept numai semnal SIGUSR1 - restul semnalelor sunt blocate */ 
        sigsuspend(&sigset); 
        /* aici se ajunge. Return din TratareSemnalSIGUSR1 */ 
        fprintf(stderr, "C: am iesit din asteptare semnal. SIGUSR1 a fost "
                        "livrat\n");

        fprintf(stderr, "C: refac masca de semnal initiala cu semnal SIGINT "
                        "probabil deblocat\n"); 
        /* Restaureaza masca de semnal in care SIGINT (probabil) nu e blocat*/ 
        sigprocmask(SIG_SETMASK, &sigoldmask, NULL); 
    } 
    else 
    { /* cod parinte */ 
        int stat; 
        int TratareSemnalSIGUSR2(); 
        signal(SIGUSR2, (void (*))(int)TratareSemnalSIGUSR2); 
        pause();
        /* se asteapta un semnal SIGUSR2 de la copil atunci cand copilul asteapta semnale */ 
        fprintf(stderr, "P: Am primit SIGUSR2, deci Copilul este pregatit "
                        "pentru a receptiona SIGINT si SIGUSR1\n"); 
        fprintf(stderr, "P: emit un semnal SIGINT - inca blocat de Copil, "
                        "catre Copil\n"); 
        kill(pid, SIGINT); // emit semnal SIGINT ctrl-c (fara efect- semnalul este blocat in Copil) 
        fprintf(stderr, "P: emit un semnal SIGUSR1 - deblocat de Copil, "
                        "catre Copil\n"); 
        kill(pid, SIGUSR1); // emit semnal SIGUSR1 (cu efect- semnalul nu este blocat in Copil) 
        fprintf(stderr, "P: Astep terminarea Copilului\n"); 
        pid = wait(&stat);
        /* Macro WEXITSTATUS(stat) returneaza codul de retur al procesului copil dat prin exit(100) */
        fprintf(stderr,"P: Copilul a iesit cu exit status = %d\n", 
                        WEXITSTATUS(stat)); 
    }
    /* secventa comuna parinte/copil */ 
    exit(100); // cod retur proces = 100 
} 
/* Functii tratare semnale / sunt activate numai pentru semnale neblocate */ 
int TratareSemnalSIGUSR1 (int signo) 
{ 
    fprintf(stderr, "\tC-TS: tratare semnal SIGUSR1 - Semnal %d - emis din "
                    "parinte\n", signo); 
    fflush(stderr); // golire fortata bufer 
    return 200; 
} 
int TratareSemnalSIGINT (int signo) 
{ 
    fprintf(stderr, "\tC-TS: tratare semnal SIGINT - Semnal %d - emis din "
                    "parinte\n", signo); 
    fflush(stderr); // golire fortata bufer 
    return 300; 
} 
int TratareSemnalSIGUSR2 () 
{ 
    signal(SIGUSR2,(void (*))(int)TratareSemnalSIGUSR2); 
    fprintf(stderr, "\tP-TS: tratare semnal SIGUSR2 - Semnal %d - emis din "
                    "copil- porneste parintele\n", SIGUSR2);
    fflush(stderr); // golire fortata bufer 
    return 400; 
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