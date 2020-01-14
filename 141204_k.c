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

struct msg_buf{
  long mtype;
  int mpid;
  char mtext[100];
}msg_pid;

struct msg_get{
  long mpid;
  char mtext[100];
}msg_rec;

void clean_stdin(){
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);}

void user();
void zaloguj();
void interface();

int main(int argc, char* argv[]){
    int mid = msgget(4444,0644|IPC_CREAT);
    zaloguj(mid);
    interface(mid);
}

void interface(int mid){
char x ='1';
while(x=='1' || x=='2' || x=='3'){
    printf("Co chcesz zrobic?\n");
    printf(" 1 <- Obsluga uzytkownika\n");
    printf(" 2 <- Obsulga grup\n");
    printf(" 3 <- Wiadomosci\n");
    printf(" 4 <- Wyloguj\n\n", );
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
    printf("Co chcesz zrobic?\n");
    printf(" 1 <- Wyswietl liste zalogowanych uzytkownikow\n");
    printf(" 2 <- Wyswietl liste uzytkownikow zapisancyh do danej grupy tematycznej\n\n");
    y=getchar();
    clean_stdin();
    switch (y)
    {
        case '2':
            printf("Podaj login: \n");
            scanf("%s",id);
            clean_stdin();
            break;}
    return;}

  void zaloguj(int mid){
    char id[8];
    printf("Podaj login: \n");
    scanf("%s",id);
    clean_stdin();
    strcpy(msg_pid.mtext,id);
    msg_pid.mtype = 1000;
    msg_pid.mpid = getpid();
    msgsnd(mid,&msg_pid,sizeof(msg_pid)-sizeof(long),0);
    msgrcv(mid,&msg_rec,sizeof(msg_rec)-sizeof(long),getpid(),0);
    printf("%s\n",msg_rec.mtext );
  }
