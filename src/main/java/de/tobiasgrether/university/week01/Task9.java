package de.tobiasgrether.university.week01;

import de.tobiasgrether.university.utils.TriFunction;

public class Task9 {
    public static void main(String args[]){
        printResult('a', (x, y, z) -> 2000000000 + x);
        printResult('b', (x, y, z) -> 2000000000 + 'x');
        printResult('c', (x, y, z) -> 2000000000 + "x");
        printResult('d', (x, y, z) ->  (byte) (3 * z));
        printResult('e', (x, y, z) -> (int) 2147483648L * z);
        printResult('f', (x, y, z) ->  (byte) 256 * 3f);
        printResult('h', (x, y, z) -> y != 'y' ? 1.0 : 'z');
        System.out.println((int) 3f);
        System.out.println(((float) 2_000_000_000 + 1_000_000_000 + 1_294_967_296) / Integer.MAX_VALUE);
    }

    private static void printResult(Character taskIdentifier, TriFunction<Integer, Integer, Integer, Object> func){
        int x = 1000000000;
        int y = 121;
        int z = 126;

        Object output = func.apply(x, y, z);
        System.out.println(taskIdentifier + ") " + output.toString() + " (@" + output.getClass().getSimpleName() + ")");
    }

}
