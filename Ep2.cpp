#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

//Gabriel M. Jospin

// ######### ESCREVA O NROUSP AQUI
int nroUSP() {
    return 11796020;
}
//-------------------------------------------

// CHLINK eh um par <chave,link> - dentro de cada página há 3 CHLINKs
typedef struct {
    int chave;
    int linkdir;
} CHLINK;

// definicao de creat_pagina da árvore / registro do arquivo
typedef struct {
    int np; // armazene aqui o nro da creat_pagina no arquivo
    int cont; // armazene aqui a quantidade de chaves existentes no vetor CHLINK[]
    CHLINK item[3]; // a chave[0] eh desprezada, e o linkdir[0] eh na verdade o link esquerdo da chave[1]
} PAGINA;



PAGINA getPagina(FILE* file, int position);
PAGINA nullPagina();
PAGINA creat_pagina(int np, int cont, CHLINK* chlink);
bool pagina_isNull(PAGINA pagina);
void insertPagina(FILE* file, PAGINA pagina);
PAGINA dad(FILE* file, int chaveFiha,int position);

CHLINK chlink(int link, int chave);
void nullChlink(CHLINK *Tchlink);
void copyArray(CHLINK* receive, CHLINK* donate);
void CHlink_withKey(CHLINK* Tchlink, int ch);
bool hasLink(CHLINK* chlink);
bool contains(CHLINK list[3], int ch);
int direcao(CHLINK* chlink,int ch);
bool merge(CHLINK* receive, int rCount, CHLINK* donate, int dCount);
void broke(CHLINK* original, CHLINK* left, CHLINK* center, CHLINK* right,CHLINK* insertion);

int flength(FILE* file, char* arq);

void insercaoEspecial(FILE* file,char* arq, PAGINA pagina,CHLINK* insertion);
int insercaoSimples(FILE* file,char* arq, PAGINA pagina, CHLINK item[3]);
void insercaoEspecialDad(FILE* file, char* arq, PAGINA dadPage, PAGINA son);
void inserir(char nomearq[], int* raiz, int ch);

int min(int a, int b);
int max(int a, int b);


//MANIPULÇÃO DE PAGINA
PAGINA nullPagina(){
    CHLINK link[3];
    nullChlink(link);
    return creat_pagina(-1, 0, link);
}


PAGINA getPagina(FILE* file, int position){
    int size = (int ) sizeof(PAGINA);
    PAGINA Tpagina;
    rewind(file);
    fseek(file,position*size,SEEK_SET);
    int read = fread(&Tpagina, size,1,file);
    if(read < 1){
        return nullPagina();
    }
    else
        return Tpagina;
}

PAGINA creat_pagina(int np, int cont, CHLINK* chlink){
    PAGINA pagina;
    pagina.np = np;
    pagina.cont = cont;
    copyArray(pagina.item, chlink);
    return pagina;
}

bool pagina_isNull(PAGINA pagina) {
    if(pagina.np < 0)
        return true;
    if(pagina.cont <= 0)
        return true;
    if(pagina.item[1].chave <0)
        return true;
    return false;
}

void insertPagina(FILE* file, PAGINA pagina){
    int size = (int) sizeof(PAGINA);
    int position = pagina.np * size;
    rewind(file);
    fseek(file, position, SEEK_SET);
    int write;
    write = fwrite(&pagina, sizeof(PAGINA), 1, file);
    if(write<1)
        perror("!!!Erro ao escrever pagina line 96!!!!");

}


PAGINA dad(FILE* file, int chaveFiha,int position){
    if(position == -1)
        return nullPagina();
    PAGINA pag = getPagina(file,position);
    if(pagina_isNull(pag) || contains(pag.item,chaveFiha))
        return nullPagina();

    if(pag.item[1].chave > chaveFiha){
        PAGINA dadPag = dad(file,chaveFiha,pag.item[0].linkdir);
        if(pagina_isNull(dadPag))
            return pag;
        return dadPag;
    }

    if(pag.item[2].chave != -1 && pag.item[2].chave < chaveFiha){
        PAGINA dadPag = dad(file,chaveFiha,pag.item[2].linkdir);
        if(pagina_isNull(dadPag))
            return pag;
        return dadPag;
    }

    PAGINA dadPag = dad(file,chaveFiha,pag.item[1].linkdir);
    if(pagina_isNull(dadPag))
        return pag;
    return dadPag;
}

//Manipulação de CHLINK[]
CHLINK chlink(int link, int chave){
    CHLINK Tchlink;
    Tchlink.linkdir = link;
    Tchlink.chave = chave;
    return Tchlink;
}

void nullChlink(CHLINK *Tchlink){
    Tchlink[0] = chlink(-1,-1);
    Tchlink[1] = chlink(-1,-1);
    Tchlink[2] = chlink(-1,-1);
}

