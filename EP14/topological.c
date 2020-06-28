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

    Descrição de ajuda ou indicação de fonte:
        - A queue utilizada foi feita na matéria de MAC0121 (eu mesmo implementei)
        
    Se for o caso, descreva a seguir 'bugs' e limitações do seu programa:
****************************************************************/

/*
 * MAC0323 Algoritmos e Estruturas de Dados II
 * 
 * ADT Topological é uma "representação topológica" de digrafo.
 * Esta implementação usa ADT Digraph do EP13.
 *  
 * Busque inspiração em: 
 *
 *   https://algs4.cs.princeton.edu/42digraph/
 *   https://algs4.cs.princeton.edu/42digraph/DepthFirstOrder.java
 *   https://algs4.cs.princeton.edu/42digraph/Topological.java
 *   https://algs4.cs.princeton.edu/42digraph/DirectedCycle.java
 * 
 * TOPOLOGICAL
 *
 * Topological é uma ¨representação topológica" de um dado digrafo.
 * 
 * As principais operações são: 
 *
 *      - hasCycle(): indica se o digrafo tem um ciclo (DirectedCycle.java)
 *      - isDag(): indica se o digrafo é acyclico (Topological.java)
 *
 *      - pre(): retorna a numeração pré-ordem de um vértice em relação a uma dfs 
 *               (DepthFirstOrder.java)
 *      - pos(): retorna a numareção pós-ordem de um vértice em relação a uma dfs
 *               (DepthFirstOrder.java)
 *      - rank(): retorna a numeração topológica de um vértice (Topological.java)
 * 
 *      - preorder(): itera sobre todos os vértices do digrafo em pré-ordem
 *                    (em relação a uma dfs, DepthFirstOrder.java)
 *      - postorder(): itera sobre todos os vértices do digrafo em pós-ordem
 *                    (em relação a uma dfs, ordenação topologica reversa, 
 *                     DepthFirstOrder.java)
 *      - order(): itera sobre todos os vértices do digrafo em ordem  
 *                 topologica (Topological.java)
 *      - cycle(): itera sobre os vértices de um ciclo (DirectedCycle.java)
 *
 * O construtor e "destrutor" da classe consomem tempo linear..
 *
 * Cada chama das demais operações consome tempo constante.
 *
 * O espaço gasto por esta ADT é proporcional ao número de vértices V do digrafo.
 * 
 * Para documentação adicional, ver 
 * https://algs4.cs.princeton.edu/42digraph, Seção 4.2 de
 * Algorithms, 4th Edition por Robert Sedgewick e Kevin Wayne.
 *
 */

/* interface para o uso da funcao deste módulo */
#include "topological.h"

#include "digraph.h" /* Digraph, vDigraph(), eDigraph(), adj(), ... */
#include "bag.h"     /* add() e itens() */
#include "util.h"    /* emalloc(), ecalloc(), ERRO(), AVISO() */

#include <stdlib.h>  /* free() */

#undef DEBUG
#ifdef DEBUG
#include <stdio.h>   /* printf(): para debugging */
#endif



typedef struct {
   vertex *v;
   int max;
   int start;
   int end;
} queue;

typedef queue *Queue;

/*----------------------------------------------------------*/
/* 
 * Estrutura básica de um Topological
 * 
 */
struct topological {
    Bool* marked;
    Bool* onStack;
    Bool hasCycle;
    vertex* edgeTo;
    vertex* pre;
    vertex* post;
    vertex preCounter;
    vertex postCounter;
    Queue preOrder;
    Queue postOrder;
    Bag order;
    Bag cycle;
    int currentPost;
    int currentPre;
    int* rank;
    int last;
};


/*------------------------------------------------------------*/
/* 
 * Protótipos de funções administrativas: tem modificador 'static'
 * 
 */

void dfsDirected(Topological t, Digraph G, vertex v);
void dfsDepth(Topological t, Digraph G, vertex v);
Topological DepthFirstOrder(Digraph G);
Topological DirectedCycle(Digraph G);
Queue newQueue(int tam);
void resize(Queue F);
void enqueue(Queue F, vertex x);
int isEmptyQueue(Queue F);
void dequeue(Queue F);
vertex firstQueue(Queue F);
void freeQueue(Queue F);
void imprimeFila(Queue f);

/*-----------------------------------------------------------*/
/*
 *  newTopologica(G)
 *
 *  RECEBE um digrafo G.
 *  RETORNA uma representação topológica de G.
 * 
 */


