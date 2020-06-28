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
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.MinPQ;
// import edu.princeton.cs.algs4.SET;
import edu.princeton.cs.algs4.In;
import java.util.Iterator; // passo 0 para criarmos um iterador

public class Solver {
    private int moves;
    private SearchNode solution;
    // find a solution to the initial board (using the A* algorithm)
    public Solver(Board initial) {
        if(initial == null) {
            throw new java.lang.IllegalArgumentException();
        }

        if(!initial.isSolvable()) {
            StdOut.println("Não possui solução.");
            throw new java.lang.IllegalArgumentException();
        }

        // SET<Board> set = new SET<Board>();
        MinPQ<SearchNode> pq = new MinPQ<SearchNode>(); 

        SearchNode root = new SearchNode(initial, null);
        SearchNode current;

        pq.insert(root);
        // set.add(root.board);
        // Utilizando a versão sem SET pois nos testes obvete desempenho melhor
        while(!root.board.isGoal()) {
            root = pq.delMin();

            for(Board b : root.board.neighbors()) {
                current = new SearchNode(b, root);
                // if((root.previous == null || !current.board.equals(root.previous.board)) && !set.contains(current.board)){
                if(root.previous == null || !current.board.equals(root.previous.board)){
                    pq.insert(current);
                }
                // set.add(root.board);
            }
        }
            this.moves = root.moves;
            this.solution = root;
    }
    
    public static class SearchNode implements Comparable<SearchNode>{
        private int moves;
        private Board board;
        private SearchNode previous;

        public SearchNode(Board b, SearchNode previous) {
            this.board = b; 
            this.previous = previous;
            if(previous == null) {
                this.moves = 0;
            } else {
                this.moves = previous.moves + 1;
            }

        }
        
        public int compareTo(SearchNode node) {
            if((this.board.manhattan() + this.moves) < (node.board.manhattan() + node.moves)) {
                return -1;
            }
            else if ((this.board.manhattan() + this.moves) == (node.board.manhattan() + node.moves)) {
                return 0;
            }
            else {
                return 1;
            }
        }

    }

    // Moves do board
    public int moves() {
        return this.moves;
    }

    // Caminho da solução
    public Iterable<Board> solution() {
        Stack<Board> stack = new Stack<Board>();
        SearchNode current = this.solution;
        while(current != null) {
            stack.push(current.board);
            current = current.previous;
        }

        return stack;

    }

    // test client (see below) 
    public static void main(String[] args) {
        boolean flag = false;

        // Unit-test com arquivo como parâmetro
        for (String filename : args) {
            // read in the board specified in the filename
            In in = new In(filename);
            int n = in.readInt();
            int[][] blocks = new int[n][n];
            for (int row = 0; row < n; row++)
            for (int col = 0; col < n; col++)
            blocks[row][col] = in.readInt();
            Board initial = new Board(blocks);
            Solver s = new Solver(initial);
            for(Board b : s.solution()) {
                StdOut.println(b.toString());
            }
            flag = true;
        }

        // Unit-test sem arquivo como parâmetro
        if(flag == false){
            int[][] t = new int[][]{
                {1,  0,  2}, 
                {7,  5,  4}, 
                {8,  6,  3}, 
            };
            
            Board test = new Board(t); 
            Solver sol = new Solver(test);

            StdOut.printf("Board de teste \n%s\n", test);
            StdOut.printf("Caminho de solução:\n");

            for(Board b : sol.solution()) {
                StdOut.println(b.toString());
            }
        }
    }

}