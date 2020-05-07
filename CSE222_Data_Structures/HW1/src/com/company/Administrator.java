package com.company;

public class Administrator extends User{
    public Administrator(String _id, String _password) {
        password = _password;
        id = _id;
    }

    /**
     * This menu displays actions of Administrator.
     */
    @Override
    public void menu() {
        System.out.println("1. Add branch");
        System.out.println("2. Remove branch");
        System.out.println("3. Add branch employee");
        System.out.println("4. Remove branch employee");
        System.out.println("5. Add transportation personnel");
        System.out.println("6. Remove transportation personnel");
        System.out.println("0. Change user");
        System.out.println("-1. Exit");
    }
}
