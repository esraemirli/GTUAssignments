package com.company;

public class Main {

    public static void main(String[] args) {
        
        AgeSearchTree<AgeData> ageTree = new AgeSearchTree< AgeData >();

        ageTree.add(new AgeData(10));
        ageTree.add(new AgeData(20));
        ageTree.add(new AgeData(5));
        ageTree.add(new AgeData(15));
        ageTree.add(new AgeData(10));

        String treeStr = ageTree.toString();
        System.out.println(treeStr);

        System.out.println(ageTree.olderThan(15));
        //Print the number of people younger than 15
        System.out.println(ageTree.youngerThan(15));

        //Find the number of people at any age
        System.out.println(ageTree.find(new AgeData(100)).toString());
        System.out.println(ageTree.find(new AgeData(10)).toString());

        ageTree.remove(new AgeData(10));
        System.out.println(ageTree.toString());


    }



}
