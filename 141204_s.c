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
}my_msg;

struct msg_sen{
  long mpid;
  int mres;
  char mtext[100];
}msg_send;

struct account{
    char id[8];
    bool in;
    int pid;
}list[9];

struct gr{
  char name[14];
  char list[9][14];
  int count;
}group[3];

int authorize(char* login,int c );
void logged_users_list();
void join_group();
void group_users(char name[14]);
void send_to_user();
void send_to_group();

int main(int argc, char* argv[]){
    int i=0;
    FILE *fp;
    fp = fopen("dane.txt","r");
    setbuf(fp, NULL);
    while(i<9){
        fscanf(fp,"%s",list[i].id);
        list[i].in = 0;
        i+=1;}
    i=0;
    while(!feof(fp)){
      fscanf(fp,"%s",group[i].name);
      i+=1;}
    fclose(fp);
    int mid = msgget(4444,0644|IPC_CREAT);
    int cid = msgget(5555,0644|IPC_CREAT);
    while(true){
      msgrcv(mid,&my_msg,sizeof(my_msg)-sizeof(long),0,0);
      switch(my_msg.mtype/1000){
        case 1:
          printf("hello\n");
          int a = authorize(my_msg.mtext,1);
          list[a].pid=my_msg.mpid;
          msg_send.mpid=my_msg.mpid;
          msgsnd(mid,&msg_send,sizeof(msg_send)-sizeof(long),0);
          break;

        case 2:
          printf("bye\n");
          a = authorize(my_msg.mtext,0);
          msg_send.mpid=my_msg.mpid;
          msgsnd(mid,&msg_send,sizeof(msg_send)-sizeof(long),0);
          break;

        case 3:
          logged_users_list();
          msg_send.mpid=my_msg.mpid;
          msgsnd(mid,&msg_send,sizeof(msg_send)-sizeof(long),0);
          break;

        case 4:
          group_users(my_msg.mtext);
          msg_send.mpid=my_msg.mpid;
          msgsnd(mid,&msg_send,sizeof(msg_send)-sizeof(long),0);
          break;

        case 5:
          join_group(my_msg.mtext,1);
          msg_send.mpid=my_msg.mpid;
          msgsnd(mid,&msg_send,sizeof(msg_send)-sizeof(long),0);
          break;

        case 6:
          join_group(my_msg.mtext,0);
          msg_send.mpid=my_msg.mpid;
          msgsnd(mid,&msg_send,sizeof(msg_send)-sizeof(long),0);
          break;

        case 7:
          strcpy(msg_send.mtext,"Dostepne grupy:\n");
          for(int i=0;i<3;i++){
            strcat(msg_send.mtext,group[i].name);
            strcat(msg_send.mtext,"\n");
          }
          msg_send.mpid=my_msg.mpid;
          msgsnd(mid, &msg_send,sizeof(msg_send)-sizeof(long),0);
          break;

        case 8:
          send_to_group();
          break;

        case 9:
          send_to_user(cid);
          break;
      }
    }
    return 0;
}

int authorize(char* login,int c ){
    for (int i=0; i<9; i++){
        if(strcmp(login,list[i].id)==0){
                if(c && list[i].in==0){
                    list[i].in=1;
                    msg_send.mres = 1;
                    strcpy(msg_send.mtext,"Pomyslnie zalogowano.\n");
                    return i;}
                else if(!c && list[i].in==1){
                    list[i].in=0;
                    msg_send.mres = 1;
                    strcpy(msg_send.mtext,"Pomyslnie wylogowano.\n");
                    return i;}
                else{
                  strcpy(msg_send.mtext,"Nie udalo sie wykonac operacji.\n");
                  msg_send.mres = 0;
                  return 0;}
                }
        }
    strcpy(msg_send.mtext,"Nie ma uzytkownika o takim loginie.\n");
    msg_send.mres = 0;
    return 0;}

