package com.company;

import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

public class Main {
    /**********************Q3 - Part 1*********************************/
    /**
     * finds the first space char
     * @param sentence string where the spaces are required
     * @param index where to start looking at the sentence
     * @return index of first space it found
     */
    public static int spaceIndex(String sentence, int index) {

        if ( index >= sentence.length() )
            return index;
        if ( sentence.charAt(index) == ' ' )
            return index;

        return spaceIndex(sentence, index + 1);
    }

    /**
     * takes the opposite of the sentence so that the words remain the same
     * @param sentence string to be taken in reverse.
     * @param index indicates the first letter of each word of the string to be taken in reverse
     * @param result desired state of the given string
     * @return String as result
     */
    public static String revertSentence(String sentence, int index, String result) {
        if ( index >= sentence.length() )
            return result;

        int wordEnd = spaceIndex(sentence, index);
        String word = sentence.substring(index, wordEnd);
        result = word + " " + result;

        return revertSentence(sentence, wordEnd + 1, result);
    }

    /**********************Q3 - Part 2*********************************/
    /**
     * Checks does the word if elfish ?
     * @param word string to check
     * @param found array to hold the e,l,f letters found
     * @param index indicating the letter to be looked up in the word
     * @return  if elements of found are non-zero, true otherwise false
     */
    public static boolean isElfishWord(String word, int found[], int index) {
        if ( index == word.length() ) {
            if ( found[0] != 0 && found[1] != 0 && found[2] != 0 ) {
                found[0] = 0; found[1] = 0; found[2] = 0;
                return true;
            }
            return false;
        }
        switch ( word.charAt(index) ) {
            case 'e':
                found[0]++;
                break;
            case 'l':
                found[1]++;
                break;
            case 'f':
                found[2]++;
                break;
            default:
                break;
        }
        return isElfishWord(word, found, index + 1);
    }

    /**********************Q3 - Part 3*********************************/
    /**
     * finds min element in array
     * @param a the smallest element sought
     * @param size length of array
     * @param index  Shows the number to look in the array
     * @return index of the smallest element in the array, if the index under consideration equals size return -1
     */
    public static int findMinIndex(int[] a, int size, int index) {
        if ( index == size )
            return index - 1;
        int temp = a[index];

        int minIndex = findMinIndex(a, size, index + 1);

        if ( temp < a[minIndex] )
            return index;

        return minIndex;
    }

    /**
     * selection sort
     * @param a array to sort
     * @param size length of array
     * @param index Shows the number to look in the array
     */
    public static void selectionSort(int a[], int size, int index) {
        if ( size == index )
            return;

        int minIndex = findMinIndex(a, size, index + 1);

        if ( a[index] > a[minIndex] ) {
            int temp = a[index];
            a[index] = a[minIndex];
            a[minIndex] = temp;
        }

        selectionSort(a, size, index + 1);
    }

    /**********************Q3 - Part 4*********************************/

    /**
     * finds the first space char
     * @param sentence string where the spaces are required
     * @param index where to start looking at the sentence
     * @return index of first space it found
     */
    public static int spaceIndexForPrefix(String sentence, int index) {
        if ( index < 0 )
            return index;
        if ( sentence.charAt(index) == ' ' )
            return index;
        return spaceIndexForPrefix(sentence, index - 1);
    }
    /**
     * First digitizes the expression written as prefix and then calculates its result.
     * @param expression to be evaluated
     * @param index indicates each char of the expression
     * @param stack keeps numeric values
     * @return result of expression
     */
    public static double evaluatePrefix(String expression, int index, Stack< Double > stack) {
        double num1, num2;
        try {
            if ( index < 0) {
                if ( stack.size() == 1 )
                    return stack.pop();
                else
                    throw new Exception("There are extra element in stack. Invalid expression !");
            }
        } catch ( Exception e ) {
            System.out.println(e.getMessage());
            return -1;
        }

        int numberBegin = spaceIndexForPrefix(expression, index);
        switch ( expression.charAt(index) ) {
            case '+':
                num1 = stack.pop();
                num2 = stack.pop();
                stack.push(num1 + num2);
                break;

            case '-':
                num1 = stack.pop();
                num2 = stack.pop();
                stack.push(num1 - num2);
                break;

            case '*':
                num1 = stack.pop();
                num2 = stack.pop();
                stack.push(num1 * num2);
                break;

            case '/':
                num1 = stack.pop();
                num2 = stack.pop();
                stack.push(num1 / num2);
                break;

            case ' ':
                System.out.println();
                break;

            default:
                String number = expression.substring(numberBegin, index+1);
                stack.push(Double.parseDouble(number));
                break;
        }
        //Prints the stack
    /*    for ( int i = stack.size() - 1; i >= 0; i-- ) {
            System.out.println(stack.get(i));
        }
        System.out.println("_____________"); */
        return evaluatePrefix(expression, numberBegin - 1, stack);
    }

