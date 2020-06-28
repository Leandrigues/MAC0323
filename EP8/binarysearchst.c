/****************************************************************
    Nome: Leandro Rodrigues da Silva
    NUSP: 10723944

    Ao preencher esse cabeçalho com o meu nome e o meu número USP,
    declaro que todas as partes originais desse exercício programa (EP)
    foram desenvolvidas e implementadas por mim e que portanto não 
    constituem desonestidade acadêmica ou plágio.
    Declaro também que sou responsável por todas as cópias desse
    programa e que não distribui ou facilitei a sua distribuição.
    Estou ciente que os casos de plágio e desonestidade acadêmica
    serão tratados segundo os critérios divulgados na página da 
    disciplina.
    Entendo que EPs sem assinatura devem receber nota zero e, ainda
    assim, poderão ser punidos por desonestidade acadêmica.

    Abaixo descreva qualquer ajuda que você recebeu para fazer este
    EP.  Inclua qualquer ajuda recebida por pessoas (inclusive
    monitoras e colegas). Com exceção de material de MAC0323, caso
    você tenha utilizado alguma informação, trecho de código,...
    indique esse fato abaixo para que o seu programa não seja
    considerado plágio ou irregular.

    Exemplo:

        A monitora me explicou que eu devia utilizar a função xyz().

        O meu método xyz() foi baseada na descrição encontrada na 
        página https://www.ime.usp.br/~pf/algoritmos/aulas/enumeracao.html.

    Descrição de ajuda ou indicação de fonte:

    Se for o caso, descreva a seguir 'bugs' e limitações do seu programa:

****************************************************************/

/*
 * MAC0323 Estruturas de Dados e Algoritmo II
 * 
 * Tabela de simbolos implementada atraves de vetores ordenados 
 * redeminsionaveis 
 *
 *     https://algs4.cs.princeton.edu/31elementary/BinarySearchST.java.html
 * 
 * As chaves e valores desta implementaÃ§Ã£o sÃ£o mais ou menos
 * genÃ©ricos
 */

/* interface para o uso da funcao deste mÃ³dulo */
#include "binarysearchst.h"

#include <stdlib.h> /* free() */
#include <string.h> /* memcpy() */
#include "util.h"   /* eemalloc(), ecalloc() */

#undef DEBUG
#ifdef DEBUG
#include <stdio.h> /* printf(): para debug */
#endif

/*
 * CONSTANTES 
 */

/*------------------------------------------------------------*/
/* 
 * FunÃ§Ãµes administrativas
 */
void resize (BinarySearchST st, size_t newSize);    
/*----------------------------------------------------------*/
/* 
 * Estrutura BÃ¡sica da Tabela de SÃ­mbolos: 
 * 
 * implementaÃ§Ã£o com vetores ordenados
 */
struct binarySearchST {
    int (*compar)(const void *key1, const void *key2);
    int n;
    int size;
    int currentIndex;
    const void** keys;
    const void** vals;
    size_t* sizesK;
    size_t* sizesV;
};

/*-----------------------------------------------------------*/
/*
 *  initST(COMPAR)
 *
 *  RECEBE uma funÃ§Ã£o COMPAR() para comparar chaves.
 *  RETORNA (referÃªncia/ponteiro para) uma tabela de sÃ­mbolos vazia.
 *
 *  Ã‰ esperado que COMPAR() tenha o seguinte comportamento:
 *
 *      COMPAR(key1, key2) retorna um inteiro < 0 se key1 <  key2
 *      COMPAR(key1, key2) retorna 0              se key1 == key2
 *      COMPAR(key1, key2) retorna um inteiro > 0 se key1 >  key2
 * 
 *  TODAS OS OPERAÃ‡Ã•ES da ST criada utilizam a COMPAR() para comparar
 *  chaves.
 * 
 */

BinarySearchST initST(int (*compar)(const void *key1, const void *key2)) {
    BinarySearchST st = emalloc(sizeof(struct binarySearchST));
    st->compar =  compar;
    st->size = 2;
    st->keys = emalloc(sizeof(void *) * st->size);
    st->vals = emalloc(sizeof(void *) * st->size);
    st->sizesK = emalloc(sizeof(size_t) * st->size);
    st->sizesV = emalloc(sizeof(size_t) * st->size);
    st->currentIndex = 0;
    st->n = 0;

    return st;
}

