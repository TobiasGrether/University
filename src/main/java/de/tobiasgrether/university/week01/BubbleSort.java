package de.tobiasgrether.university.week01;

public class BubbleSort {
    public static void main(String[] args) {
        int[] array = new int[]{1, 3, 5, 4, 2, 0};

        for (int i = 0; i < array.length; i++) {
            for (int i1 = 0; i1 < i; i1++) {
                if (array[i1] > array[i1 + 1]) {
                    int val = array[i1];
                    array[i1] = array[i1+1];
                    array[i1+1] = val;
                }

                for(int element : array){
                    System.out.print(element);
                }
                System.out.println("");
            }
        }


    }

}
