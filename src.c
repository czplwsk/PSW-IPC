
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

struct account{
    char id[8];
    char password[8];
};

int authorize(char* login,char* password, struct account lista[3] ){
    for (int i=0; i<10; i++){
        if(strcmp(login,lista[i].id)==0){
            if(strcmp(password,lista[i].password)==0){
                printf("Pomyslnie zalogowano uzytkownika %s.\n",login);
                
            }
            else{printf("Haslo jest niepoprawne. Nie udalo się zalogowac.\n");}
        }
        else{printf("Nie ma uzytkownika o takim loginie.\n");}
        break;
    }
    return 0;
}

int main(){
    int i=0;
    FILE *fp;
    fp = fopen("dane.txt","r");
    setbuf(fp, NULL);
    struct account list[3];

    while(!feof(fp)){
        fscanf(fp,"%s%s",list[i].id,list[i].password);
        fflush(fp);
        i+=1;
    }
    authorize("eliza","eliza",list);
    fclose(fp);
}