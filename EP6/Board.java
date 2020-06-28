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
        - A ideia de utilizar 2 union-finds para evitar o bug do "backwash" foi retirada de um issue do github
        - Utilizei as dicas do checklist do enunciado

    Se for o caso, descreva a seguir 'bugs' e limitações do seu programa:

****************************************************************/

import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Queue;

import java.util.Iterator; // passo 0 para criarmos um iterador

public class Board implements Comparable<Board>{
    public int[] board;
    private int n;
    private int hammingDist;
    private int manhattanDist;
    private int blankRow;
    private int blankCol;
    public int [][]temp;


    // create a board from an n-by-n array of tiles,
    // where tiles[row][col] = tile at (row, col)
    public Board(int[][] tiles) {
        this.board = new int[tiles.length*tiles.length];
        this.n = tiles.length;
        this.hammingDist = 0;
        this.manhattanDist = 0;
        int colGoal, rowGoal, current;
        temp = new int[this.n][this.n];

        for(int i = 0; i < this.n; i++) {
            for(int j = 0; j < this.n; j++) {
                temp[i][j] = tiles[i][j];
                this.board[i*n + j] = tiles[i][j];
                current = this.board[i * n + j];
                
                if(tiles[i][j] == 0) {
                    // Salva as coodernadas do 0
                    this.blankRow = i;
                    this.blankCol = j;
                }

                if(current != 0){
                    // Salva as coodenadas finais de cada peça (!= 0) para o cálculo do manhattan
                    if(current % this.n == 0) {
                        colGoal = this.n - 1;
                        rowGoal = current / this.n - 1; 
                    }
                    else {
                        rowGoal = current / this.n; 
                        colGoal = (current % this.n) - 1; 
                    }
                    // Calcula Manhattan
                    manhattanDist += Math.abs(i - rowGoal) + Math.abs(j - colGoal);
                }
                if(this.board[i * n + j] != 0 && this.board[i * n + j] != i*n + j + 1) {
                    // Calcula Hamming
                    this.hammingDist++;
                }
            }
        }
    }
    
    // string representation of this board
    public String toString() {
        String strBoard = new String();
        strBoard += Integer.toString(this.n) + "\n";
        for(int i = 0; i < this.n; i++) {
            for(int j = 0; j < this.n; j++) {
                strBoard += Integer.toString(this.board[i * n + j]) + " ";
            }
            strBoard += "\n";

        } 
        return strBoard;
    }

    // tile at (row, col) or 0 if blank
    public int tileAt(int row, int col) {
        if(row < 0 || row >= this.n || col < 0 || col >= this.n)
            throw new java.lang.IllegalArgumentException();
        return this.board[row * n + col];
    }


    // board size n
    public int size() {
        return this.n;
    }

    // number of tiles out of place
    public int hamming() {
        return this.hammingDist;
    }

    // sum of Manhattan distances between tiles and goal
    public int manhattan() {
        return this.manhattanDist;
    }

    // is this board the goal board?
    public boolean isGoal() {
        return hamming() == 0;
    }

    // does this board equal y?
    public boolean equals(Object y) {
        if (y == this) return true;
        if (y == null) {
            return false;
        }
        if (y.getClass() != this.getClass()) return false;
        Board that = (Board) y;
        for(int i = 0; i < this.n; i++) {
            for(int j = 0; j < this.n; j++) {
                if (this.board[i * n + j] != that.board[i * n + j]) {
                    return false;
                } 
            }
        }
        return true;
    }

    // all neighboring boards
    public Iterable<Board> neighbors() {
        Queue<Board> qu = new Queue<Board>();
        int aux;
        Board neighbor;

        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                if(this.blankRow + i >= 0 &&
                    this.blankRow + i < n && this.blankCol + j >= 0 &&
                    this.blankCol + j < n && Math.abs(j) != Math.abs(i)) {
                        
                    // swap
                    aux = temp[this.blankRow][this.blankCol];
                    temp[this.blankRow][this.blankCol] = temp[this.blankRow + i][this.blankCol + j];   
                    temp[this.blankRow + i][this.blankCol + j] = aux; 

                    neighbor = new Board(temp);
                    qu.enqueue(neighbor);

                    // desfaz swap
                    aux = temp[this.blankRow][this.blankCol];
                    temp[this.blankRow][this.blankCol] = temp[this.blankRow + i][this.blankCol + j];   
                    temp[this.blankRow + i][this.blankCol + j] = aux; 
                   }
            }
        }
        return qu;
    }

    // Função auxiliar para copiar array
    private int[] copyArray(int[] board) {
        int[] array = new int[this.n*this.n + 1];
        int index = 1;
        for(int i = 0; i < this.n*this.n; i++) {
            if(board[i] != 0) {
                array[index] = board[i];
                index++;
            }
        }
        return array;
    }

    // is this board solvable?
    public boolean isSolvable() {
        int s = 0;
        int i = 1;
        int[] solvable = copyArray(this.board);
        while (i <= this.n*this.n - 1) {
            if (i != solvable[i]) {
                int aux = solvable[i];
                solvable[i] = solvable[aux];
                solvable[aux] = aux;
                s = 1 - s;
            }
            else
                i++;
        }

        if (this.n % 2 == 1)
            return s % 2 == 0;

        return (s + this.blankRow) % 2 == 1;
    }
    // compareTo é apenas utilizado para verificar se os dois são iguais 
    public int compareTo(Board that) {
        if(this.board.equals(that.board)) {
            return 0;
        }
        else {
            return -1;
        }
    } 

    // unit testing (required)
    public static void main(String[] args) {
        int[][] test = new int[][] 
        {
        {1, 2, 3, 4},
        {6, 10, 7, 8},
        {5, 0, 11, 12},
        {9, 13, 14, 15},
        };
       
        Board board = new Board(test);
        StdOut.printf("Board de testes %s\n", board);
        StdOut.printf("Tamanho do Board %s\n", board.size());
        StdOut.printf("tileAt(1, 2): %d\n", board.tileAt(1, 2));
        StdOut.printf("hamming(): %d\n", board.hamming());
        StdOut.printf("manhattan(): %d\n", board.manhattan());
        StdOut.printf("isGoal()? %s\n", board.isGoal());

        
        int[][] compare = new int[][] 
        {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 15, 14, 0},
        };

        Board boardCompare = new Board(compare);
        StdOut.printf("\nCriando novo board para comparação: %s\n", boardCompare);
        StdOut.printf("O board 1 é igual o board 2? %s\n", board.equals(boardCompare));

        StdOut.printf("Iterando pelos neighboors do board 1\n");
        for(Board b : board.neighbors()) {
            StdOut.printf("%s\n", b);
        }

        StdOut.printf("O board 1 é resolvível? %s\n", board.isSolvable());
        StdOut.printf("O board 2 é resolvível? %s\n", boardCompare.isSolvable());
    }
}