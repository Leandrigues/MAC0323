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

/* interface para o uso da funcao deste módulo */
#include "redblackst.h"  

#include <stdlib.h>  /* free() */
#include <string.h>  /* memcpy() */
#include "util.h"    /* emalloc(), ecalloc() */

#undef DEBUG
#ifdef DEBUG
#include <stdio.h>   /* printf(): para debug */
#endif

/*
 * CONSTANTES 
 */
#define RED   TRUE
#define BLACK FALSE 
#define COUNT 1

/*----------------------------------------------------------*/
/* 
 * Estrutura Básica da Tabela de Símbolos: 
 * 
 * implementação com árvore rubro-negra
 */



/*----------------------------------------------------------*/
/* 
 * Estrutura de um nó da árvore
 *
 */

struct node {
    void* key;           // key
    void* val;         // associated data
    struct node* left;
    struct node* right;  // links to left and right subtrees
    Bool color;     // color of parent link
    int size;          // subtree count
    size_t keySize;
    size_t valSize;
};

typedef struct node Node;

struct redBlackST {
    Node* root;
    int current;
    int (*compar)(const void *key1, const void *key2);
};


/*------------------------------------------------------------*/
/* 
 *  Protótipos de funções administrativas.
 * 
 *  Entre essa funções estão isRed(), rotateLeft(), rotateRight(),
 *  flipColors(), moveRedLeft(), moveRedRight() e balance().
 * 
 *  Não deixe de implmentar as funções chamadas pela função 
 *  check(): isBST(), isSizeConsistent(), isRankConsistent(),
 *  is23(), isBalanced().
 *
 */

/*---------------------------------------------------------------*/
Bool isRed(Node* x);

static Bool isBST(RedBlackST st);

/*---------------------------------------------------------------*/
static Bool
isSizeConsistent(RedBlackST st);

/*---------------------------------------------------------------*/
static Bool
isRankConsistent(RedBlackST st);

/*---------------------------------------------------------------*/
static Bool
is23(RedBlackST st);

/*---------------------------------------------------------------*/
static Bool
isBalanced(RedBlackST st);

/*-----------------------------------------------------------*/
int sizeNode(Node* h); // Devolve o tamanho da subárvore de um nó
int mathMax(int a, int b); // Adaptação da função Math.max
Node* balance(Node* h);
Node* moveRedRight(Node* h);
Node* moveRedLeft(Node* h);
Node* minNode(Node* x);
Node* deleteMinNode(Node* h);
void imprime(Node* root); // Função de impressão da tabela de simbolos para testes
void freeNode(Node* n); // Libera a memória alocada no node e em seus ponteiros
/*
 *  initST(COMPAR)
 *
 *  RECEBE uma função COMPAR() para comparar chaves.
 *  RETORNA (referência/ponteiro para) uma tabela de símbolos vazia.
 *
 *  É esperado que COMPAR() tenha o seguinte comportamento:
 *
 *      COMPAR(key1, key2) retorna um inteiro < 0 se key1 <  key2
 *      COMPAR(key1, key2) retorna 0              se key1 == key2
 *      COMPAR(key1, key2) retorna um inteiro > 0 se key1 >  key2
 * 
 *  TODAS OS OPERAÇÕES da ST criada utilizam a COMPAR() para comparar
 *  chaves.
 * 
 */

RedBlackST initST(int (*compar)(const void *key1, const void *key2)) {    
    RedBlackST st = malloc(sizeof(struct redBlackST)); 
    st->compar = compar;
    st->root = NULL;
    st->current = 0;
    return st;
}

Node* newNode(const void* key, size_t keySize, const void* val, size_t valSize, Bool color, int size) {
    Node* n = malloc(sizeof(struct node));
    n->key = malloc(keySize);
    n->val = malloc(valSize);

    memcpy(n->key, key, keySize);
    memcpy(n->val, val, valSize);

    n->left = NULL;
    n->right = NULL;
    n->color = color;
    n->size = size;
    n->keySize = keySize;
    n->valSize = valSize;
    return n;
}

