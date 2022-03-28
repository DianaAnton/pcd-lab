/**
Lab. 04 Tema 4.2
Titlul temei:
    2. Utilizarea de tehnici IPC de tip "coadă de mesaje". Crearea a două 
procese P1 și P2. Procesele NU sunt în relație de părinte-copil. Procesul 1 
crează o coadă și trimite un mesaj în coadă./ Procesul 2 Citește mesajul din 
coadă. /  program t04l04_2.c
    Sfaturi de urmat: 
        - Pentru P1 se vor utiliza apelurile: ftok(), msgget() și msgsnd(), 
        iar pentru P2 apelurile: ftok(), msgget(), msgrvc() și msgctl().
        - Inspiratie din exemplele din lab. 04, care tratau comunicarea prin 
        memorie partajata

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
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>

#define FISIER "."
#define P_ID 1

struct memorie {
    char message[500];
};


int main(int argc, char *argv[])
{
    struct memorie mem_send, mem_recv;
    key_t shm_key; // Cheie identificare segment memorie partajata
    int shm_id; // ID segment memorie partajata
    pid_t childpid_1, childpid_2;
    int status;
    
    childpid_1 = fork();
    if (childpid_1 == -1)
    {
        fprintf(stderr, "ERROR! NU s-a putut crea primul copil!\n");
        exit(1);
    }
    if (childpid_1 == 0)
    { // suntem in copilul 1
        printf("Copilul 1 cu PID %d si PPID %d.\n", getpid(), getppid());
        key_t key;
        int msg_id;
        msgctl(msg_id, IPC_RMID, NULL);

        char *text[500] = {"Salut!", "din", "primul", "copil."};

        // se genereaza cheia pentru primul proces
        key = ftok(FISIER, P_ID);
        if (key == -1)
        {
            fprintf(stderr, "ERROR! Nu s-a putut crea cheia!\n");
            exit(1);
        }
        for (int i = 0; i < 4; i++)
        {
            // se initializeaza mesajul
            msg_id = msgget(key, IPC_CREAT | 0666 );
            if (msg_id == -1)
            {
                fprintf(stderr, "ERROR! Nu s-a putut crea coada de mesaje!\n");
                exit(1);
            }
            // se copiaza textul in structura
            strcpy(mem_send.message, text[i]);
            // se trimite mesajul
            if (msgsnd(msg_id, &mem_send, sizeof(mem_send), IPC_NOWAIT) == -1)
            {
                fprintf(stderr, "ERROR! Mesajul '%s' nu a putut fi transmis!\n%s\n", 
                        text[i], strerror(errno));
                continue;
            }
            printf("Mesajul '%s' a fost trimis cu succes!\n");
            sleep(2);
        }
        // iesim din copil
        exit(0);
    }
    else 
    { // suntem in parinte
        // asteptam dupa primul copil
        wait(&status);

        childpid_2 = fork();
        if (childpid_2 == -1)
        {
            fprintf(stderr, "ERROR! NU s-a putut crea al doilea copil!\n%s\n", 
                    strerror(errno));
            exit(1);
        }
        if (childpid_2 == 0)
        { // suntem in copilul 2
            printf("Copilul 2 cu PID %d si PPID %d.\n", getpid(), getppid());
            key_t key;
            int msg_id;

            // primim toate mesajele
            for (int i = 0; i < 4; i++) {
                // se genereaza cheia pentru al doilea proces
                key = ftok(FISIER, P_ID);
                if (key == -1)
                {
                    fprintf(stderr, "ERROR! Nu s-a putut crea cheia!\n%s\n",
                             strerror(errno));
                    continue;
                }
                // se initializeaza mesajul
                msg_id = msgget(key, IPC_CREAT | 0666 | IPC_NOWAIT);
                if (msg_id == -1)
                {
                    fprintf(stderr, "ERROR! Nu s-a putut crea coada de mesaje!"
                                    "\n%s\n", strerror(errno));
                    continue;
                }

                // se primeste mesajul
                if (msgrcv(msg_id, &mem_recv, sizeof(mem_recv), 0, IPC_NOWAIT) == -1)
                {
                    fprintf(stderr, "ERROR! Nu s-a putut primi mesajul!\n%s\n", 
                            strerror(errno));
                    continue;
                }
                printf("Mesajul '%s' a fost primit cu succces!\n", 
                        mem_recv.message);
            }
            // distrugem coada de mesaje
            msgctl(msg_id, IPC_RMID, NULL);
            exit(0);
        }
        else
        { // suntem in parinte
            // asteptam dupa al doilea copil
            wait(&status);
        }
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