/*-----------------------------------------------------------*/
/*
 *  freeST(ST)
 *
 *  RECEBE uma BinarySearchST  ST e devolve ao sistema toda a memoria 
 *  utilizada por ST.
 *
 */
void freeST(BinarySearchST st) {
    if(st == NULL) {
        printf("FreeST chamada com ST NULL\n");
        return;
    }

    for (int i = 0; i < st->n; i++) {
        // Desalocando ponteiros do vetor
        free((void *)st->keys[i]);
        free((void *)st->vals[i]);
    }

    // Desaolocando vetores e ST 
    free(st->keys);
    free(st->vals);
    free(st->sizesK);
    free(st->sizesV);
    free(st);
}

/*------------------------------------------------------------*/
/*
 * OPERAÃ‡Ã•ES USUAIS: put(), get(), contains(), delete(),
 * size() e isEmpty().
 */

/*-----------------------------------------------------------*/
/*
 *  put(ST, KEY, NKEY, VAL, NVAL)
 * 
 *  RECEBE a tabela de sÃ­mbolos ST e um par KEY-VAL e procura a KEY na ST.
 *
 *     - se VAL Ã© NULL, a entrada da chave KEY Ã© removida da ST  
 *  
 *     - se KEY nao e' encontrada: o par KEY-VAL Ã© inserido na ST
 *
 *     - se KEY e' encontra: o valor correspondente Ã© atualizado
 *
 *  NKEY Ã© o nÃºmero de bytes de KEY e NVAL Ã© o nÃºmero de bytes de NVAL.
 *
 *  Para criar uma copia/clone de KEY Ã© usado o seu nÃºmero de bytes NKEY.
 *  Para criar uma copia/clode de VAL Ã© usado o seu nÃºmero de bytes NVAL.
 *
 */
void put(BinarySearchST st, const void *key, size_t nKey, const void *val, size_t nVal)
{
    if (key == NULL) {
        printf("Put() chamada com key null\n");
        return;
    }

    if (val == NULL) {
        delete(st, key);
        return;
    }

    int i = rank(st, key);

    if (i < st->n && st->compar(st->keys[i], key) == 0) {
        st->sizesV[i] = nVal;
        free((void *)st->vals[i]);
        st->vals[i] = emalloc(nVal);
        memcpy((void *)st->vals[i], val, nVal);
        return;
    }

    if (st->n == st->size) {
        resize(st, 2 * st->size);
    }

    for (int j = st->n; j > i; j--) {
        st->keys[j] = st->keys[j - 1];
        st->vals[j] = st->vals[j - 1];
        st->sizesK[j] = st->sizesK[j - 1];
        st->sizesV[j] = st->sizesV[j - 1];
    }

    st->keys[i] = emalloc(nKey);
    st->vals[i] = emalloc(nVal);
    st->sizesK[i] = nKey;
    st->sizesV[i] = nVal;

    memcpy((void *)st->keys[i], key, nKey);
    memcpy((void *)st->vals[i], val, nVal);

    st->n += 1;
}

/*-----------------------------------------------------------*/
/*
 *  get(ST, KEY)
 *
 *  RECEBE uma tabela de sÃ­mbolos ST e uma chave KEY.
 *
 *     - se KEY estÃ¡ em ST, RETORNA NULL;
 *
 *     - se KEY nÃ£o estÃ¡ em ST, RETORNA uma cÃ³pia/clone do valor
 *       associado a KEY.
 * 
 */
void* get(BinarySearchST st, const void* key){
    if (key == NULL)
        return NULL;

    if (isEmpty(st))
        return NULL;

    int i = rank(st, key); 

    if(i > st->n || i < 0) {
        return NULL;
    }

    if(i < st->n && st->compar(st->keys[i], key) == 0 ){
        // Criando cópia
        void* temp = emalloc(st->sizesV[i]);
        memcpy(temp, (void *)st->vals[i], st->sizesV[i]);

        return temp;
    }

    return NULL;
}

/*-----------------------------------------------------------*/
/* 
 *  CONTAINS(ST, KEY)
 *
 *  RECEBE uma tabela de sÃ­mbolos ST e uma chave KEY.
 * 
 *  RETORNA TRUE se KEY estÃ¡ na ST e FALSE em caso contrÃ¡rio.
 *
 */