    /**********************Q3 - Part 5*********************************/
    /**
     * First digitizes the expression written as prefix and then calculates its result.
     * @param expression to be evaluated
     * @param index indicates each char of the expression
     * @param stack keeps numeric values
     * @return result of expression
     */
    public static double evaluatePostfix(String expression, int index, Stack< Double > stack) {
        double num1, num2;
        try {
            if ( expression.length() <= index ) {
                if ( stack.size() == 1 )
                    return stack.pop();
                else {
                    System.out.println(stack.pop() + " " + stack.pop());
                    throw new Exception("There are extra element in stack. Invalid expression !");
                }
            }
        } catch ( Exception e ) {
            System.out.println(e.getMessage());
            return -1;
        }

        int numberEnd = spaceIndex(expression, index);

        switch ( expression.charAt(index) ) {
            case '+':
                num1 = stack.pop();
                num2 = stack.pop();
                stack.push(num1 + num2);
                break;

            case '-':
                num1 = stack.pop();
                num2 = stack.pop();
                stack.push(num2 - num1);
                break;

            case '*':
                num1 = stack.pop();
                num2 = stack.pop();
                stack.push(num2 * num1);
                break;

            case '/':
                num1 = stack.pop();
                num2 = stack.pop();
                stack.push(num2 / num1);
                break;

            case ' ':
                break;

            default:
                String number = expression.substring(index, numberEnd);
                stack.push(Double.parseDouble(number));
                break;
        }
        //Prints the stack
      /*  for ( int i = stack.size() - 1; i >= 0; i-- ) {
            System.out.println(stack.get(i));
        }
        System.out.println("_____________"); */
        return evaluatePostfix(expression, numberEnd + 1, stack);
    }


    public static void main(String[] args) {

        /**********************Q3 - Part 1*********************************/
        String sentence = "this function writes the sentence in reverse";
       System.out.print(revertSentence(sentence, 0, ""));


        /**********************Q3 - Part 2*********************************/
        System.out.println("\n");
        int[] found = {0, 0, 0};  //e,l,f
        String word = "waffles";
        System.out.println(word + " is elfish : " + isElfishWord(word, found, 0));

        word = "wrong";
        System.out.println(word + " is elfish : " + isElfishWord(word, found, 0));


        /**********************Q3 - Part 3*********************************/
        int[] listForSort = {3, 1, -5, 7, 0,25,70,-100,6};
        selectionSort(listForSort, listForSort.length, 0);

        for ( int i = 0; i < listForSort.length; i++ ) {
            System.out.print(listForSort[i] + " ");
        }
        System.out.println("\n");


        /**********************Q3 - Part 4*********************************/
        Stack< Double > stack1 = new Stack();
        String exp = "+ 7 + / - 8 * 2 3 2 - 10 / 20 5";
        System.out.println(exp + " result (Prefix) :" + evaluatePrefix(exp,exp.length()-1 , stack1));



        /**********************Q3 - Part 5*********************************/
        Stack< Double > stack = new Stack();
        exp = "7 8 2 3 * - 2 / + 10 + 20 5 / -";
        System.out.println(exp + " result (Postfix) :" + evaluatePostfix(exp, 0, stack));

    }


}
