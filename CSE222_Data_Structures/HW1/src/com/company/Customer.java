package com.company;

public class Customer extends User {

    public Customer(String _id, String _password) {
        id= _id;
        password = _password;
   }

    /**
     * This menu displays actions of Customer.
     */
    @Override
    public void menu() {

        System.out.println("1.Check your package information : ");
        System.out.println("0. Change user");
        System.out.println("-1. Exit");    }
}