void join_group(char groupname[14],int c){
  int index=-1;
  for (int j=0;j<3; j++){
    if(strcmp(group[j].name,groupname)==0){
      index=j;
      break;}
  }
  if(index==-1){
    strcpy(msg_send.mtext,"Nie ma takiej grupy.\n");
    return;}
  for(int i=0; i<9; i++){
    if(my_msg.mpid==list[i].pid){
      if(c){
        strcpy(group[index].list[group[index].count],list[i].id);
        group[index].count+=1;
        strcpy(msg_send.mtext,"Zapisano cie do grupy.\n");
        return;
      }else{
        for(int k=0;k<group[index].count;k++){
          if(strcmp(group[index].list[k],list[i].id)==0){
            strcpy(group[index].list[k],"");
            group[index].count-=1;
            strcpy(msg_send.mtext,"Wypisano cie pomyslnie.\n");
            return;}
          else{strcpy(msg_send.mtext,"Nie należysz do tej grupy.\n");
        return;}}
  }}}}

void logged_users_list(){
  strcpy(msg_send.mtext,"Zalogowani uzytkownicy:\n");
  for(int i=0;i<9;i++){
    if(list[i].in==1){
      strcat(msg_send.mtext,list[i].id);
      strcat(msg_send.mtext,"\n");
  }}}

void group_users(char name[14]){
  for (int i=0;i<3;i++){
    if(strcmp(name,group[i].name)==0){
        if(group[i].count){
          strcpy(msg_send.mtext,"Uzytkownicy nalezacy do grupy ");
          strcat(msg_send.mtext,name);
          strcat(msg_send.mtext,":\n");
          for(int j=0;j<group[i].count;j++){
            strcat(msg_send.mtext,group[i].list[j]);
            strcat(msg_send.mtext,"\n");}
          return;
        }else{
          strcpy(msg_send.mtext,"Zaden uzytkownik nie nalezy do tej grupy.\n");
          return;
  }}}
  strcpy(msg_send.mtext,"Nie znaleziono takiej grupy.\n");
  return;
}

void send_to_group(int cid){
  printf("Zaczynam wysyłanie");
  fflush(stdout);
  int group_index;
  int sender_pid=my_msg.mpid;
  char receiver_name[14];
  strcpy(receiver_name,my_msg.mtext);
  for (int i=0;i<9;i++){
    if(list[i].pid==sender_pid){
      strcpy(my_msg.mtext,list[i].id);
      printf("od %s\n",list[i].id);
      fflush(stdout);
      break;
    }}
  for(int j=0;j<3;j++){
    if(strcmp(group[j].name,receiver_name)==0){
      group_index=j;
      printf("grupa nr %d do %d uzytkownikow\n",group_index,group[group_index].count);
      fflush(stdout);
      break;}
    }
  for(int k=0;k<group[group_index].count;k++){
    for(int l=0;l<9;l++){
      if(strcmp(group[group_index].list[k],list[l].id)==0){
        my_msg.mtype=list[l].pid;
        printf("%s\n",list[l].id);
        if(!msgsnd(cid,&my_msg,sizeof(my_msg)-sizeof(long),0)){
          printf("Wyslano wiadomosc\n");}
        break;}
    }
  }
}

void send_to_user(int cid){
  printf("Zaczynam wysyłanie");
  fflush(stdout);
  int sender_pid=my_msg.mpid;
  char receiver_name[14];
  strcpy(receiver_name,my_msg.mtext);
  for (int i=0;i<9;i++){
    if(list[i].pid==sender_pid){
      strcpy(my_msg.mtext,list[i].id);
      printf("od %s\n",list[i].id);
      fflush(stdout);
      break;
    }}
    for(int l=0;l<9;l++){
      if(strcmp(receiver_name,list[l].id)==0){
        my_msg.mtype=list[l].pid;
        printf("%s\n",list[l].id);
        if(!msgsnd(cid,&my_msg,sizeof(my_msg)-sizeof(long),0)){
          printf("Wyslano wiadomosc\n");}
        break;}
    }
}
