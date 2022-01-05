public class Task03 {
    public static class A {
        public final String x;

        public A() { // Signatur : A()
            this(" written in A()");
        }

        public A(int p1) { // Signatur : A(int )
            this(" written in A( int )");
        }

        public A(String x) { // Signatur : A( String )
            this.x = x;
        }

        public void f(A p1) { // Signatur : A.f(A)
            System.out.println(" called A.f(A)");
        }
    }

    public static class B extends A {
        public final String x;

        public B() { // Signatur : B()
            this(" written in B()");
        }

        public B(int p1) { // Signatur : B(int )
            this(" written in B( int )");
        }

        public B(A p1) { // Signatur : B(A)
            this(" written in B(A)");
        }

        public B(B p1) { // Signatur : B(B)
            this(" written in B(B)");
        }

        public B(String x) { // Signatur : B( String )
            super(" written in B( String )");
            this.x = x;
        }

        public void f(A p1) { // Signatur : B.f(A)
            System.out.println(" called B.f(A)");
        }

        public void f(B p1) { // Signatur : B.f(B)
            System.out.println(" called B.f(B)");
        }
    }

    public static void main(String[] args) {
        // a)
        A v1 = new A(100); // (1)
        System.out.println("v1.x: " + v1.x);

        A v2 = new B(100); // (2)
        System.out.println("v2.x: " + v2.x);
        System.out.println("((B) v2 ).x: " + ((B) v2).x);
        B v3 = new B(v2); // (3)
        System.out.println("((A) v3 ).x: " + ((A) v3).x);

        System.out.println("v3.x: " + v3.x);

        B v4 = new B(); // (4)
        System.out.println("((A) v4 ).x: " + ((A) v4).x);
        System.out.println("v4.x: " + v4.x);

        // b)
        v1.f(v1); // (1)
        v1.f(v2); // (2)
        v1.f(v3); // (3)
        v2.f(v1); // (4)
        v2.f(v2); // (5)
        v2.f(v3); // (6)
        v3.f(v1); // (7)
        v3.f(v2); // (8)
        v3.f(v3); // (9)
    }
}