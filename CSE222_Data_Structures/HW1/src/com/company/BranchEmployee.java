package com.company;

public class BranchEmployee extends User {
    private String branchName;

    public BranchEmployee(String _id, String _password, String _branchName) {
        id = _id;
        password = _password;
        branchName = _branchName;
    }

    /**
     * This menu displays actions of Branch Employee.
     */
    @Override
    public void menu() {
        System.out.println("1. Enter information about the shipments and sender and receiver name");
        System.out.println("2. Remove shipment");
        System.out.println("3. Add users (customers) to the system.");
        System.out.println("4. Remove users (customers) to the system");
        System.out.println("5. Package current status is entered into the system");
        System.out.println("0. Change user");
        System.out.println("-1. Exit");    }
}
