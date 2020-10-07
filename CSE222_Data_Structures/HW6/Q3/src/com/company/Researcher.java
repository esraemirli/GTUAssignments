package com.company;

import java.util.Map;
import java.util.Scanner;
import java.util.Set;

public class Researcher extends User {
    BookStore bookStore;
    Researcher(BookStore bookStore) {
        this.bookStore = bookStore;
    }
    @Override
    public void menu() {
        System.out.println("1. Search book by author name. ");
        System.out.println("2. Search book by book title. ");
    }

    public void searchByAuthor(String author) {
        Scanner scanner = new Scanner(System.in);
        String book;
        Map<String, Map<String, Set<String>>> map = bookStore.getMap();

        for( Map.Entry<String, Map<String, Set<String> >> t :map.entrySet()) {
            String key = t.getKey();
            if ( author.matches(key) ) {
                System.out.println("Author Name: " + author);
                for ( Map.Entry< String, Set< String > > e : t.getValue().entrySet() )
                    System.out.println("\t" + e.getKey());

                System.out.println("Enter the name of the book of your choice.");
                book = scanner.nextLine();
                for ( Map.Entry< String, Set< String > > e : t.getValue().entrySet() ) {
                    if(book.matches(e.getKey())) {
                        System.out.println("\tLocations: " + e.getValue());
                        return;
                    }
                }
            }
        }
        System.out.println("No record!");
    }
    public void searchByTitle(String title) {
        Map<String, Map<String, Set<String>>> map = bookStore.getMap();

        for( Map.Entry<String, Map<String, Set<String> >> t :map.entrySet()) {
            String key = t.getKey();
            for ( Map.Entry< String, Set< String > > e : t.getValue().entrySet() ) {
                if(title.matches(e.getKey())) {
                    System.out.println("Author Name: " + key);
                    System.out.println("\tBook Name: " + e.getKey() + "\n\t\tLocations: "+e.getValue());
                    if(e.getValue().size() > 0)
                        System.out.println("\t\tStatus: Available" );
                    return;
                }
            }
        }
        System.out.println("Status: Not-Available!");
    }


}
