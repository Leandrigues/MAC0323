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
        O meu método STPerms() foi baseado no código colocado pelo professor Coelho no PACA.

    Se for o caso, descreva a seguir 'bugs' e limitações do seu programa:
        Meu código demora muito mais que os exemplos do enunciado.

****************************************************************/

/******************************************************************************
 *  Compilation:  javac-algs4 STPerms.java
 *  Execution:    java STPerms n s t opcao
 *  
 *  Enumera todas as (s,t)-permutações das n primeiras letras do alfabeto.
 *  As permutações devem ser exibidas em ordem lexicográfica.
 *  Sobre o papel da opcao, leia o enunciado do EP.
 *
 *  % java STPerms 4 2 2 0
 *  badc
 *  bdac
 *  cadb
 *  cdab
 *  4
 *  % java STPerms 4 2 2 1
 *  4
 *  % java STPerms 4 2 2 2
 *  badc
 *  bdac
 *  cadb
 *  cdab
 *  4
 *   
 ******************************************************************************/

import edu.princeton.cs.algs4.StdOut;

public class STPerms {
    private static int contador = 0;

    public static int maiorSubS(String permutacao, int n, int s) {
        // Função que verifica se as subsequencias crescentes tem tamanho menor ou igual a s. 
        int j;
        int i;
        int maiorAtual[] = new int[n]; 
        int maior = 0; 

        for (i = 0; i < n; i++) 
            maiorAtual[i] = 1; 

        for (i = 1; i < n; i++) 
            for (j = 0; j < i; j++) 
                if (permutacao.charAt(i) > permutacao.charAt(j) && maiorAtual[i] < maiorAtual[j] + 1){
                    maiorAtual[i] = maiorAtual[j] + 1; 
                    if (maiorAtual[i] > s) {
                        return -1;
                    }
                }

        for (i = 0; i < n; i++) 
            if (maior < maiorAtual[i]) 
                maior = maiorAtual[i]; 

        return maior; 
    } 

    public static int maiorSubT(String permutacao, int n, int t) { 
        // Função que verifica se as subsequencias decrescentes tem tamanho menor ou igual a t. 
        int j;
        int i;
        int maiorAtual[] = new int[n]; 
        int maior = 0; 

        for (i = 0; i < n; i++) 
            maiorAtual[i] = 1; 

        for (i = 1; i < n; i++) 
            for (j = 0; j < i; j++) 
                if (permutacao.charAt(i) < permutacao.charAt(j) && maiorAtual[i] < maiorAtual[j] + 1){

                    maiorAtual[i] = maiorAtual[j] + 1; 
                    if (maiorAtual[i] > t) {
                        return -1;
                    }
                } 

        for (i = 0; i < n; i++) 
            if (maior < maiorAtual[i]) 
                maior = maiorAtual[i]; 
  
        return maior; 
    } 
  
    public static void STPerms(String permutacao, String seq, int s, int t, int opcao) {
        // Função que gera as permutações e as imprime caso sejam (s,t)-permutações.
        int n = seq.length();   
        if (n == 0) {
            int tamS = maiorSubS(permutacao, permutacao.length(), s);
            int tamT = maiorSubT(permutacao, permutacao.length(), t);

            if(tamS <= s && tamS != -1 && tamT != -1 && tamT <= t){
                if(opcao != 1) 
                    StdOut.printf("%s\n",permutacao);  
                contador++;
            } 
        }

            for (int i = 0; i < n; i++) {
                STPerms(permutacao + seq.charAt(i), seq.substring(0, i) + seq.substring(i+1, n), s, t, opcao);
            }
    }

    public static void main(String[] args) {
        // Função principal
        // opções default
        int s = 1;
        int t = 1;
        int n = 1;
        int opcao = 2;
        String alfabeto = "abcdefghijklmnopqrstuvwxyz";
        String permutacao = "";

        if(args.length > 0) 
            n = Integer.parseInt(args[0]);
        if(args.length > 1) 
            s = Integer.parseInt(args[1]);
        if(args.length > 2) 
            t = Integer.parseInt(args[2]);
        if(args.length > 3) 
            opcao = Integer.parseInt(args[3]);
        
        String sequencia = alfabeto.substring(0, n);
        STPerms(permutacao, sequencia, s, t, opcao);

        if(opcao != 0) 
            StdOut.printf("%d\n",contador);  

        return;
    }
}

