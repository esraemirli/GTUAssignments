package com.company;

import java.util.Scanner;

public class Main {

    /**
     * adds the shipment instance to shipment array
     * @param _array shipment array
     * @param _instance shipment instance
     * @return Shipment Array
     */
    public static Shipment[] addShipment(Shipment[] _array, Shipment _instance){
        for (int i = 0; ; i++) {
            if(_array[i] == null ) {
                _array[i] = _instance;
                _array[i+1] = null;
                return _array;
            }
        }
    }

    /**
     * removes the shipment instance to shipment array
     * @param _array shipment array
     * @param _instance shipment instance
     * @return Shipment Array
     */
    public static Shipment[] removeShipment(Shipment[] _array, Shipment _instance){
        for (int i = 0; _array[i]!=null ; i++) {
            if(_array[i].getTrackingNumber() == _instance.getTrackingNumber()){
                for (int j = i; _array[j]!=null ; j++) {
                    _array[j] = _array[j+1];
                }
                return _array;
            }
        }
        return _array;
    }

    /**
     * Returns the matching instance tracking numbers.
     * @param _shipmentArray Searching in it.
     * @param _trackingNumber Searching this.
     * @return Shipment
     */
    public static Shipment findShipment(Shipment[] _shipmentArray, int _trackingNumber){
        for (int i = 0; _shipmentArray[i] != null; i++) {
            if( _shipmentArray[i].getTrackingNumber() == _trackingNumber) {
                return _shipmentArray[i];
            }
        }
        return null;
    }

    /**
     * This method is doing add and remove process to branch  array
     * @param _array branch  array
     * @param _instance branch instance
     * @param _todo add or remove process
     * @return branch array
     */
    public static Branch[] updateToBranchArray(Branch[] _array, Branch _instance, String _todo) {
        if( _todo.equals("remove") ){
            for (int i = 0; _array[i]!=null ; i++) {
                if(_array[i].getName() == _instance.getName()){
                    for (int j = i; _array[j]!=null ; j++) {
                        _array[j] = _array[j+1];
                    }
                    return _array;
                }
            }
        }
        //Kaydet.. Size'a dikkat et...
        for (int i = 0; ; i++) {
            if(_array[i] == null ) {
                _array[i] = _instance;
                _array[i+1] = null;
                return _array;
            }
        }
    }

    /**
     * Returns the matching instance tracking numbers.
     * @param _branchArray Searching in it.
     * @param _name Searching this
     * @return Branch instance
     */
    public static Branch findBranch(Branch[] _branchArray, String _name){
        for (int i = 0; _branchArray[i] != null; i++) {
            if(_branchArray[i].getName().equals(_name) ) {
                return _branchArray[i];
            }
        }
        return null;
    }

    /**
     * This method is doing add and remove user instance (administrator,customer..) to user array
     * @param _array user array
     * @param _instance user instance
     * @param _todo add or remove process
     * @return user array
     */
    public static User[] updateToUserArray(User[] _array, User _instance, String _todo) {
        if( _todo.equals("remove") ){
            for (int i = 0; _array[i]!=null ; i++) {
                if(_array[i].getId() == _instance.getId()){

                    for (int j = i; _array[j]!=null ; j++) {
                        _array[j] = _array[j+1];
                    }
                    return _array;
                }
            }
        }
        for (int i = 0; ; i++) {
            if(_array[i] == null ) {
                _array[i] = _instance;
                _array[i+1] = null;
                return _array;
            }
        }
    }
    /**
     * Returns the matching instance tracking numbers.
     * @param _userArray Searching in it.
     * @param _id Searching this
     * @param  _password Searching this
     * @return Branch instance
     */
    public static User findUser(User[] _userArray, String _id, String _password){
        for (int i = 0; _userArray[i] != null; i++) {
            if(_userArray[i].id.equals(_id) && _userArray[i].password.equals(_password)) {
                return _userArray[i];
            }
        }
        return null;
    }