/*-----------------------------------------------------------*/
/*
 *  freeST(ST)
 *
 *  RECEBE uma RedBlackST  ST e devolve ao sistema toda a memoria 
 *  utilizada por ST.
 *
 */
void postOrderFree(RedBlackST st, Node* root) {
    if (root != NULL) {
        postOrderFree(st, root->left);
        postOrderFree(st, root->right);

        if(root->key != NULL)
            free(root->key);
        if(root->val != NULL)        
            free(root->val);
        free(root);
    }
}

void freeST(RedBlackST st) {
    if (st == NULL) {
        return;
    }
    postOrderFree(st, st->root);
    free(st);

}


/*------------------------------------------------------------*/
/*
 * OPERAÇÕES USUAIS: put(), get(), contains(), delete(),
 * size() e isEmpty().
 */

/*-----------------------------------------------------------*/
/*
 *  put(ST, KEY, NKEY, VAL, NVAL)
 * 
 *  RECEBE a tabela de símbolos ST e um par KEY-VAL e procura a KEY na ST.
 *
 *     - se VAL é NULL, a entrada da chave KEY é removida da ST  
 *  
 *     - se KEY nao e' encontrada: o par KEY-VAL é inserido na ST
 *
 *     - se KEY e' encontra: o valor correspondente é atualizado
 *
 *  NKEY é o número de bytes de KEY e NVAL é o número de bytes de NVAL.
 *
 *  Para criar uma copia/clone de KEY é usado o seu número de bytes NKEY.
 *  Para criar uma copia/clode de VAL é usado o seu número de bytes NVAL.
 *
 */
void flipColors(Node* h) {
    if (h == NULL || h->left == NULL || h->right == NULL) {
        printf("Deu ruim na flip\n");
        return;
    }
    h->color = !h->color;
    h->left->color = !h->left->color;
    h->right->color = !h->right->color;
}

Node* rotateRight(Node* h) {
    Node* x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = x->right->color;
    x->right->color = RED;
    x->size = h->size;
    h->size = sizeNode(h->left) + sizeNode(h->right) + 1;

    return x;
}

Node* rotateLeft(Node* h) {
    Node* x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = x->left->color;
    x->left->color = RED;
    x->size = h->size;
    h->size = sizeNode(h->left) + sizeNode(h->right) + 1;

    return x;
}


Node* putNode(RedBlackST st, Node* h, const void* key, size_t keySize, const void* val, size_t valSize) {
    if (h == NULL) {
        return newNode(key, keySize, val, valSize, RED, 1);
    }

    int cmp = st->compar(key, h->key);

    if (cmp < 0) {
        h->left = putNode(st, h->left, key, keySize, val, valSize); 
    } 


    else if (cmp > 0) {
        h->right = putNode(st, h->right, key, keySize, val, valSize); 
    }

    else{
        h->valSize = valSize;
        free(h->val);
        h->val = malloc(valSize);
        memcpy(h->val, val, valSize);
    }

    if (isRed(h->right) && !isRed(h->left)){
        h = rotateLeft(h);
    }

    if (isRed(h->left)  &&  isRed(h->left->left)) {
        h = rotateRight(h);
    }

    if (isRed(h->left)  &&  isRed(h->right)) {
        flipColors(h);
    }

    h->size = sizeNode(h->left) + sizeNode(h->right) + 1;
    
    return h;
}

void put(RedBlackST st, const void *key, size_t keySize, const void *val, size_t valSize) {
    if (key == NULL){
        printf("Key NULL em put\n");
        return;
    }

    if (val == NULL) {
        printf("Val NULL\n");
        delete(st, key);
        return;
    }
    st->root = putNode(st, st->root, key, keySize, val, valSize);
    
    st->root->color = BLACK;

}      


/*-----------------------------------------------------------*/
/*
 *  get(ST, KEY)
 *
 *  RECEBE uma tabela de símbolos ST e uma chave KEY.
 *
 *     - se KEY está em ST, RETORNA NULL;
 *
 *     - se KEY não está em ST, RETORNA uma cópia/clone do valor
 *       associado a KEY.
 * 
 */