void CHlink_withKey(CHLINK* Tchlink, int ch){
    nullChlink(Tchlink);
    Tchlink[1].chave = ch;
}

bool hasLink(CHLINK* chlink){
    for (int i = 0; i < 3; ++i) {
        if(chlink[i].linkdir != -1)
            return true;
    }
    return false;
}

bool contains(CHLINK list[3], int ch) {
    for (int i = 0; i < 3; ++i)
        if(list[i].chave == ch)
            return true;

    return false;
}


int direcao(CHLINK* chlink,int ch){
    if(ch < chlink[1].chave)
        return 0;
    if(chlink[2].chave != -1 && ch > chlink[2].chave)
        return 2;
    return 1;
}


bool merge(CHLINK* receive, int rCount, CHLINK* donate, int dCount) {
    if (rCount + dCount <= 0 || rCount + dCount > 2)
        return false;

    if (rCount == 0) {
        receive[1].chave = donate[1].chave;
        receive[1].linkdir = donate[1].linkdir;
        return true;
    }

    int donateK = donate[1].chave;
    int receiveK = receive[1].chave;

    receive[1].chave = min(donateK, receiveK);
    receive[2].chave = max(donateK, receiveK);

    if (donateK < receiveK) {
        receive[2].linkdir = receive[1].linkdir;
        receive[1].linkdir = receive[0].linkdir;
        receive[0].linkdir = donate[0].linkdir;
        return true;
    }
    receive[2].linkdir = donate[1].linkdir;
    return true;
}

void broke(CHLINK* original, CHLINK* left, CHLINK* center, CHLINK* right,CHLINK* insertion){
    if(insertion[1].chave < original[1].chave){
        left[1].chave = insertion[1].chave;
        left[0].linkdir = insertion[0].linkdir;
        left[1].linkdir = original[0].linkdir;
        center[1].chave = original[1].chave;
        right[1].chave = original[2].chave;
        right[0].linkdir = original[1].linkdir;
        right[1].linkdir = original[2].linkdir;
        return;
    }

    if(insertion[1].chave > original[2].chave){
        left[1].chave = original[1].chave;
        left[0].linkdir = original[0].linkdir;
        left[1].linkdir = original[1].linkdir;
        center[1].chave = original[2].chave;
        right[1].chave = insertion[1].chave;
        right[0].linkdir = original[2].linkdir;
        right[1].linkdir = insertion[1].linkdir;
        return;
    }

    left[1].chave = original[1].chave;
    left[0].linkdir = original[0].linkdir;
    left[1].linkdir = original[1].linkdir;
    center[1].chave = insertion[1].chave;
    right[1].chave = original[2].chave;
    right[1].linkdir = original[2].linkdir;
}

void copyArray(CHLINK* receive, CHLINK* donate){
    for (int i = 0; i < 3; i++){
        receive[i].chave = donate[i].chave;
        receive[i].linkdir = donate[i].linkdir;
    }
}

//Manipulação de Arqivo
int flength(FILE* file, char* arq){
    int position = ftell(file);
    fclose(file);
    fopen(arq,"rb+");
    fseek(file,0,SEEK_END);
    int length = ftell(file);
    fseek(file,position,SEEK_SET);
    return length;
}


//Metodos de insercao

int insercaoSimples(FILE* file,char* arq, PAGINA pagina, CHLINK item[3]){
    if(pagina.np == -1) pagina.np = flength(file,arq) / (int) sizeof(PAGINA);
    copyArray(pagina.item,item);
    pagina.cont++;
    insertPagina(file,pagina);
    return pagina.np;
}


void insercaoEspecial(FILE* file,char* arq, PAGINA pagina,CHLINK* insertion){
    CHLINK left[3];
    nullChlink(left);
    CHLINK right[3];
    nullChlink(right);
    CHLINK central[3];
    nullChlink(central);

    broke(pagina.item,left,central,right,insertion);

    PAGINA pNull = creat_pagina(-1,0,insertion);
    central[0].linkdir = insercaoSimples(file,arq,pNull,left);
    central[1].linkdir = insercaoSimples(file,arq,pNull,right);

    PAGINA dadPage = dad(file,pagina.item[1].chave,0);
    if(!pagina_isNull(dadPage) && dadPage.cont == 1){
        merge(central,1,dadPage.item,1);
        insercaoSimples(file,arq,dadPage,central);
        return;
    }else if(!pagina_isNull(dadPage)){
        pagina.cont = 0;
        int pos = insercaoSimples(file,arq,pagina,central);
        pagina = getPagina(file,pos);
        insercaoEspecialDad(file,arq,dadPage,pagina);
        return;
    }
    pagina.cont = 0;
    insercaoSimples(file,arq,pagina,central);
}

