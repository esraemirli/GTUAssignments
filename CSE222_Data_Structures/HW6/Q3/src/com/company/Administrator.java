package com.company;

import java.util.*;

public class Administrator extends User {
    BookStore bookStore;
    String password;

    public Administrator(BookStore bookStore, String pasword) {
        this.bookStore = bookStore;
        this.password = pasword;
    }
    public boolean login() {
        Scanner scanner = new Scanner(System.in);
        String pw;

        System.out.println("Please enter your password.");
        pw = scanner.nextLine();

        if(pw.matches(this.password))
            return true;
        return false;
    }
    @Override
    public void menu() {
        System.out.println("1. Add new book.");
        System.out.println("2. Delete book.");
        System.out.println("3. Update book.");
    }

    public boolean addBook() {
        Scanner scanner = new Scanner(System.in);

        String authorName,bookName,locationName;
        System.out.println("Enter author name");

        authorName = scanner.nextLine();

        System.out.println("Enter book name");
        bookName = scanner.nextLine();

        System.out.println("Enter a location ");
        locationName = scanner.nextLine();

        if( locationExist(locationName) ) {
            System.out.println("This location is full.");
            return false;
        }
        Map<String, Map<String, Set<String>>> map = bookStore.getMap();
        Map<String,Set<String>> bookMap;
        Set<String> locationSet;

        if(map.containsKey(authorName)) { //yazar varsa
            bookMap = map.get(authorName); //o yazarın kitapları
            if(bookMap.containsKey(bookName)) { //bu kitap varsa
                locationSet = bookMap.get(bookName); // bu kitabın yerleri
                if(locationSet.contains(locationName)) { //bu yer varsa
                    System.out.println("This book already recorded!");
                    return false;
                }
                else {
                    locationSet.add(locationName);
                }
            }
            else {
                locationSet = new HashSet<>();
                locationSet.add(locationName);
                bookMap.put(bookName,locationSet);
                System.out.println("Location Added!");

            }
        } else {
            locationSet = new HashSet<>();
            locationSet.add(locationName);

            bookMap = new HashMap<>();
            bookMap.put(bookName,locationSet);
            map.put(authorName,bookMap);
            System.out.println("Successful Record!");
        }
        bookStore.setMap(map);
        return true;
    }

    private boolean locationExist(String location) {
        Map<String, Map<String, Set<String>>> map = bookStore.getMap();
        for(Map.Entry<String, Map<String,Set<String>>> t :map.entrySet()) {
            String key = t.getKey();
            for ( Map.Entry< String, Set< String > > e : t.getValue().entrySet() ) {
                if ( e.getValue().contains(location) ) {
                    return true;
                }
            }
        }
        return false;
    }

    public boolean deleteBook() {
        Scanner scanner = new Scanner(System.in);

        String bookName;
        System.out.println("Enter book name");
        bookName = scanner.nextLine();

        Map<String, Map<String, Set<String>>> map = bookStore.getMap();

        for(Map.Entry<String, Map<String,Set<String>>> t :map.entrySet()){
            String key = t.getKey();
            for (Map.Entry<String ,Set<String >> e : t.getValue().entrySet()) {
                if(e.getKey().matches(bookName)) {
                    Set<String> locationSet = map.get(key).get(bookName); //yazarın kitaplarının konumları
                    //yazarı da sil
                    if(map.get(key).size() == 1) {
                        System.out.println("Successful Deletion!");
                        map.remove(key);
                        bookStore.setMap(map);
                        return true;
                    }
                    else {
                        System.out.println("One of the locations has been deleted!");
                        map.get(key).remove(bookName);
                        bookStore.setMap(map);
                        return true;
                    }
                }
            }
        }
        System.out.println("No record!");
        return false;
    }

    public boolean updateBook() {
        Scanner scanner = new Scanner(System.in);
        System.out.println("1. Update author name");
        System.out.println("2. Update book title");
        System.out.println("3. Update book's locations");

        Map<String, Map<String, Set<String>>> map = bookStore.getMap();
        Map<String,Set<String>> bookMap;
        Set<String> locationSet;
        String authorName, bookName, location;
        int item;
        try {
            item = Integer.parseInt( scanner.nextLine() );
        }catch ( NumberFormatException e ) {
            System.out.println("Error ! "+ e);
            return false;
        }
        if(item == 1) {
            System.out.println("Enter the old author name.");
            authorName = scanner.nextLine();
            if(! map.containsKey(authorName)) {
                System.out.println("The author with this name is not recorded.");
                return false;
            }
            bookMap = map.get(authorName);
            map.remove(authorName);
            System.out.println("Enter the new author name.");
            authorName = scanner.nextLine();
            map.put(authorName,bookMap);
            System.out.println("Successful Update!");
            bookStore.setMap(map);
            return true;
        }
        else if(item == 2) {
            System.out.println("Enter the old book title.");
            bookName = scanner.nextLine();

            for(Map.Entry<String, Map<String,Set<String>>> t :map.entrySet()) {
                String key = t.getKey();
                for ( Map.Entry< String, Set< String > > e : t.getValue().entrySet() ) {
                    if ( e.getKey().matches(bookName) ) {
                        locationSet = map.get(key).get(bookName);
                        map.get(key).remove(bookName);
                        System.out.println("Enter the new book title.");
                        bookName = scanner.nextLine();
                        map.get(key).put(bookName,locationSet);

                        bookStore.setMap(map);
                        System.out.println("Successful Update!");
                        return true;
                    }
                }
            }
            System.out.println("No record!");
            return false;
        }
        else if(item == 3) {
            System.out.println("Enter the old location.");
            location = scanner.nextLine();

            for(Map.Entry<String, Map<String,Set<String>>> t :map.entrySet()) {
                String key = t.getKey();
                for ( Map.Entry< String, Set< String > > e : t.getValue().entrySet() ) {
                    if ( e.getValue().contains(location) ) {
                        locationSet = map.get(key).get(e.getKey());
                        locationSet.remove(location);

                        System.out.println("Enter the new location.");
                        location = scanner.nextLine();
                        locationSet.add(location);

                        bookStore.setMap(map);
                        System.out.println("Successful Update!");
                        return true;
                    }
                }
            }
            System.out.println("No record!");
            return false;
        } else {
            System.out.println("Invalid choice!");
            return false;
        }
    }

}
