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
import edu.princeton.cs.algs4.BinaryStdIn;
import edu.princeton.cs.algs4.BinaryStdOut;
import edu.princeton.cs.algs4.StdOut;   

public class MoveToFront {

    // apply move-to-front encoding, reading from standard input and writing to standard output
    private static void shift(char[] array, char c, boolean flag) {
        int i;
        for(i = 0; array[i] != c; i++);
        if(flag)
            BinaryStdOut.write((char) i);
        for(int l = i; l > 0; l--) {
            array[l] = array[l - 1];
        }
        array[0] = c;

    }
    
    public static void encode() {
        char[] alphabet = new char[256];

        int m;
        for(int k = 0; k < alphabet.length; k++) {
            alphabet[k] = (char) k;
        }

        while(!BinaryStdIn.isEmpty()) {
            char c = BinaryStdIn.readChar();
            shift(alphabet, c, true);

        }

        for(int j = 0; j < alphabet.length; j++) {
        }

        // BinaryStdIn.close();    
        BinaryStdOut.close();
    }

    // apply move-to-front decoding, reading from standard input and writing to standard output
    public static void decode() {
        char[] alphabet = new char[256];
        for(int k = 0; k < alphabet.length; k++) {
            alphabet[k] = (char) k;
        }

        while(!BinaryStdIn.isEmpty()) {
            char c = BinaryStdIn.readChar();
            BinaryStdOut.write(alphabet[c]);
            shift(alphabet, alphabet[c], false);
        }
        BinaryStdOut.close();

    }

    // if args[0] is "-", apply move-to-front encoding
    // if args[0] is "+", apply move-to-front decoding
    public static void main(String[] args) {
        if(args.length < 1)
            return;
        if(args[0].equals("-"))
            encode();
        
        if(args[0].equals("+"))
            decode();
    }
}