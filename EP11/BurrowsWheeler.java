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
        Utilizei os videos sobre counting sort do GeeksForGeeks.

    Se for o caso, descreva a seguir 'bugs' e limitações do seu programa:

****************************************************************/
import edu.princeton.cs.algs4.BinaryStdIn;
import edu.princeton.cs.algs4.BinaryStdOut;
import edu.princeton.cs.algs4.StdOut;   


public class BurrowsWheeler {

    // apply Burrows-Wheeler transform,
    // reading from standard input and writing to standard output 
    public static void transform() {
        String s = BinaryStdIn.readString();
        CircularSuffixArray array = new CircularSuffixArray(s);
        char[] charOutput = new char[array.length];

        for(int i = 0; i < array.length; i++) {
            int index = array.suffixes[i].index;

            if(index == 0) {
                charOutput[i] = s.charAt(array.length - 1);
                BinaryStdOut.write(i);

            }
            else {
                charOutput[i] = s.charAt(index - 1);
            }
        }

        for(char c : charOutput){
            BinaryStdOut.write(c);
        }

        BinaryStdOut.close();
    }

    // apply Burrows-Wheeler inverse transform,
    // reading from standard input and writing to standard output
    public static void inverseTransform() {
        int first = BinaryStdIn.readInt();
        String s = BinaryStdIn.readString();
        char[] t = s.toCharArray();
        int size = s.length();
        int[] next = new int[size];
        int[] aux = new int[size];
        int[] frequency = new int[257];
        
        for (int i = 0; i < size; i++){
            frequency[t[i] + 1]++;
        }

        for (int j = 0; j < 256; j++){
            frequency[j + 1] += frequency[j];
        }
        
        for (int i = 0; i < size; i++) {
            int index = frequency[t[i]];
            char c = t[i];
            next[frequency[c]] = i;  
            frequency[c]++;
            aux[index] = c;
        }
        
        BinaryStdOut.write((char) aux[first]);
        int indexNext = first;

        for (int i = 0; i < size - 1; i++) {
            indexNext = next[indexNext];
            BinaryStdOut.write((char) aux[indexNext]);
        }

        // BinaryStdIn.close();
        BinaryStdOut.close();    
    }

    // if args[0] is "-", apply Burrows-Wheeler transform
    // if args[0] is "+", apply Burrows-Wheeler inverse transform
    public static void main(String[] args) {
        if(args.length < 1)
            return;
        if(args[0].equals("-"))
            transform();
        if(args[0].equals("+"))
            inverseTransform();
    }
}