void* getNode(RedBlackST st, Node* x, const void* key) {

    while (x != NULL) {
        int cmp = st->compar(key, x->key);
        if (cmp < 0) x = x->left;
        else if (cmp > 0) x = x->right;
        else {
            void *val = emalloc(x->valSize);
            memcpy(val, x->val, x->valSize);
            return val;
        }
    }

    return NULL;
}

void* get(RedBlackST st, const void* key) {
    if (key == NULL) {
        printf("Key NULL em get()\n");
        return NULL;
    }

    return getNode(st, st->root, key);

}


/*-----------------------------------------------------------*/
/* 
 *  CONTAINS(ST, KEY)
 *
 *  RECEBE uma tabela de símbolos ST e uma chave KEY.
 * 
 *  RETORNA TRUE se KEY está na ST e FALSE em caso contrário.
 *
 */
Bool contains(RedBlackST st, const void *key) {
    void* temp = get(st, key);
    if (temp == NULL)
        return FALSE;
    free(temp);
    return TRUE;
}

/*-----------------------------------------------------------*/
/* 
 *  DELETE(ST, KEY)
 *
 *  RECEBE uma tabela de símbolos ST e uma chave KEY.
 * 
 *  Se KEY está em ST, remove a entrada correspondente a KEY.
 *  Se KEY não está em ST, faz nada.
 *
 */

// delete the key-value pair with the given key rooted at h
Node* deleteNode(RedBlackST st, Node* h, const void* key) { 
    if (st->compar(key, h->key) < 0)  {
        
        if (!isRed(h->left) && !isRed(h->left->left))
            h = moveRedLeft(h);
        h->left = deleteNode(st, h->left, key);
    }
    else {
        if (isRed(h->left))
            h = rotateRight(h);

        if (st->compar(key, h->key) == 0 && (h->right == NULL)) {
            freeNode(h);
            return NULL;
        }

        if (!isRed(h->right) && !isRed(h->right->left))
            h = moveRedRight(h);
            
        if (st->compar(key, h->key) == 0) {
            Node* x = minNode(h->right);
            h->keySize = x->keySize;
            h->valSize = x->valSize;

            free(h->key);
            free(h->val);

            h->key = malloc(h->keySize);
            h->val = malloc(h->valSize);

            memcpy((void*)h->key, x->key, h->keySize);
            memcpy((void*)h->val, x->val, h->valSize);

            h->right = deleteMinNode(h->right);
        }
        else h->right = deleteNode(st, h->right, key);
    }

    return balance(h);
}

void delete(RedBlackST st, const void *key) {
    if (key == NULL){
        printf("Key NULL em DELETE()\n");
        return;
    }

    if (!contains(st, (void*)key))
        return;

    if (!isRed(st->root->left) && !isRed(st->root->right))
        st->root->color = RED;


    st->root = deleteNode(st, st->root, key);

    if (!isEmpty(st)) st->root->color = BLACK;
}


/*-----------------------------------------------------------*/
/* 
 *  SIZE(ST)
 *
 *  RECEBE uma tabela de símbolos ST.
 * 
 *  RETORNA o número de itens (= pares chave-valor) na ST.
 *
 */
int size(RedBlackST st) {
    return st->root->size;
}

/*-----------------------------------------------------------*/
/* 
 *  ISEMPTY(ST, KEY)
 *
 *  RECEBE uma tabela de símbolos ST.
 * 
 *  RETORNA TRUE se ST está vazia e FALSE em caso contrário.
 *
 */
Bool isEmpty(RedBlackST st) {
    return st->root == NULL;
}

/*------------------------------------------------------------*/
/*
 * OPERAÇÕES PARA TABELAS DE SÍMBOLOS ORDENADAS: 
 * min(), max(), rank(), select(), deleteMin() e deleteMax().
 */

/*-----------------------------------------------------------*/
/*
 *  MIN(ST)
 * 
 *  RECEBE uma tabela de símbolos ST e RETORNA uma cópia/clone
 *  da menor chave na tabela.
 *
 *  Se ST está vazia RETORNA NULL.
 *
 */
Node* minNode(Node* x) { 
    if (x->left == NULL) {
        return x;
    }
    else 
        return minNode(x->left); 
} 

