package com.company;

import java.util.InputMismatchException;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {

        BookStore bookStore = new BookStore();

        Scanner scanner = new Scanner(System.in);
        User admin = new Administrator(bookStore,"admin");  //password fill on constructor. pw = admin
        User researcher = new Researcher(bookStore);


        int chooseUser = 0;
        while ( chooseUser != -1 ) {

            System.out.println("1. Login as Administrator");
            System.out.println("2. Login as Researcher");
            System.out.println("-1. Exit");

            try {
                chooseUser = Integer.parseInt( scanner.nextLine() );
            }catch ( NumberFormatException e) {
                System.out.println("Error ! " + e );
                return;
            }
            if(chooseUser == 1) {
                if( (( Administrator ) admin).login() ) {
                    admin.menu();
                    int menuItem;
                    try {
                        menuItem = Integer.parseInt( scanner.nextLine() );

                    } catch ( NumberFormatException e ) {
                        System.out.println("Error ! " + e);
                        return;
                    }
                    if(menuItem == 1)
                        (( Administrator ) admin).addBook();
                    else if(menuItem == 2)
                        (( Administrator ) admin).deleteBook();
                    else if(menuItem == 3)
                        (( Administrator ) admin).updateBook();
                    else
                        System.out.println("Invalid choice !");
                }
                else
                    System.out.println("Invalid Password !");

            } else if(chooseUser == 2) {
                researcher.menu();
                int menuItem;
                try {
                    menuItem = Integer.parseInt( scanner.nextLine() );
                } catch ( NumberFormatException e ) {
                    System.out.println("Error ! " + e);
                    return;
                }
                if(menuItem == 1) {
                    System.out.println("Enter an author name.");
                    String author = scanner.nextLine();
                    (( Researcher ) researcher).searchByAuthor(author);
                }
                else if(menuItem == 2) {
                    System.out.println("Enter a book title.");
                    (( Researcher ) researcher).searchByTitle(scanner.nextLine());
                } else
                    System.out.println("Invalid choice !");

            } else if(chooseUser != -1) {
                System.out.println("Invalid user !");
            }
        }





    }
}
