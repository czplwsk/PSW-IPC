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
}my_msg;

struct msg_sen{
  long mpid;
  char mtext[100];
}msg_send;

struct account{
    char id[8];
    bool in;
    int pid;
}list[3];

int authorize(char* login,int c );
void logged_users_list();

int main(int argc, char* argv[]){
    int i=0;
    FILE *fp;
    fp = fopen("dane.txt","r");
    setbuf(fp, NULL);
    while(!feof(fp)){
        fscanf(fp,"%s",list[i].id);
        list[i].in = 0;
        i+=1;}
    fclose(fp);
    int mid = msgget(4444,0644|IPC_CREAT);
    while(true){
      msgrcv(mid,&my_msg,sizeof(my_msg)-sizeof(long),0,0);
      switch(my_msg.mtype/1000){
        case 1:
          printf("hello\n");
          int a = authorize(my_msg.mtext,1);
          if(a)list[a].pid=my_msg.mpid;
          msg_send.mpid=my_msg.mpid;
          msgsnd(mid,&msg_send,sizeof(msg_send)-sizeof(long),0);
          break;

        case 2:
        break;
      }
    }
    return 0;
}

int authorize(char* login,int c ){
    for (int i=0; i<3; i++){
        if(strcmp(login,list[i].id)==0){
                if(c && list[i].in==0){
                    list[i].in=1;
                    strcpy(msg_send.mtext,"Pomyslnie zalogowano.\n");
                    return i;}
                else if(!c && list[i].in==1){
                    list[i].in=0;
                    strcpy(msg_send.mtext,"Pomyslnie wylogowano.\n");
                    return i;}
                else{
                  strcpy(msg_send.mtext,"Nie udało się wykonać operacji.\n");
                  return 0;}
                }
        }
    strcpy(msg_send.mtext,"Nie ma uzytkownika o takim loginie.\n");
    return 0;}

void logged_users_list(){
  printf("Zalogowani użytkownicy:\n");
  for(int i=0;i<3;i++)
  {
    if(list[i].in==1)
    {
      printf("%s\n",list[i].id );
    }
  }
}
