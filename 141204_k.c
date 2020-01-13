#define _XOPEN_SOURCE 600
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>

struct pid_buf{
  long mtype;
  int mpid;
  char mtext;
}msg_pid;



void clean_stdin(){
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);}

void user();
void interface();

int main(int argc, char* argv[]){
    int mid = msgget(4444,0644|IPC_CREAT);
    msg_pid.mtype = 1;
    msg_pid.mtext= getpid();
    msgsnd(mid,&msg_pid,sizeof(int),0);
    int id =msgget(getpid(),0644|IPC_CREAT);
    interface(id);
}

void interface(int mid){
char x ='1';
while(x=='1' || x=='2' || x=='3'){
    printf("Co chcesz zrobic?\n");
    printf(" 1 <- Obsluga uzytkownika\n");
    printf(" 2 <- Obsulga grup\n");
    printf(" 3 <- Wiadomosci\n\n");
    x=getchar();
    clean_stdin();
    switch(x){
        case '1':
            user(mid);
            break;
        case '2':
            break;
        }
}}

void user(int mid){
    char y = '0';
    char id[8];
    char password[8];
    printf("Co chcesz zrobic?\n");
    printf(" 1 <- Zaloguj\n");
    printf(" 2 <- Wyloguj\n");
    printf(" 3 <- Wyswietl liste zalogowanych uzytkownikow\n");
    printf(" 4 <- Wyswietl liste uzytkownikow zapisancyh do danej grupy tematycznej\n\n");
    y=getchar();
    clean_stdin();
    switch (y)
    {
        case '1':
            printf("Podaj login i haslo: \n");

            clean_stdin();
            break;
        case '2':
            printf("Podaj login i haslo: \n");
            scanf("%s%s",id,password);
            clean_stdin();
            break;}
    return;}
