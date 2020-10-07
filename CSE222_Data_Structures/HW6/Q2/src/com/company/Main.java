package com.company;

import java.io.IOException;
import java.lang.reflect.Array;
import java.util.*;

public class Main {
    public static void main(String[] args) throws IOException {
         /* Selection Sort
        - Bubble Sort
                - Insertion Sort
                - Shell Sort
                - Merge Sort
                - Heap Sort
                - Quick Sort
            */

        int bound = 180000;
        //20 tane 10 binlik random ...
        Integer[] l1 = new Integer[bound];

        int x;
    /*    Random rand = new Random();
        for ( int i = 0; i < bound ; i++ ) {
            x = rand.nextInt(1000);
            l1[i] = x;
        }

        long startTime = System.nanoTime();
        SelectionSort selectionSort = new SelectionSort();
        selectionSort.sort(l1);
        long duration = System.nanoTime() - startTime;
        System.out.println(duration);

        startTime = System.nanoTime();
        BubbleSort bubbleSort = new BubbleSort();
        bubbleSort.sort(l1);
        duration = System.nanoTime() - startTime;
        System.out.println(duration);

        startTime = System.nanoTime();
        InsertionSort insertionSort = new InsertionSort();
        insertionSort.sort(l1);
        duration = System.nanoTime() - startTime;
        System.out.println(duration);

        startTime = System.nanoTime();
        ShellSort shellSort = new ShellSort();
        shellSort.sort(l1);
        duration = System.nanoTime() - startTime;
        System.out.println(duration);

        startTime = System.nanoTime();
        MergeSort mergeSort = new MergeSort();
        mergeSort.sort(l1);
        duration = System.nanoTime() - startTime;
        System.out.println(duration);

        startTime = System.nanoTime();
        HeapSort heapSort = new HeapSort();
        heapSort.sort(l1);
        duration = System.nanoTime() - startTime;
        System.out.println(duration);

        startTime = System.nanoTime();
        QuickSort quickSort = new QuickSort();
        quickSort.sort(l1);
        duration = System.nanoTime() - startTime;
        System.out.println(duration);

        System.out.println("-------------------");

        bound = 180000;
        LinkedList<Integer> linkedList = new LinkedList<>();
        for ( int i = 0; i < bound ; i++ ) {
            linkedList.add(i);
        }

        MyMergeSort<Integer> myMergeSort = new MyMergeSort<>();
        long startTime = System.nanoTime();
        myMergeSort.sort(linkedList);
        long duration = System.nanoTime() - startTime;
        System.out.println(duration); */

        bound = 10;
        LinkedList< Integer > linkedList = new LinkedList<>();
        linkedList.add(50);

        for ( int i = 0; i < bound; i++ ) {
            linkedList.add(i);
        }

        MyQuickSort< Integer > myQuickSort = new MyQuickSort<>();

        long startTime = System.nanoTime();
        myQuickSort.sort(linkedList);
        long duration = System.nanoTime() - startTime;
        System.out.println(duration);

        for ( int i = 0; i < bound+1; i++ ) {
            System.out.println(linkedList.get(i));
        }
    }
}
