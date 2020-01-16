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
  char mcontent[100];
}msg_pid;

struct msg_get{
  long mpid;
  int mres;
  char mtext[100];
}msg_rec;

void clean_stdin(){
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);}

void group();
void user();
void message();
void interface();

int main(int argc, char* argv[]){
    int mid = msgget(4444,0644|IPC_CREAT);
    int cid = msgget(5555,0644|IPC_CREAT);
    int parent = getpid();
    char id[8];
    while(!msg_rec.mres){
      printf("Podaj login: \n");
      scanf("%s",id);
      clean_stdin();
      strcpy(msg_pid.mtext,id);
      msg_pid.mtype = 1000;
      msg_pid.mpid = getpid();
      msgsnd(mid,&msg_pid,sizeof(msg_pid)-sizeof(long),0);
      msgrcv(mid,&msg_rec,sizeof(msg_rec)-sizeof(long),getpid(),0);
      printf("%s\n",msg_rec.mtext );
      fflush(stdout);}
    if(fork()==0){
      printf("Hej, tu potomek");
      fflush(stdout);
      while(getppid()==parent)
      {
        printf("Hello\n");
        if(msgrcv(cid,&msg_pid,sizeof(msg_pid)-sizeof(long),parent,0)!=-1)
        printf("Wiadomosc od %s:\n",msg_pid.mtext);
        fflush(stdout);
        printf("%s",msg_pid.mcontent);
        fflush(stdout);
      }
      return 0;
    }
    interface(mid,id);
    return 0;

}

void interface(int mid, char id[8]){
char x ='1';
while(x=='1' || x=='2' || x=='3'){
    printf("Co chcesz zrobic?\n");
    printf(" 1 <- Wyświetl wybraną listę\n");
    printf(" 2 <- Obsulga grup\n");
    printf(" 3 <- Wiadomosci\n");
    printf(" 4 <- Wyloguj\n\n");
    x=getchar();
    clean_stdin();
    switch(x){
        case '1':
          user(mid);
          break;

        case '2':
          group(mid);
          break;

        case '3':
          message(mid);
          break;

        case '4':
          strcpy(msg_pid.mtext,id);
          msg_pid.mtype = 2000;
          msg_pid.mpid = getpid();
          msgsnd(mid,&msg_pid,sizeof(msg_pid)-sizeof(long),0);
          msgrcv(mid,&msg_rec,sizeof(msg_rec)-sizeof(long),getpid(),0);
          printf("\n%s\n",msg_rec.mtext );
          fflush(stdout);
          break;

        }
}}

void user(int mid){
    char groupname[14];
    char y = '0';
    printf("Co chcesz zrobic?\n");
    printf(" 1 <- Wyswietl liste zalogowanych uzytkownikow\n");
    printf(" 2 <- Wyswietl liste uzytkownikow zapisancyh do danej grupy tematycznej\n\n");
    y=getchar();
    clean_stdin();
    switch (y)
    {
        case '1':
          msg_pid.mtype = 3000;
          msg_pid.mpid = getpid();
          msgsnd(mid, &msg_pid,sizeof(msg_pid)-sizeof(long),0);
          msgrcv(mid, &msg_rec,sizeof(msg_rec)-sizeof(long),getpid(),0);
          printf("\n%s\n",msg_rec.mtext);
          break;

        case '2':
          printf("Podaj nazwe grupy: \n");
          scanf("%s",groupname);
          clean_stdin();
          strcpy(msg_pid.mtext,groupname);
          msg_pid.mtype=4000;
          msg_pid.mpid=getpid();
          msgsnd(mid, &msg_pid,sizeof(msg_pid)-sizeof(long),0);
          msgrcv(mid, &msg_rec,sizeof(msg_rec)-sizeof(long),getpid(),0);
          printf("\n%s\n",msg_rec.mtext);
          break;}
    return;}

void group(int mid){
  char groupname[14];
  char y = '0';
  printf("Co chcesz zrobic?\n");
  printf(" 1 <- Zapisz się do grupy\n");
  printf(" 2 <- Wypisz się z grupy\n");
  printf(" 3 <- Wyświetl listę dostępnych grup\n\n");
  y=getchar();
  clean_stdin();
  switch (y)
  {
      case '1':
        printf("Podaj nazwe grupy: \n");
        scanf("%s",groupname);
        clean_stdin();
        strcpy(msg_pid.mtext,groupname);
        msg_pid.mtype=5000;
        msg_pid.mpid=getpid();
        msgsnd(mid, &msg_pid,sizeof(msg_pid)-sizeof(long),0);
        msgrcv(mid, &msg_rec,sizeof(msg_rec)-sizeof(long),getpid(),0);
        printf("\n%s\n",msg_rec.mtext);
        break;

      case '2':
        printf("Podaj nazwe grupy: \n");
        scanf("%s",groupname);
        clean_stdin();
        strcpy(msg_pid.mtext,groupname);
        msg_pid.mtype=6000;
        msg_pid.mpid=getpid();
        msgsnd(mid, &msg_pid,sizeof(msg_pid)-sizeof(long),0);
        msgrcv(mid, &msg_rec,sizeof(msg_rec)-sizeof(long),getpid(),0);
        printf("\n%s\n",msg_rec.mtext);
        break;

      case '3':
        msg_pid.mtype=7000;
        msg_pid.mpid=getpid();
        msgsnd(mid, &msg_pid,sizeof(msg_pid)-sizeof(long),0);
        msgrcv(mid, &msg_rec,sizeof(msg_rec)-sizeof(long),getpid(),0);
        printf("\n%s\n",msg_rec.mtext);
        break;
  }
}

void message(int mid){
  char receiver[14];
  char content[100];
  char y = '0';
  printf("Co chcesz zrobic?\n");
  printf(" 1 <- Wiadomosc do grupy\n");
  printf(" 2 <- Wiadomsc do uzytkownika\n");
  printf(" 3 <- Odbierz wiadomosci\n\n");
  y=getchar();
  clean_stdin();
  switch (y)
  {
      case '1':
        printf("Podaj nazwe grupy: \n");
        scanf("%s",receiver);
        clean_stdin();
        strcpy(msg_pid.mtext,receiver);
        printf("Podaj wiadomosc: \n");
        scanf("%s",content);
        clean_stdin();
        strcpy(msg_pid.mcontent,content);
        msg_pid.mtype=8000;
        msg_pid.mpid=getpid();
        msgsnd(mid,&msg_pid,sizeof(msg_pid)-sizeof(long),0);
        break;

        case '2':
          printf("Podaj login odbiorcy: \n");
          scanf("%s",receiver);
          clean_stdin();
          strcpy(msg_pid.mtext,receiver);
          printf("Podaj wiadomosc: \n");
          scanf("%s",content);
          clean_stdin();
          strcpy(msg_pid.mcontent,content);
          msg_pid.mtype=9000;
          msg_pid.mpid=getpid();
          msgsnd(mid,&msg_pid,sizeof(msg_pid)-sizeof(long),0);
          break;
    }
  }
