#define _XOPEN_SOURCE 600
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <sys/types.h>
#include <unistd.h>

void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

struct account{
    char id[8];
    char password[8];
    bool in;
}list[3];

//struct account *logged = malloc(3*sizeof(struct account));
void authorize(char* login,char* password,int c );
void user();
void interface();


int main(int argc, char* argv[]){
    int i=0;
    FILE *fp;
    fp = fopen("dane.txt","r");
    setbuf(fp, NULL);
    while(!feof(fp)){
        fscanf(fp,"%s",list[i].id);
        fscanf(fp,"%s",list[i].password);
        printf("%s %s\n",list[i].id,list[i].password );
        list[i].in = 0;
        i+=1;}
    interface(list);
    //if(fork()==0)
    //{execv("141204_k.c",NULL);}
    fclose(fp);
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

void interface(){
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
            user();
            break;
        case '2':
            break;
        }
}}

void user(){
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
            scanf("%s%s",id,password);
            clean_stdin();
            authorize(id, password,1);
            break;
        case '2':
            printf("Podaj login i haslo: \n");
            scanf("%s%s",id,password);
            clean_stdin();
            authorize(id, password,0);
            break;
        case '3':
            printf("Zalogowani użytkownicy:\n");
            for(int i=0;i<3;i++)
            {
              if(list[i].in==1)
              {
                printf("%s\n",list[i].id );
              }
            }
            break;}
    return;}
