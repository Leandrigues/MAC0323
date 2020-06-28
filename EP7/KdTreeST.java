/****************************************************************
    Nome: Leandro Rodrigue da Silva
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
    Entendo que  EPs sem assinatura devem receber nota zero e, ainda
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
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.RedBlackBST;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.Queue;
import edu.princeton.cs.algs4.StdRandom;
import java.util.Iterator;

public class KdTreeST<Value> {
    Node<Value> root;
    int size;

    public static class Node<Value> {
        Node<Value> right;
        Node<Value> left;
        Point2D key;
        Value val;
        boolean vert;
        RectHV rect;

        public Node(Node<Value> right, Node<Value> left, Point2D key, Value val, boolean vert, RectHV rect) {
            this.right = right;
            this.left = left;
            this.key = key;
            this.val = val;
            this.vert = vert;
            this.rect = rect;
        }
    }

    // construct an empty symbol table of points 
    public KdTreeST() {
        RectHV rect = new RectHV(Double.POSITIVE_INFINITY * -1, Double.POSITIVE_INFINITY * -1, Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY);
        this.root = new Node<Value>(null, null, null, null, true, rect);
        this.size = 0;
    }

    // is the symbol table empty? 
    public boolean isEmpty() {
        return this.size == 0;
    }

    // number of points
    public int size() {
        return this.size;
    }

    // associate the value val with point p
    public void put(Point2D p, Value val) {
        if(p == null || val == null) {
            throw new java.lang.IllegalArgumentException();
        }
        if(root.key == null) {
            root.key = p;
            root.val = val;
            return;
        }

        Node<Value> current = this.root; 
        Node<Value> previous = new Node<Value>(null, null, null, null, true, null);
        Node<Value> newNode;
        while(current != null) {
            previous = current;
            if(current.vert) {
                if(p.x() < current.key.x()) {
                    current = current.left;
                }
                else {
                    current = current.right;
                }
            }

            else {
                if(p.y() < current.key.y()) {
                    current = current.left;
                }
                else {
                    current = current.right;
                }
            }
        }

        if(previous.vert) {
            if(p.x() < previous.key.x()) {
                RectHV rect = new RectHV(previous.rect.xmin(), previous.rect.ymin(), previous.key.x(), previous.rect.ymax());
                newNode = new Node<Value>(null, null, p, val, true, rect);
                previous.left = newNode;

            }
            else {
                RectHV rect = new RectHV(previous.key.x(), previous.rect.ymin(), previous.rect.xmax(), previous.rect.ymax());
                newNode = new Node<Value>(null, null, p, val, true, rect);
                previous.right = newNode;

            }
        }
        else {
            if(p.y() < previous.key.y()) {
                RectHV rect = new RectHV(previous.rect.xmin(), previous.rect.ymin(), previous.rect.xmax(), previous.key.y());
                newNode = new Node<Value>(null, null, p, val, true, rect);
                previous.left = newNode;
            }
            else {
                RectHV rect = new RectHV(previous.rect.xmin(), previous.key.y(), previous.rect.xmax(), previous.rect.ymax());
                newNode = new Node<Value>(null, null, p, val, true, rect);
                previous.right = newNode;
            }      
        }
        newNode.vert = !previous.vert;
    }

    // value associated with point p 
    public Value get(Point2D p) {
        if(p == null) {
            throw new java.lang.IllegalArgumentException();
        }
        Node<Value> current = this.root; 
        Node<Value> previous = new Node<Value>(null, null, null, null, true, null);

        while(current != null && (current.key.x() != p.x() || current.key.y() != p.y())) {
            previous = current;
            if(current.vert) {
                if(p.x() < current.key.x()) {
                    current = current.left;
                }
                else {
                    current = current.right;
                }
            }

            else {
                if(p.y() < current.key.y()) {
                    current = current.left;
                }
                else {
                    current = current.right;
                }
            }
        }

        if(current == null) {
            StdOut.println("O ponto não está na tabela");
            return null;
        }
        return current.val;
    }

    // does the symbol table contain point p? 
    public boolean contains(Point2D p) {
        if(p == null) {
            throw new java.lang.IllegalArgumentException();
        }
        Value val = this.get(p);
        return val != null;
    }

    // all points in the symbol table 
    public Iterable<Point2D> points() {
        Queue<Node<Value>> q1 = new Queue<Node<Value>>();
        Queue<Node<Value>> q2 = new Queue<Node<Value>>();
        Node<Value> current;
        q1.enqueue(this.root);
        if(this.root.left != null) {
            q2.enqueue(this.root.left);
        }
        if(this.root.right != null) {
            q2.enqueue(this.root.right);
        }

        while(!q2.isEmpty()) {
            current = q2.dequeue();
            q1.enqueue(current);
            if(current.left != null) {
                q2.enqueue(current.left);
            }
            if(current.right != null) {
                q2.enqueue(current.right);
            }
        }

        Queue<Point2D> q3 = new Queue<Point2D>();
        while(!q1.isEmpty()) {
            q3.enqueue(q1.dequeue().key);
        }
        return q3;
    }

    // all points that are inside the rectangle (or on the boundary) 
    private void rangeWrapper(RectHV rect, Node<Value> root, Queue<Point2D> q) {
        if(root != null && rect.intersects(root.rect)){
            if(rect.contains(root.key)) {
                q.enqueue(root.key);
            }
            if(root.left != null && rect.intersects(root.left.rect)) {
                if(rect.contains(root.left.key)) {
                    q.enqueue(root.left.key);
                }
                rangeWrapper(rect, root.left, q);
            }
            if(root.right != null && rect.intersects(root.right.rect)) {
                if(rect.contains(root.right.key)) {
                    q.enqueue(root.right.key);
                }
                rangeWrapper(rect, root.right, q);
            }
        }
    }
    public Iterable<Point2D> range(RectHV rect) {
        if (rect == null) {
            throw new java.lang.IllegalArgumentException();
        }
        Queue<Point2D> q = new Queue<Point2D>();
        this.rangeWrapper(rect, this.root, q);
        return q;
    }

    // a nearest neighbor of point p; null if the symbol table is empty 
        private Point2D nearestWrapper(Node<Value> current, Point2D p, Point2D closest){
        if(current != null){
            if(closest == null){
                closest = current.key;
            }

            if(closest.distanceSquaredTo(p) >= current.rect.distanceSquaredTo(p)){
                if(current.key.distanceSquaredTo(p) < closest.distanceSquaredTo(p))
                    closest = current.key;

                if(current.right != null && current.right.rect.contains(p)){
                    closest = nearestWrapper(current.right, p, closest);
                    closest = nearestWrapper(current.left, p , closest);
                }

                else{
                    closest = nearestWrapper(current.left, p , closest);
                    closest = nearestWrapper(current.right, p, closest);
                }

            }
        }
        return closest;

    }
    public Point2D nearest(Point2D p) {
        if (p == null) {
            throw new java.lang.IllegalArgumentException();
        }
        return nearestWrapper(root, p, null);
    }

    // unit testing (required)
    public static void main(String[] args) {
        StdOut.println("Criando uma ST vazia");
        KdTreeST<Integer> st = new KdTreeST<Integer>();
        Node<Integer> r = st.root;

        StdOut.println("isEmpty()?" + st.isEmpty());

        StdOut.println("Fazendo put() de 5 pontos (do exemplo do enunciado");
        Point2D p;
        p = new Point2D(0.7, 0.2);
        st.put(p, 1);
        p = new Point2D(0.5, 0.4);
        st.put(p, 2);
        p = new Point2D(0.2, 0.3);
        st.put(p, 3);
        p = new Point2D(0.4, 0.7);
        st.put(p, 4);
        p = new Point2D(0.9, 0.6);
        st.put(p, 5);

        StdOut.println("A tabela contem o ponto (.9, .6)? " + st.contains(p));
        StdOut.println("\nIterando com points()");
        for(Point2D q : st.points()) {
            StdOut.println(q);
        }

        StdOut.println("\nCriando um retângulo com pontos (0,0) e (.5,.5)");
        RectHV rect = new RectHV(0, 0, 0.5, 0.5);

        StdOut.println("Pontos contidos no retângulo :");
        for(Point2D k : st.range(rect)) {
            StdOut.println(k);  
        }
        StdOut.println();
        for(int i = 0; i <= 5; i++) {
            double x = StdRandom.uniform(0, 10)/(double)10;
            double y = StdRandom.uniform(0, 10)/(double)10;
            Point2D t = new Point2D(x,y);
            StdOut.println("O ponto mais próximo de " + t + " é " + st.nearest(t));
        }

        StdOut.println("\nFazendo get() de todos valores");
        for(Point2D k : st.points()) {
            StdOut.println("O valor armazenado no ponto " + k + " é " + st.get(k));
        }

    }
}