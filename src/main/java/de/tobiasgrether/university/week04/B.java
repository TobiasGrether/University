package de.tobiasgrether.university.week04;

public class B {
    private Integer i1;
    private int i2;
    private Double d;
    private float f;

    public B(int a, int b, int c, int d) {
        this.i1 = a;
        this.i2 = b;
        this.d = (double) d;
        this.f = c;
    }

    public B(Integer a, int b, Double c, float d) {
        this.i1 = a;
        this.i2 = b;
        this.d = c;
        this.f = d;
    }

    public Integer f(double x, int y) {
        return 11;
    }

    public int f(int x, float y) {
        return 12;
    }

    public int f(Double x, long y) {
        return 13;
    }

    public double g(Float x) {
        return 7.0;
    }

    public Float g(double x) {
        return 8f;
    }

    public static void main(String[] args) {
        B b1 = new B(1, 2, 3, 4);
        System.out.println(b1.d);
        System.out.println(b1.f(7d, 8L));
        System.out.println(b1.f(10d, 17));
        System.out.println(b1.f(5, 6L));
        B b2 = new B(b1.i1, 5, 6, 9);
        System.out.println(b2.f);
        System.out.println(b2.f(b1.f, b1.i2));
        B b3 = new B(b2.i1, 14, 1.5, 16);
        System.out.println(b3.d);
        System.out.println(b3.g(b1.i1));
        System.out.println(b3.g(Float.valueOf(18)));
        System.out.println(b3.f(b2.g(19f), 21));

    }
}
// a) // b) // c) // d)
// e) // f)
// g) // h) // i) // j)