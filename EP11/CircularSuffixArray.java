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
import edu.princeton.cs.algs4.StdOut;   
import edu.princeton.cs.algs4.QuickX;   


public class CircularSuffixArray {
    public String original;
    public int length;
    public CircularSuffix[] suffixes;

    // circular suffix array of s
    public class CircularSuffix implements Comparable<CircularSuffix>{
        public int index;

        public CircularSuffix(String s, int i) {
            this.index = i;
        }

        public int compareTo(CircularSuffix that) {
            int i = this.index;
            int j = that.index;
            char thisChar = original.charAt(i);
            char thatChar =  original.charAt(j);
            i++;
            j++;
            
            while(thisChar == thatChar && i != this.index && j != that.index) {
                if (i >= original.length()) {
                    i = 0;
                }
                
                if (j >= original.length()) {
                    j = 0;
                }

                thisChar = original.charAt(i);
                thatChar = original.charAt(j);
                i++;
                j++;
            }
            return thisChar - thatChar;
        }
    }
    
    private void imprime() {
        for(int i = 0; i < this.length; i++) {
            StdOut.println(this.suffixes[i].index + ") " + original);
        }
    }

    public CircularSuffixArray(String s) {
        this.original = s;
        this.length = s.length();
        this.suffixes = new CircularSuffix[this.length];
        
        for(int i = 0; i < this.length; i++) {
            CircularSuffix suffix = new CircularSuffix(s, i);
            this.suffixes[i] = suffix;
        }
        QuickX.sort(this.suffixes);

    }

    // length of s
    public int length() {
        return this.length;
    }

    // returns index of ith sorted suffix
    public int index(int i) {
        return this.suffixes[i].index;
    }

    // unit testing (required)
    public static void main(String[] args) {
        CircularSuffixArray array = new CircularSuffixArray("ABRACADABRA!");
        array.imprime();
        StdOut.println("array.length(): " +  array.length());
    }

}