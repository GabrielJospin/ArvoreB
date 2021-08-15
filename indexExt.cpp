#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
using namespace std;

typedef struct {
    int key;
    long address;
}REGISTER;

typedef struct No {
   REGISTER* rg;
   No* prox;
}NO;

typedef struct Info{
    int key;
    int album;
    int artista;
}INFO;


void print(INFO list[],int length);
void print(NO* list);
INFO Info(int key, int album, int artista);

bool insertRegister(NO* *list, int key, long address){
    NO* table = *list;
    REGISTER* rg = (REGISTER*) malloc(sizeof(REGISTER));
    rg->address = address;
    rg->key = key;
    NO* no = (NO*) malloc(sizeof(NO));
    no->rg = rg;
    no->prox = NULL;

    if(!table){
        *list = no;
        return true;
    }

    NO* back = NULL;
    while(table && table->rg->key <= key){
        back = table;
        table = table->prox;
    }

   if(back && back->rg->key == key)
       return false;


   if(back){
       no->prox = back->prox;
       back->prox = no;
       return true;
   }

   no->prox = *list;
   *list = no;
    return true;
}

int searchAddress(NO* list, int key){
    NO* it = list;
    while(list && list->rg->key < key)
        list = list->prox;

    if(list && list->rg->key == key)
        return list->rg->address;

    return -1;
}

int deleteRegister(NO* *list, int key){
    NO* it = *list;
    NO* back = NULL;
    while(it && it->rg->key < key){
        back=it;
        it = it->prox;
    }

    if(it->rg->key != key)
        return -1;

    long address = it->rg->address;
    back->prox = it->prox;
    free(it);

    return address;
}

int insertInfo(INFO info, NO* *list, FILE* file, int* length){
    int position = *length;
    fseek(file,0,SEEK_END);
    fwrite(&info,sizeof(INFO),1,file);
    insertRegister(list,info.key, position * sizeof(info));
    (*length)++;
    return 0;
}

int searchInfo(int key, NO* list, FILE* file, INFO* info){
    int address = searchAddress(list,key);
    if(address > -1){
        fseek(file,address,SEEK_SET);
        int read = fread(info,sizeof(INFO),1,file);
        if(read<1)
            printf("Erro, not found\n");
        else
            return 0;
    }

    perror("Erro ao encontrar a chave");
    return -1;
}

void deleteInfo(int key, NO* list, FILE* file){
    INFO info = Info(-1, 0 ,0);

    int address = deleteRegister(&list,key);
    if(address>-1){
        fseek(file,address,SEEK_SET);
        fwrite(&info,sizeof(INFO),1,file);
    }else{
        perror("Erro ao encontrar a chave");
    }
}


NO* creatTable(){
    long infoSize = sizeof(INFO);
    NO* list = NULL;
    INFO* infos = (INFO*) malloc(infoSize * 5);

    FILE* file = fopen("..\\dados.bin","rb");
    fread(infos, infoSize,5,file);
    for(int i = 0; i < 5; i++){
        insertRegister(&list,infos[i].key,i*infoSize);
    }
    fclose(file);
    return list;
}

void print(NO* list){
    printf("chave | endereco \n");
    while(list){
        REGISTER* rg = list->rg;
        printf("%d | %d\n", rg->key, rg->address);
        list = list->prox;
    }
}

void print(INFO* info){
    if(info->key == -1)
        return;
    printf("{key = %d, album = %d, artista=  %d }\n",info->key,info->album, info->artista);

}

void print(INFO list[],int length){    printf("[");
    for(int i = 0; i<length; i++)
        printf("%d, ",list[i].key);
    printf("]\n");
}

INFO Info(int key, int album, int artista){
    INFO info;
    info.album = album;
    info.key = key;
    info.artista = artista;
    return info;
}

NO* creatFiles(FILE* file){
    INFO info;
    NO* list = NULL;
    int lenght = 0;
    info = Info(1,0,55);
    insertInfo(info,&list,file,&lenght);
    info = Info(4,2,55);
    insertInfo(info,&list,file,&lenght);
    info = Info(3,0,52);
    insertInfo(info,&list,file,&lenght);
    info = Info(2,7,52);
    insertInfo(info,&list,file,&lenght);
    info = Info(5,0,53);
    insertInfo(info,&list,file,&lenght);
    info = Info(6,9,53);
    insertInfo(info,&list,file,&lenght);
    return list;
}


int main() {
    FILE* fileWrite = fopen("..\\dados.bin","wb");
    FILE* fileRead = fopen("..\\dados.bin","rb");
    NO* list = creatFiles(fileWrite);
    print(list);
    INFO info;
    int result = searchInfo(1,list,fileRead,&info);
    if(result>=0) print(&info);
    deleteInfo(2,list,fileWrite);
    print(list);
    fclose(fileRead);
    fclose(fileWrite);

}