Topological newTopological(Digraph G) {
    Topological t = malloc(sizeof(struct topological));

    // Contadores e auxiliares de iterador
    t->preCounter = t->postCounter = 
    t->currentPost = t->currentPre = 0;

    int V = vDigraph(G);

    // Arrays
    t->edgeTo = malloc(sizeof(vertex) * V);
    t->pre = malloc(sizeof(vertex) * V);
    t->post = malloc(sizeof(vertex) * V);
    t->rank = malloc(sizeof(vertex) * V);
    t->marked = malloc(sizeof(vertex) * V);
    t->onStack = malloc(sizeof(vertex) * V);

    // Filas
    t->preOrder = newQueue(V + 1);
    t->postOrder = newQueue(V + 1);

    t->cycle = NULL;
    t->order = NULL;

    // Inicializando arrays
    for(int i = 0; i < V; i++) {
        t->pre[i] = 0;
        t->post[i] = 0;
        t->edgeTo[i] = -1;
        t->marked[i] = FALSE;
        t->onStack[i] = FALSE;
        t->rank[i] = 0;
    } 

    // DFS do DirectedCycle
    for (int v = 0; v < vDigraph(G); v++) {
        if (!t->marked[v] && t->cycle == NULL){
            dfsDirected(t, G, v);
        }
    }

    // Reiniciando o vetor marked
    for(int j = 0; j < V; j++) {
        t->marked[j] = FALSE;
    }

    // DFS do DepthFirstOrder
    for (int v = 0; v < vDigraph(G); v++){
        if (!t->marked[v]) {
            dfsDepth(t, G, v);
        }
    }

    int rank = V - 1;
    t->order = newBag();

    for(vertex v = postorder(t, TRUE); v != -1; v = postorder(t, FALSE)){
        add(t->order, v);
        t->rank[v] = rank--;
    }
    // Reiniciando iterador
    t->currentPost = 0;    

    return t;
}

/*-----------------------------------------------------------*/
/*
 *  freeTopological(TS)
 *
 *  RECEBE uma representação topologica TS.
 *  DEVOLVE ao sistema toda a memória usada por TS.
 *
 */
void freeTopological(Topological ts) {
    free(ts->edgeTo);
    free(ts->pre);
    free(ts->post);
    free(ts->rank);
    free(ts->marked);
    free(ts->onStack);
    freeQueue(ts->preOrder);
    freeQueue(ts->postOrder);
    if(ts->cycle != NULL)
        freeBag(ts->cycle);
    if(ts->order != NULL)
        freeBag(ts->order);

    free(ts);
}    

/*------------------------------------------------------------*/
/*
 *  OPERAÇÕES: 
 *
 */

/*-----------------------------------------------------------*/
/* 
 *  HASCYCLE(TS)
 *
 *  RECEBE uma representação topológica TS de um digrafo;
 *  RETORNA TRUE seu o digrafo possui um ciclo e FALSE em caso 
 *  contrário.
 *
 */
Bool hasCycle(Topological ts) {
    return ts->cycle != NULL;
}

/*-----------------------------------------------------------*/
/* 
 *  ISDAG(TS)
 *
 *  RECEBE um representação topológica TS de um digrafo.
 *  RETORNA TRUE se o digrafo for um DAG e FALSE em caso 
 *  contrário.
 *
 */
Bool isDag(Topological ts) {
    if(hasCycle(ts))
        return FALSE;

    return ts->order != NULL;
}

/*-----------------------------------------------------------*/
/* 
 *  PRE(TS, V)
 *
 *  RECEBE uma representação topológica TS de um digrafo e um 
 *  vértice V.
 *  RETORNA a numeração pré-ordem de V em TS.
 *
 */
int pre(Topological ts, vertex v) {
    return ts->pre[v];
}

/*-----------------------------------------------------------*/
/* 
 *  POST(TS, V)
 *
 *  RECEBE uma representação topológica TS de um digrafo e um 
 *  vértice V.
 *  RETORNA a numeração pós-ordem de V em TS.
 *
 */
int post(Topological ts, vertex v) {
    return ts->post[v];
}

/*-----------------------------------------------------------*/
/* 
 *  RANK(TS, V)
 *
 *  RECEBE uma representação topológica TS de um digrafo e um 
 *  vértice V.
 *  RETORNA a posição de V na ordenação topológica em TS;
 *  retorna -1 se o digrafo não for um DAG.
 *
 */
int rank(Topological ts, vertex v) {
    if(isDag(ts))
        return ts->rank[v];

    return -1;
}

/*-----------------------------------------------------------*/
/* 
 *  PREORDER(TS, INIT)
 * 
 *  RECEBE uma representação topológica TS de um digrafo e um 
 *  Bool INIT.
 *
 *  Se INIT é TRUE,  PREORDER() RETORNA o primeiro vértice na ordenação pré-ordem de TS.
 *  Se INIT é FALSE, PREORDER() RETORNA o vértice sucessor do último vértice retornado
 *                   na ordenação pré-ordem de TS; se todos os vértices já foram retornados, 
 *                   a função retorna -1.
 */
