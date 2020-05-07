package com.company;

public class TransportationPersonnel extends User {
    private String branchName;

    public TransportationPersonnel(String _id, String _password, String _branchName) {
        id = _id;
        password = _password;
        branchName = _branchName;
    }

    /**
     * This menu displays actions of Tranportation Personnel
     */
    @Override
    public void menu() {
        System.out.println("1. Update package status");
        System.out.println("0. Change user");
        System.out.println("-1. Exit");
    }
}