void* min(RedBlackST st) {
    if (isEmpty(st)){
        printf("ST vazia em min()\n");
        return NULL;
    }

    Node* auxNode = minNode(st->root); 
    void* temp = malloc(auxNode->keySize);
    memcpy(temp, auxNode->key, auxNode->keySize);
    
    return temp;
} 


/*-----------------------------------------------------------*/
/*
 *  MAX(ST)
 * 
 *  RECEBE uma tabela de símbolos ST e RETORNA uma cópia/clone
 *  da maior chave na tabela.
 *
 *  Se ST está vazia RETORNA NULL.
 *
 */
Node* maxNode(Node* x) { 
    if (x->right == NULL) {
        return x; 
    }
    else 
        return maxNode(x->right); 
} 

void* max(RedBlackST st) {
    if (isEmpty(st)){
        printf("ST vazia em max()\n");
        return NULL;
    }

    Node* auxNode = maxNode(st->root); 
    void* temp = malloc(auxNode->keySize);
    memcpy(temp, auxNode->key, auxNode->keySize);

    return temp;
} 

/*-----------------------------------------------------------*/
/*
 *  RANK(ST, KEY)
 * 
 *  RECEBE uma tabela de símbolos ST e uma chave KEY.
 *  RETORNA o número de chaves em ST menores que KEY.
 *
 *  Se ST está vazia RETORNA NULL.
 *
 */
int rankNode(RedBlackST st, const void* key, Node* x) {
    if (x == NULL)
        return 0; 

    int cmp = st->compar(key, x->key); 

    if (cmp < 0)
        return rankNode(st, key, x->left); 
    else if (cmp > 0) 
        return 1 + sizeNode(x->left) + rankNode(st, key, x->right); 
    else 
        return sizeNode(x->left); 

} 


int rank(RedBlackST st, const void* key) {
    if (key == NULL) {
        printf("Key NULL em rank()");
        return -1;
    }

    return rankNode(st, key, st->root);
} 
/*-----------------------------------------------------------*/
/*
 *  SELECT(ST, K)
 * 
 *  RECEBE uma tabela de símbolos ST e um inteiro K >= 0.
 *  RETORNA a (K+1)-ésima menor chave da tabela ST.
 *
 *  Se ST não tem K+1 elementos RETORNA NULL.
 *
 */
Node* selectNode(Node* x, int k) {
    int t = sizeNode(x->left); 
    
    if (t > k) 
        return selectNode(x->left,  k); 
    else if (t < k)
        return selectNode(x->right, k - t - 1); 
    else 
        return x; 
} 


void* select(RedBlackST st, int k) {
    if (k < 0 || k >= sizeNode(st->root)) {
        printf("k < 0 ou k >= size em select()\n");
        return NULL;
    }

    Node* x = selectNode(st->root, k);

    return x->key;
}


/*-----------------------------------------------------------*/
/*
 *  deleteMIN(ST)
 * 
 *  RECEBE uma tabela de símbolos ST e remove a entrada correspondente
 *  à menor chave.
 *
 *  Se ST está vazia, faz nada.
 *
 */
Node* deleteMinNode(Node* h) { 
    if (h->left == NULL) {
        freeNode(h);
        return NULL;
    }

    if (!isRed(h->left) && !isRed(h->left->left))
        h = moveRedLeft(h);

    h->left = deleteMinNode(h->left);

    return balance(h);
}


void deleteMin(RedBlackST st) {
    if (isEmpty(st)){
        printf("Tabela vazia em deleteMin()\n");
        return;
    }

    if (!isRed(st->root->left) && !isRed(st->root->right))
        st->root->color = RED;

    st->root = deleteMinNode(st->root);

    if (!isEmpty(st))
        st->root->color = BLACK;
}


/*-----------------------------------------------------------*/
/*
 *  deleteMAX(ST)
 * 
 *  RECEBE uma tabela de símbolos ST e remove a entrada correspondente
 *  à maior chave.
 *
 *  Se ST está vazia, faz nada.
 *
 */