vertex preorder(Topological ts, Bool init) {
    if(isEmptyQueue(ts->preOrder) || (ts->preOrder->start + ts->currentPre == ts->preOrder->end)) {
        return -1;
    }

    if(init) {
        vertex vert = firstQueue(ts->preOrder);
        ts->currentPre = ts->preOrder->start + 1;
        return vert;
    }
    else {
        vertex vert = ts->preOrder->v[ts->preOrder->start + ts->currentPre];
        ts->currentPre = ts->preOrder->start + ts->currentPre + 1;
        return vert;

    }
}
/*-----------------------------------------------------------*/
/* 
 *  POSTORDER(TS, INIT)
 * 
 *  RECEBE uma representação topológica TS de um digrafo e um 
 *  Bool INIT.
 *
 *  Se INIT é TRUE,  POSTORDER() RETORNA o primeiro vértice na ordenação pós-ordem de TS.
 *  Se INIT é FALSE, POSTORDER() RETORNA o vértice sucessor do último vértice retornado
 *                   na ordenação pós-ordem de TS; se todos os vértices já foram retornados, 
 *                   a função retorna -1.
 */
vertex postorder(Topological ts, Bool init) {
    if(isEmptyQueue(ts->postOrder) || (ts->postOrder->start + ts->currentPost == ts->postOrder->end))
        return -1;


    if(init) {
        vertex vert = firstQueue(ts->postOrder);
        ts->currentPost = ts->postOrder->start + 1;
        return vert;
    }
    else {
        vertex vert = ts->postOrder->v[ts->postOrder->start + ts->currentPost];
        ts->currentPost = ts->postOrder->start + ts->currentPost + 1;
        return vert;

    }
}

/*-----------------------------------------------------------*/
/* 
 *  ORDER(TS, INIT)
 * 
 *  RECEBE uma representação topológica TS de um digrafo e um Bool INIT.
 *
 *  Se INIT é TRUE,  ORDER() RETORNA o primeiro vértice na ordenação topológica 
 *                   de TS.
 *  Se INIT é FALSE, ORDER() RETORNA o vértice sucessor do último vértice retornado
 *                   na ordenação topológica de TS; se todos os vértices já foram 
 *                   retornados, a função retorna -1.
 *
 *  Se o digrafo _não_ é um DAG, ORDER() RETORNA -1.
 */
vertex order(Topological ts, Bool init) {
    if(!isDag(ts))
        return -1;

    return itens(ts->order, init);
}

/*-----------------------------------------------------------*/
/* 
 *  CYCLE(TS, INIT)
 * 
 *  RECEBE uma representação topológica TS de um digrafo e um Bool INIT.
 *
 *  Se INIT é TRUE,  CYCLE() RETORNA um vértice em um ciclo do digrafo.
 *  Se INIT é FALSE, CYCLE() RETORNA o vértice  no ciclo que é sucessor do 
 *                   último vértice retornado; se todos os vértices no ciclo já 
 *                   foram retornados, a função retorna -1.
 *
 *  Se o digrafo é um DAG, CYCLE() RETORNA -1.
 *
 */
vertex cycle(Topological ts, Bool init){
    if(isDag(ts))
        return -1;

    vertex aux = itens(ts->cycle, init);
    return aux;
}


/*------------------------------------------------------------*/
/* 
 * Implementaçao de funções administrativas: têm o modificador 
 * static.
 */
void dfsDepth(Topological t, Digraph G, int v) {
    t->marked[v] = TRUE;
    t->pre[v] = t->preCounter++;
    enqueue(t->preOrder, v);

    for(int w = adj(G, v, TRUE); w != -1; w = adj(G, v, FALSE)){
        if (!t->marked[w]) {
            dfsDepth(t, G, w);
        }
    }

    enqueue(t->postOrder, v);
    t->post[v] = t->postCounter++;
}

void dfsDirected(Topological t, Digraph G, vertex v) {
    t->onStack[v] = TRUE;
    t->marked[v] = TRUE;

    for (int w =  adj(G, v, TRUE); w != -1; w = adj(G, v, FALSE)) {
        if (t->cycle != NULL){
            return;
        }
            

        else if (!t->marked[w]) {
            t->edgeTo[w] = v;
            dfsDirected(t, G, w);
        }

        else if (t->onStack[w]) {
            t->cycle = newBag();
            for (int x = v; x != w; x = t->edgeTo[x]) {
                add(t->cycle, x);
            }

            add(t->cycle, w);
        }
    }
    t->onStack[v] = FALSE;
}
    
Queue newQueue(int tam) {
    Queue F = malloc(sizeof(queue));
    F->start = 0;
    F->end = 0;
    F->max = tam;
    F->v = malloc(tam * sizeof(vertex));

    return F;
}

void resize(Queue F) {
    F->max = 2 * F->max;
    vertex *aux = malloc(2 * sizeof(vertex) * F->max);

    for(int i = F->start; i < F->end; i++) {
        aux[i] = F->v[i];
    }

    free(F->v);
    F->v = aux;
}

void enqueue(Queue F, vertex x) {
   if ((F->end + 1) % F->max == F->start){
       resize(F);
   }

   F->v[F->end] = x;
   F->end = (F->end + 1) % F->max; 
}

int isEmptyQueue(Queue F) {
   return (F->start == F->end);
}

void dequeue(Queue F) {
    F->start = (F->start + 1) % F->max;
}

vertex firstQueue(Queue F) {
   if (isEmptyQueue(F)) {
       return -1;
   }
   return F->v[F->start];
}

void freeQueue(Queue F) {
    free(F->v);
    free(F);
}