void insercaoEspecialDad(FILE* file, char* arq, PAGINA dadPage, PAGINA son){
    PAGINA GrandPa = dad(file,dadPage.item[1].chave,0);


    if(son.item[1].chave > dadPage.item[2].chave){
        CHLINK left[3];
        nullChlink(left);
        left[1].chave = dadPage.item[1].chave;
        left[0].linkdir = dadPage.item[0].linkdir;
        left[1].linkdir = dadPage.item[1].linkdir;
        dadPage.item[1].chave = dadPage.item[2].chave;
        dadPage.item[2].chave = -1;
        dadPage.cont = 0;

        PAGINA pLeft = nullPagina();
        int lLeft = insercaoSimples(file, arq, pLeft, left);
        dadPage.item[0].linkdir = lLeft;
        dadPage.item[1].linkdir = son.np;
        dadPage.item[2].linkdir = -1;


    } else if(son.item[1].chave < dadPage.item[1].chave) {
        CHLINK right[3];
        nullChlink(right);
        right[1].chave = dadPage.item[2].chave;
        right[0].linkdir = dadPage.item[1].linkdir;
        right[1].linkdir = dadPage.item[2].linkdir;
        dadPage.item[2].chave = -1;
        dadPage.cont = 0;

        PAGINA pRight = nullPagina();
        int lRight = insercaoSimples(file,arq,pRight,right);
        dadPage.item[0].linkdir = son.np;
        dadPage.item[1].linkdir = lRight;
        dadPage.item[2].linkdir = -1;
    } else{
        CHLINK right[3];
        nullChlink(right);
        CHLINK left[3];
        nullChlink(left);

        left[1].chave = dadPage.item[1].chave;
        left[0].linkdir = dadPage.item[0].linkdir;
        left[1].linkdir = son.item[0].linkdir;

        right[1].chave = dadPage.item[2].chave;
        right[0].linkdir = son.item[1].linkdir;
        right[1].linkdir = dadPage.item[2].linkdir;

        dadPage.item[2].chave = -1;
        dadPage.item[1].chave = son.item[1].chave;


        PAGINA pLeft = nullPagina();
        int lLeft = insercaoSimples(file, arq, pLeft, left);
        PAGINA pRight = nullPagina();
        int lRight = insercaoSimples(file, arq, pRight, right);

        dadPage.item[0].linkdir = lLeft;
        dadPage.item[1].linkdir = lRight;
        dadPage.item[2].linkdir = -1;

    }


    if(!pagina_isNull(GrandPa) && GrandPa.cont == 1){
        merge(dadPage.item,1,GrandPa.item,1);
        insercaoSimples(file,arq,GrandPa,dadPage.item);
    }else if(!pagina_isNull(GrandPa)){
        dadPage.cont = 0;
        int pos = insercaoSimples(file,arq,dadPage,dadPage.item);
        dadPage.np = pos;
        insercaoEspecialDad(file,arq,GrandPa,dadPage);

    }else
        insercaoSimples(file, arq, dadPage, dadPage.item);

}

//Metodo do Exercício
void inserir(char nomearq[], int* raiz, int ch){
    FILE* file = fopen(nomearq, "rb+");int root = *raiz;
    int position = (*raiz) * (int) sizeof(PAGINA);
    PAGINA pagina = getPagina(file,(*raiz));

    if(contains(pagina.item,ch))
        return;



    CHLINK insertion[3];
    CHlink_withKey(insertion,ch);

    if(pagina_isNull(pagina)){
        insercaoSimples(file,nomearq, pagina,insertion);

    }

    else if(hasLink(pagina.item)){
        int direction = direcao(pagina.item,ch);
        if(pagina.item[direction].linkdir == -1){
            pagina.item[direction].linkdir = flength(file,nomearq) / (int) sizeof(PAGINA);
            insertPagina(file,pagina);
        }
        int newRoot = pagina.item[direction].linkdir;
        inserir(nomearq,&newRoot,ch);

    }

    else if(merge(pagina.item,pagina.cont,insertion,1)){
        insercaoSimples(file,nomearq, pagina,pagina.item);

    }else
        insercaoEspecial(file,nomearq,pagina,insertion);

    fclose(file);
}

//METODOS EXTRAS E UNIVERSAIS
int min(int a, int b){
    return a < b ? a : b;
}
int max(int a, int b){
    return a > b ? a : b;
}


/*
“All that is gold does not glitter,
Not all those who wander are lost;
The old that is strong does not wither,
Deep roots are not reached by the frost.

From the ashes a fire shall be woken,
A light from the shadows shall spring;
Renewed shall be blade that was broken,
The crownless again shall be king.”
*/