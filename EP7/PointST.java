import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.RedBlackBST;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.StdRandom;
import java.util.Iterator; // passo 0 para criarmos um iterador

public class PointST<Value> {
    RedBlackBST<Point2D, Value> rbTree;

    // construct an empty symbol table of points 
    public PointST() {
        this.rbTree = new RedBlackBST<Point2D, Value>(); 
    }

    // is the symbol table empty? 
    public boolean isEmpty() {
        return this.rbTree.isEmpty();
    }

    // number of points
    public int size() {
        return this.rbTree.size();
    }

    // associate the value val with point p
    public void put(Point2D p, Value val) {
        if(val == null || p == null) {
            throw new java.lang.IllegalArgumentException();
        }

        this.rbTree.put(p, val);
    }

    // value associated with point p 
    public Value get(Point2D p) {
        if(p == null) {
            throw new java.lang.IllegalArgumentException();
        }

        return this.rbTree.get(p);
    }

    // does the symbol table contain point p? 
    public boolean contains(Point2D p) {
        if(p == null) {
            throw new java.lang.IllegalArgumentException();
        }
        return this.rbTree.contains(p);
    }

    // all points in the symbol table 
    public Iterable<Point2D> points() {
        return this.rbTree.keys();
    }

    // all points that are inside the rectangle (or on the boundary) 
    public Iterable<Point2D> range(RectHV rect) {
        if(rect == null) {
            throw new java.lang.IllegalArgumentException();
        }
        Iterable<Point2D> it = this.points();
        Stack<Point2D> stk = new Stack<Point2D>();

        for(Point2D p : it) {
            if(rect.contains(p)) {
                stk.push(p);
            }
        }

        return stk;
    }

    // a nearest neighbor of point p; null if the symbol table is empty 
    public Point2D nearest(Point2D p) {
        if(p == null) {
            throw new java.lang.IllegalArgumentException();
        }
        
        double minDist = 0;
        Point2D nearest = null; 
        boolean first = true;
        Iterable<Point2D> it = this.points();

        for(Point2D q : it) {
            // Se for a primeira iteração, o primeiro ponto é o mair próximo.
            if(first) {
                minDist = p.distanceSquaredTo(q);
                nearest = q;
                first = false;
            }

            else {
                if(p.distanceSquaredTo(q) < minDist) { 
                    minDist = p.distanceSquaredTo(q); 
                    nearest = q;
                }
            }
        }

        return nearest;
    }

    // unit testing (required)
    public static void main(String[] args) {
        StdOut.println("Criando uma sybol table vazia");
        PointST<Integer> st  = new PointST<Integer>();
        StdOut.println("isEmpty()? " + st.isEmpty());
        
        Point2D p;

        StdOut.println("Fazendo put() de 5 pontos (do exemplo do enunciado) na ST");
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

        StdOut.println("size()? " + st.size());

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