// delete the key-value pair with the maximum key rooted at h
Node* deleteMaxNode(Node* h) { 
    if (isRed(h->left)) {
        h = rotateRight(h);
    }

    if (h->right == NULL) {
        freeNode(h);
        return NULL;
    }

    if (!isRed(h->right) && !isRed(h->right->left))
        h = moveRedRight(h);

    h->right = deleteMaxNode(h->right);

    return balance(h);
}

void deleteMax(RedBlackST st) {
    if (isEmpty(st)) {
        printf("Tabela vazia em deleteMax()\n");
        return;
    }

    if (!isRed(st->root->left) && !isRed(st->root->right))
        st->root->color = RED;

    st->root = deleteMaxNode(st->root);

    if (!isEmpty(st))
        st->root->color = BLACK;
}


/*-----------------------------------------------------------*/
/* 
 *  KEYS(ST, INIT)
 * 
 *  RECEBE uma tabela de símbolos ST e um Bool INIT.
 *
 *  Se INIT é TRUE, KEYS() RETORNA uma cópia/clone da menor chave na ST.
 *  Se INIT é FALSE, KEYS() RETORNA a chave sucessora da última chave retornada.
 *  Se ST está vazia ou não há sucessora da última chave retornada, KEYS() retorna NULL.
 *
 *  Se entre duas chamadas de KEYS() a ST é alterada, o comportamento é 
 *  indefinido. 
 *  
 */
void * keys(RedBlackST st, Bool init) {
    if (init == TRUE) {
        Node* minKey = minNode(st->root);
        void* temp = malloc(minKey->keySize);
        memcpy(temp, minKey->key, minKey->keySize);
        st->current = 1;
        return temp;
    }

    else if (st->current == size(st)) {
        return NULL;
    }

    Node* selected = selectNode(st->root, st->current);
    void* temp = malloc(selected->keySize);
    memcpy(temp, selected->key, selected->keySize);
    st->current++;

    return temp;

}



/*------------------------------------------------------------*/
/* 
 * Funções administrativas
 */

/***************************************************************************
 *  Utility Nodes.
 ***************************************************************************/

/*
 * HEIGHT(ST)
 * 
 * RECEBE uma RedBlackST e RETORNA a sua altura. 
 * Uma BST com apenas um nó tem altura zero.
 * 
 */

int heightNode(Node* x) {
    if (x == NULL) 
        return -1;

    return 1 + mathMax(heightNode(x->left), heightNode(x->right));
}

int height(RedBlackST st) {
    return heightNode(st->root);
}

Bool isRed(Node* x) {
    if (x == NULL) {
        return FALSE;
    }

    return x->color == RED;
}

int sizeNode(Node* h) {
    if (h == NULL) {
        return 0;
    }

    return h->size;
}

int mathMax(int a, int b) {
    if (a < b) 
        return b;

    return a;
}

Node* balance(Node* h) {
    if (isRed(h->right))
        h = rotateLeft(h);

    if (isRed(h->left) && isRed(h->left->left))
        h = rotateRight(h);

    if (isRed(h->left) && isRed(h->right))
        flipColors(h);

    h->size = sizeNode(h->left) + sizeNode(h->right) + 1;

    return h;
}


Node* moveRedLeft(Node* h) {
    flipColors(h);

    if (isRed(h->right->left)) { 
        h->right = rotateRight(h->right);
        h = rotateLeft(h);
        flipColors(h);
    }

    return h;
}

Node* moveRedRight(Node* h) {
    flipColors(h);

    if (isRed(h->left->left)) { 
        h = rotateRight(h);
        flipColors(h);
    }

    return h;
}

void imprime(Node* root) {
    if (root != NULL) {
        imprime(root->left);
        printf("%s: %d\n", (char*)root->key, *(int *)root->val);
    }
}

void freeNode(Node* n) {
    if(n != NULL) {
        free(n->key);
        free(n->val);
        free(n);
    }
}


/***************************************************************************
 *  Check integrity of red-black tree data structure.
 ***************************************************************************/

/*
 * CHECK(ST)
 *
 * RECEBE uma RedBlackST ST e RETORNA TRUE se não encontrar algum
 * problema de ordem ou estrutural. Em caso contrário RETORNA 
 * FALSE.
 * 
 */
