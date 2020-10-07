package com.company;

import java.util.LinkedList;

public class MyMergeSort<E extends Comparable<E>> extends LinkedList<E> {

    private void merge(LinkedList<E> linkedList,int left,int middle,int right) {
        int s1 = middle-left+1;
        int s2 = right - middle;

        LinkedList<E> subListLeft = new LinkedList<>();
        LinkedList<E> subListRight = new LinkedList<>();

        for ( int i = 0; i < s1 ; i++ )
            subListLeft.add(i, linkedList.get(left + i));
        for ( int i = 0; i <  s2 ; i++ )
            subListRight.add( i, linkedList.get(middle+1+i) );

        int i = 0, j= 0;
        int k = left;
        while ( i < subListLeft.size() && j < subListRight.size() ){
            int compRes = subListLeft.getFirst().compareTo( subListRight.getFirst() );
            if( compRes <= 0 ) {
                linkedList.set(k, subListLeft.get(i++));
            } else {
                linkedList.set(k,subListRight.get(j++));
            }
            k++;
        }
        while (i < s1)
        {
            linkedList.set(k++, subListLeft.get(i++));
        }

        while (j < s2)
        {
            linkedList.set(k++, subListRight.get(j++));
        }
    }


    private void sort(LinkedList<E> linkedList,int left,int right) {
        if(left < right) {;
            int middle = (left + right) /2;

            sort(linkedList,left,middle);
            sort(linkedList,middle+1,right);

            merge(linkedList,left,middle,right);
        }
    }

    public void sort(LinkedList<E> linkedList) {
        sort(linkedList,0,linkedList.size()-1);

    }
}