    /**
     * User types are determined and the operations that he/she wants to do according to the individual menus are selected.
     * @param args
     */
    public static void main(String[] args) {
        User[] userArray = new User[20];
        Branch[] branchArray = new Branch[20];
        Shipment[] shipmentsArray = new Shipment[20];
        Administrator administrator;
        BranchEmployee branchEmployee;
        TransportationPersonnel transportationPersonnel;
        Customer customer;
        // add to first administrator to userArray..
        Scanner scanner = new Scanner(System.in);
        User admin = new Administrator("root", "1");
        userArray = updateToUserArray(userArray,admin,"add");


        String userID, password;
        int chooseUser = 0;

        while (chooseUser != -1) {
            int choose = -1;
            System.out.println("1. Login as Administrator");
            System.out.println("2. Login as Bracnh Employee");
            System.out.println("3. Login as Transportation Personnel");
            System.out.println("4. Login as Customer");
            System.out.println("-1. Exit");
            chooseUser = scanner.nextInt();

            if( chooseUser != -1) {
                System.out.print("Please enter your ID :");
                userID = scanner.next();
                System.out.print("Please enter your password :");
                password = scanner.next();

                User user = findUser(userArray, userID, password);
                if (user instanceof Administrator && chooseUser == 1) {
                    administrator = (Administrator) user;
                    while (choose != 0) {
                        administrator.menu();
                        choose = scanner.nextInt();

                        switch (choose) {
                            case 1:
                                System.out.print("Please enter the branch name :");
                                String branchName = scanner.next();
                                Branch branch = new Branch(branchName);
                                branchArray = updateToBranchArray(branchArray, branch, "add");
                                if(branchArray != null)
                                    System.out.println("Added!");
                                break;

                            case 2:
                                System.out.print("Please enter the branch name :");
                                branchName = scanner.next();
                                branch = findBranch(branchArray, branchName);
                                if(branch != null) {
                                    branchArray = updateToBranchArray(branchArray, branch, "remove");
                                    if (branchArray != null)
                                        System.out.println("Removed!");
                                }
                                else
                                    System.out.println("No branch registered with this name can be found");
                                break;

                            case 3:
                                System.out.print("Please enter the branch employee name :");
                                String id = scanner.next();
                                System.out.print("Please enter the branch name :");
                                branchName = scanner.next();
                                branchEmployee = new BranchEmployee(id, branchName + "2020BE", branchName);
                                branch = findBranch(branchArray, branchName);
                                if (branch != null) {
                                    if( branch.addToArray(branchEmployee) ) {
                                        System.out.println("Added");
                                        userArray = updateToUserArray(userArray, branchEmployee, "add");
                                    }
                                } else
                                    System.out.println("No branch registered with this name can be found");
                                break;

                            case 4:
                                System.out.print("Please enter the branch employee name :");
                                id = scanner.next();
                                System.out.print("Please enter the branch name :");
                                branchName = scanner.next();
                                branch = findBranch(branchArray, branchName);
                                branchEmployee = (BranchEmployee) branch.findEmployee(id);
                                System.out.println(branchEmployee);
                                if (branch != null && branchEmployee != null) {
                                    if ( branch.removeToArray(branchEmployee) ){
                                        System.out.println("Removed");
                                        userArray = updateToUserArray(userArray, branchEmployee, "remove");
                                    }
                                } else
                                    System.out.println("No branch or branch employee registered with this name can be found");
                                break;

                            case 5:
                                System.out.print("Please enter the transportation personnel name :");
                                id = scanner.next();
                                System.out.print("Please enter the branch name :");
                                branchName = scanner.next();
                                transportationPersonnel = new TransportationPersonnel(id, branchName + "2020TP", branchName);
                                branch = findBranch(branchArray, branchName);
                                if (branch != null) {
                                    if( branch.addToArray(transportationPersonnel) ){
                                        System.out.println("Added");
                                        userArray = updateToUserArray(userArray, transportationPersonnel, "add");
                                    }
                                } else
                                    System.out.println("No branch registered with this name can be found");
                                break;

                            case 6:
                                System.out.print("Please enter the transportation personnel name :");
                                id = scanner.next();
                                System.out.print("Please enter the branch name :");
                                branchName = "maltepe"; //branchName = scanner.next();
                                branch = findBranch(branchArray, branchName);
                                transportationPersonnel = (TransportationPersonnel) branch.findEmployee(id);
                                System.out.println(transportationPersonnel);
                                if (branch != null && transportationPersonnel != null) {
                                    if( branch.removeToArray(transportationPersonnel) ){
                                        System.out.println("Removed");
                                        userArray = updateToUserArray(userArray, transportationPersonnel, "remove");
                                    }
                                } else
                                    System.out.println("No branch or branch employee registered with this name can be found");
                                break;
                            case 0:
                                break;
                            case -1:
                                choose =0;
                                chooseUser = -1 ;
                                break;
                            default:
                                System.out.println("Your choose did not found in menu");
                        }
                    }
                } else if (user instanceof BranchEmployee && chooseUser == 2) {
                    branchEmployee = (BranchEmployee) user;
                    while (choose != 0) {
                        branchEmployee.menu();
                        choose = scanner.nextInt();

                        switch (choose) {
                            case 1:
                                System.out.print("Please enter the receiver name :");
                                String receiver  = scanner.next();
                                System.out.print("Please enter the sender name :");
                                String sender = scanner.next();
                                System.out.print("Please enter the information for shipment :");
                                String info = scanner.next();
                                Shipment shipment = new Shipment(receiver, sender, info);
                                shipmentsArray = addShipment(shipmentsArray, shipment);
                                System.out.println("Added");
                                break;

                            case 2:
                                System.out.print("Please enter the tracking number:");
                                int trackingNumber = scanner.nextInt();
                                shipment = findShipment(shipmentsArray, trackingNumber);
                                if (shipment != null) {
                                    shipmentsArray = removeShipment(shipmentsArray, shipment);
                                    System.out.println("Removed");
                                }
                                else
                                    System.out.println("No shipment registered with this name can be found");
                                break;

                            case 3:
                                System.out.print("Please enter customer name:");
                                String id = scanner.next();
                                System.out.print("Please enter customer password:");
                                password = scanner.next();
                                customer = new Customer(id, password);
                                userArray = updateToUserArray(userArray, customer, "add");
                                System.out.println("Added");
                                break;

                            case 4:
                                System.out.print("Please enter customer name:");
                                id = scanner.next();
                                System.out.print("Please enter customer password:");
                                password = scanner.next();
                                customer = (Customer) findUser(userArray, id, password);
                                if( customer != null){
                                    userArray = updateToUserArray(userArray, customer, "remove");
                                    System.out.println("Removed");
                                }else{
                                    System.out.println("No customer registered with this name can be found");
                                }
                                break;

                            case 5:
                                System.out.print("Please enter package tracking number :");
                                trackingNumber = scanner.nextInt();
                                System.out.print("Please enter package status :");
                                String status = scanner.next();
                                shipment = findShipment(shipmentsArray, trackingNumber);
                                if(shipment != null) {
                                    shipment.setStatus(status);
                                    System.out.println("Added");
                                }
                                else
                                    System.out.println("No shipment registered with this name can be found");
                                break;

                            case 0:
                                break;
                            case -1:
                                choose =0;
                                chooseUser = -1 ;
                                break;
                            default:
                                System.out.println("Your choose did not found in menu");
                        }
                    }
                } else if (user instanceof TransportationPersonnel && chooseUser == 3) {
                    transportationPersonnel = (TransportationPersonnel) user;
                    while (choose != 0) {
                        transportationPersonnel.menu();
                        choose = scanner.nextInt();

                        switch (choose) {
                            case 1:
                                System.out.print("Please enter the tracking number :");
                                int trackingNumber = scanner.nextInt();
                                Shipment shipment = findShipment(shipmentsArray, trackingNumber);
                                if (shipment != null){
                                    shipment.setStatus("Teslim edildi");
                                    System.out.println("Updated");
                                }
                                else
                                    System.out.println("No shipment registered with this name can be found");
                                break;

                            case 0:
                                break;
                            case -1:
                                choose =0;
                                chooseUser = -1 ;
                                break;
                            default:
                                System.out.println("Your choose did not found in menu");
                                choose = 0;
                                System.out.println(choose);
                        }
                    }
                } else if (user instanceof Customer && chooseUser == 4) {
                    customer = (Customer) user;
                    while (choose != 0) {
                        customer.menu();
                        choose = scanner.nextInt();

                        switch (choose) {

                            case 1:
                                System.out.print("Please enter the tracking number :");
                                int trackingNumber = scanner.nextInt();
                                Shipment shipment = findShipment(shipmentsArray, trackingNumber);
                                if (shipment != null) {
                                    System.out.println(shipment.getSender());
                                    System.out.println(shipment.getReceiver());
                                    System.out.println(shipment.getStatus());
                                    System.out.println(shipment.getInfo());
                                } else
                                    System.out.println("No shipment registered with this name can be found");
                                break;

                            case 0:
                                break;
                            case -1:
                                choose =0;
                                chooseUser = -1 ;
                                break;
                            default:
                                System.out.println("Your choose did not found in menu");
                        }
                    }
                } else {
                    System.out.println("No User can be found!");
                }
            }
        }
    }
}