Bool
check(RedBlackST st)
{
    if (!isBST(st))            ERROR("check(): not in symmetric order");
    if (!isSizeConsistent(st)) ERROR("check(): subtree counts not consistent");
    if (!isRankConsistent(st)) ERROR("check(): ranks not consistent");
    if (!is23(st))             ERROR("check(): not a 2-3 tree");
    if (!isBalanced(st))       ERROR("check(): not balanced");
    return isBST(st) && isSizeConsistent(st) && isRankConsistent(st) && is23(st) && isBalanced(st);
}


/* 
 * ISBST(ST)
 * 
 * RECEBE uma RedBlackST ST.
 * RETORNA TRUE se a árvore é uma BST.
 * 
 */

Bool isBSTBool(RedBlackST st, Node* x, const void* min, const void* max) {
    if (x == NULL)
        return TRUE;

    if (min != NULL && st->compar(x->key, min) <= 0)
        return FALSE;

    if (max != NULL && st->compar(x->key, max) >= 0){
        printf("Max %s\n", (char*)max);
        return FALSE;
    }

    return isBSTBool(st, x->left, min, x->key) && isBSTBool(st, x->right, x->key, max);
} 

static Bool isBST(RedBlackST st) {
    return isBSTBool(st, st->root, NULL, NULL);
}
/* 
 *  ISSIZECONSISTENT(ST) 
 *
 *  RECEBE uma RedBlackST ST e RETORNA TRUE se para cada nó h
 *  vale que size(h) = 1 + size(h->left) + size(h->right) e 
 *  FALSE em caso contrário.
 */

Bool isSizeConsistentBool(Node* x) {
    if (x == NULL) 
        return TRUE;
    if (x->size != sizeNode(x->left) + sizeNode(x->right) + 1)
        return FALSE;
        
    return isSizeConsistentBool(x->left) && isSizeConsistentBool(x->right);
} 

static Bool isSizeConsistent(RedBlackST st) {
    return isSizeConsistentBool(st->root);
}

/* 
 *  ISRANKCONSISTENT(ST)
 *
 *  RECEBE uma RedBlackST ST e RETORNA TRUE se seus rank() e
 *  select() são consistentes.
 */  
/* check that ranks are consistent */
static Bool isRankConsistent(RedBlackST st) {
    for (int i = 0; i < st->root->size; i++)
        if (i != rank(st, select(st, i))) return FALSE;

    Bool init = TRUE;
    for(int j = 0; j < st->root->size; j++){
        void* key = keys(st, init);
        if (st->compar(key, select(st, rank(st, key))) != 0){
            free(key);
            return FALSE;
        }
        free(key);
        init = FALSE;
    }
    return TRUE;
}

/* 
 *  IS23(ST)
 *
 *  RECEBE uma RedBlackST ST e RETORNA FALSE se há algum link RED
 *  para a direta ou se ha dois links para esquerda seguidos RED 
 *  Em caso contrário RETORNA TRUE (= a ST representa uma árvore 2-3). 
 */
Bool is23Bool(RedBlackST st, Node* x) {
        if (x == NULL)
            return TRUE;
        if (isRed(x->right))
            return FALSE;
        if (x != st->root && isRed(x) && isRed(x->left))
            return FALSE;

        return is23Bool(st, x->left) && is23Bool(st, x->right);
} 

static Bool is23(RedBlackST st) {
    return is23Bool(st, st->root);
}


/* 
 *  ISBALANCED(ST) 
 * 
 *  RECEBE uma RedBlackST ST e RETORNA TRUE se st satisfaz
 *  balanceamento negro perfeiro.
 */ 

// does every path from the root to a leaf have the given number of black links?
Bool isBalancedBool(RedBlackST st, Node* x, int black) {
    if (x == NULL)
        return black == 0;

    if (!isRed(x))
        black--;

    return isBalancedBool(st, x->left, black) && isBalancedBool(st, x->right, black);
} 

static Bool isBalanced(RedBlackST st) { 
    int black = 0;     // number of black links on path from root to min
    Node* x = st->root;

    while (x != NULL) {
        if (!isRed(x)) black++;
        x = x->left;
    }

    return isBalancedBool(st, st->root, black);
}