Bool contains(BinarySearchST st, const void *key) {
    if (key == NULL) {
        printf("Contains() chamada com key null\n");
        return FALSE;
    }
    void* temp = get(st, key);
    Bool contains;

    if(temp == NULL)
        contains = FALSE;
    else 
        contains = TRUE;

    free(temp);
    return contains;
}

/*-----------------------------------------------------------*/
/* 
 *  DELETE(ST, KEY)
 *
 *  RECEBE uma tabela de sÃ­mbolos ST e uma chave KEY.
 * 
 *  Se KEY estÃ¡ em ST, remove a entrada correspondente a KEY.
 *  Se KEY nÃ£o estÃ¡ em ST, faz nada.
 *
 */
void delete(BinarySearchST st, const void *key) {
    if(!contains(st, key))
        return;

    if (key == NULL){
        printf("Delete() chamada com key NULL\n");
        return;
    }

    if (isEmpty(st))
        return;

    int i = rank(st, key);

    if (i == st->n || st->compar(st->keys[i], key) != 0)
        return;

    free((void *)st->keys[i]);
    free((void *)st->vals[i]);

    for (int j = i; j < st->n - 1; j++) {
        st->sizesK[j] = st->sizesK[j + 1];
        st->sizesV[j] = st->sizesV[j + 1];
        st->keys[j] = st->keys[j + 1];
        st->vals[j] = st->vals[j + 1];
    }
    st->n--;

    // Rediomensiona de n = 1/4 do tamanho do vetor 
    if (st->n > 0 && st->n == st->size / 4)
        resize(st, st->size / 2);
}

/*-----------------------------------------------------------*/
/* 
 *  SIZE(ST)
 *
 *  RECEBE uma tabela de sÃ­mbolos ST.
 * 
 *  RETORNA o nÃºmero de itens (= pares chave-valor) na ST.
 *
 */

int size(BinarySearchST st) {
    return st->n;
}

/*-----------------------------------------------------------*/
/* 
 *  ISEMPTY(ST, KEY)
 *
 *  RECEBE uma tabela de sÃ­mbolos ST.
 * 
 *  RETORNA TRUE se ST estÃ¡ vazia e FALSE em caso contrÃ¡rio.
 *
 */
Bool isEmpty(BinarySearchST st) {
    return st->n == 0;
}

/*------------------------------------------------------------*/
/*
 * OPERAÃ‡Ã•ES PARA TABELAS DE SÃMBOLOS ORDENADAS: 
 * min(), max(), rank(), select(), deleteMin() e deleteMax().
 */

/*-----------------------------------------------------------*/
/*
 *  MIN(ST)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e RETORNA uma cÃ³pia/clone
 *  da menor chave na tabela.
 *
 *  Se ST estÃ¡ vazia RETORNA NULL.
 *
 */
void* min(BinarySearchST st) {
    // Criando cópia
    if(isEmpty(st))
        return NULL;

    void* temp = emalloc(st->sizesK[0]);
    memcpy(temp, (void *)st->keys[0], st->sizesK[0]);
    return temp;
}

/*-----------------------------------------------------------*/
/*
 *  MAX(ST)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e RETORNA uma cÃ³pia/clone
 *  da maior chave na tabela.
 *
 *  Se ST estÃ¡ vazia RETORNA NULL.
 *
 */
void *max(BinarySearchST st){
    if(isEmpty(st))
        return NULL;

    // Criando cópia
    void* temp = emalloc(st->sizesK[st->n - 1]);
    memcpy(temp, (void *)st->keys[st->n - 1], st->sizesK[st->n - 1]);
    return temp;
}

/*-----------------------------------------------------------*/
/*
 *  RANK(ST, KEY)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e uma chave KEY.
 *  RETORNA o nÃºmero de chaves em ST menores que KEY.
 *
 *  Se ST estÃ¡ vazia RETORNA NULL.
 *
 */

int rank(BinarySearchST st, const void *key){
    int lo = 0, hi = st->n - 1;

    // Busca binária
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int cmp = st->compar(key, st->keys[mid]);
        if (cmp < 0)
            hi = mid - 1;
        else if (cmp > 0)
            lo = mid + 1;
        else
            return mid;
    }

    return lo;
}

