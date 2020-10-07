package com.company;

import org.w3c.dom.Node;

import java.util.Comparator;
import java.util.LinkedList;
import java.util.ListIterator;

public class MyQuickSort<E extends Comparable<E>> extends LinkedList<E>{

    private int partition(LinkedList<E> linkedList, int down, int up)
    {
        E pivot = linkedList.get(up);
        int i = (down-1); // index of smaller element
        for (int j=down; j<up; j++)
        {
            // If current element is smaller than the pivot
            int compRes = pivot.compareTo(linkedList.get(j)); //  pivot < j = -1
            if (compRes > 0)
            {
                i++;
                // swap arr[i] and arr[j]
                E temp = linkedList.get(i);
                linkedList.set(i , linkedList.get(j) );
                linkedList.set(j, temp);
            }
        }

        // swap arr[i+1] and arr[high] (or pivot)
        E temp =linkedList.get(i+1);
        linkedList.set(i+1,linkedList.get(up) );
        linkedList.set(up, temp);

        return i+1;
    }

    private void sort(LinkedList<E> linkedList,int down, int up) {
        if(down < up) {
            int partitionIndex = partition(linkedList,down,up);

            sort(linkedList,down,partitionIndex-1);
            sort(linkedList,partitionIndex+1,up);
        }
    }

    public void sort(LinkedList<E> linkedList) {
        sort(linkedList,0,linkedList.size()-1);
    }



}
