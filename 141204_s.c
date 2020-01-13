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
  int mtext;
}my_msg;

struct account{
    char id[8];
    char password[8];
    bool in;
}list[3];

void authorize(char* login,char* password,int c );
void logged_users_list();

int main(int argc, char* argv[]){
    int pids[9];
    int i=0;
    FILE *fp;
    fp = fopen("dane.txt","r");
    setbuf(fp, NULL);
    while(!feof(fp)){
        fscanf(fp,"%s",list[i].id);
        fscanf(fp,"%s",list[i].password);
        //printf("%s %s\n",list[i].id,list[i].password );
        list[i].in = 0;
        i+=1;}
    fclose(fp);
    int mid = msgget(4444,0644|IPC_CREAT);
    while(true){
      if(msgrcv(mid,&my_msg,sizeof(int),1,0)==sizeof(int)){

      }

      printf("%d\n",my_msg.mtext);
      fflush(stdout);
    }
}

void authorize(char* login,char* password,int c ){
    for (int i=0; i<3; i++){
        if(strcmp(login,list[i].id)==0){
            if(strcmp(password,list[i].password)==0){
                if(c){
                    list[i].in=1;
                    printf("\nPomyslnie zalogowano uzytkownika %s.\n\n",login);
                    return ;}
                else{
                    list[i].in=0;
                    printf("\nPomyslnie wylogowano uzytkownika %s.\n\n",login);
                    return ;}
            }else{
                printf("Haslo jest niepoprawne.\n");
                return;}
        }}
    printf("Nie ma uzytkownika o takim loginie.\n");
    return ;}

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