/*-----------------------------------------------------------*/
/*
 *  SELECT(ST, K)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e um inteiro K >= 0.
 *  RETORNA a (K+1)-Ã©sima menor chave da tabela ST.
 *
 *  Se ST nÃ£o tem K+1 elementos RETORNA NULL.
 *
 */
void *select(BinarySearchST st, int k) {
    if (k < 0 || k >= size(st)) {
        return NULL;
    }

    return (void *)st->keys[k];
}

/*-----------------------------------------------------------*/
/*
 *  deleteMIN(ST)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e remove a entrada correspondente
 *  Ã  menor chave.
 *
 *  Se ST estÃ¡ vazia, faz nada.
 *
 */
void deleteMin(BinarySearchST st) {
    if (isEmpty(st)) {
        return;
    }
    delete(st, st->keys[0]);
}


/*-----------------------------------------------------------*/
/*
 *  deleteMAX(ST)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e remove a entrada correspondente
 *  Ã  maior chave.
 *
 *  Se ST estÃ¡ vazia, faz nada.
 *
 */
void deleteMax(BinarySearchST st) {
    if (isEmpty(st)) {
        return;
    }

    delete(st, st->keys[0]);
}

/*-----------------------------------------------------------*/
/* 
 *  KEYS(ST, INIT)
 * 
 *  RECEBE uma tabela de sÃ­mbolos ST e um Bool INIT.
 *
 *  Se INIT Ã© TRUE, KEYS() RETORNA uma cÃ³pia/clone da menor chave na ST.
 *  Se INIT Ã© FALSE, KEYS() RETORNA a chave sucessora da Ãºltima chave retornada.
 *  Se ST estÃ¡ vazia ou nÃ£o hÃ¡ sucessora da Ãºltima chave retornada, KEYS() RETORNA NULL.
 *
 *  Se entre duas chamadas de KEYS() a ST Ã© alterada, o comportamento Ã© 
 *  indefinido. 
 *  
 */
void* keys(BinarySearchST st, Bool init) {
    if(isEmpty(st) || st->currentIndex == st->n) {
        return NULL;
    }

    if(init == TRUE) {
        st->currentIndex = 0;
        st->currentIndex++;

        return min(st);
    }
    else {
        // Criando cópia
        void* temp = emalloc(st->sizesK[st->currentIndex]);
        memcpy(temp, (void *)st->keys[st->currentIndex], st->sizesK[st->currentIndex]);
        st->currentIndex++;

        return temp;
    }
    
    return NULL;
}

/*-----------------------------------------------------------*/
/*
  Visit each entry on the ST.

  The VISIT function is called, in-order, with each pair key-value in the ST.
  If the VISIT function returns zero, then the iteration stops.

  visitST returns zero if the iteration was stopped by the visit function,
  nonzero otherwise.
*/
int visitST(BinarySearchST st, int (*visit)(const void *key, const void *val)) {
    int i = 0;
    const void* currentKey;
    const void* currentVal;

    int flag = 0;
    while(i < st-> n) {
        currentKey = st->keys[i];
        currentVal = st->vals[i];
        if(visit(currentKey, currentVal) == 0) {
            flag = 1;
            break;
        }
        i++;
    }
    if(flag == 1) {
        return 0;
    }

    return 1;
}

/*------------------------------------------------------------*/
/* 
 * FunÃ§Ãµes administrativas
 */

// Função de redimensionamento
void resize (BinarySearchST st, size_t newSize) {
    size_t sizeVoid = sizeof(void*);
    size_t sizeT = sizeof(size_t);
    const void **newKeys = emalloc(sizeVoid*newSize);
    const void **newVals = emalloc(sizeVoid*newSize);
    size_t *tempKeySizes = emalloc(sizeT*newSize);
    size_t *tempValSizes = emalloc(sizeT*newSize);

    for (int i = 0; i < st->n; i++) {
        newKeys[i] = st->keys[i];
        newVals[i] = st->vals[i];
        tempKeySizes[i] = st->sizesK[i];
        tempValSizes[i] = st->sizesV[i];
    }

    // Desalocando vetores antigos
    free(st->keys);
    free(st->vals);
    free(st->sizesK);
    free(st->sizesV);

    st->keys = newKeys;
    st->vals = newVals;
    st->sizesK = tempKeySizes;
    st->sizesV = tempValSizes;
    st->size = newSize;
}