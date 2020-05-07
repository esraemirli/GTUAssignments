package com.company;

import java.util.ArrayList;
import java.util.Scanner;

public class Main {

     /**
     * Returns the matching instance tracking numbers.
     * @param _shipmentArray Searching in it.
     * @param _trackingNumber Searching this.
     * @return Shipment or null
     */
    public static Shipment findShipment(ArrayList<Shipment> _shipmentArray, int _trackingNumber){
        for (int i = 0; i <  _shipmentArray.size(); i++) {
            if( _shipmentArray.get(i).getTrackingNumber() == _trackingNumber) {
                return _shipmentArray.get(i);
            }
        }
        return null;
    }


    /**
     * Returns the matching instance tracking numbers.
     * @param _branchArray Searching in it.
     * @param _name Searching this
     * @return Branch instance or null
     */
    public static Branch findBranch(ArrayList<Branch> _branchArray, String _name){
        for (int i = 0; i < _branchArray.size(); i++) {
            if(_branchArray.get(i).getName().equals(_name) ) {
                return _branchArray.get(i);
            }
        }
        return null;
    }

    /**
     * Returns the matching instance tracking numbers.
     * @param _userArray Searching in it.
     * @param _id Searching this
     * @param  _password Searching this
     * @return Branch instance or null
     */
    public static User findUser(ArrayList<User> _userArray, String _id, String _password){
        for (int i = 0; i < _userArray.size(); i++) {
            if(_userArray.get(i).id.equals(_id) && _userArray.get(i).password.equals(_password)) {
                return _userArray.get(i);
            }
        }
        return null;
    }

    /**
     * User types are determined and the operations that he/she wants to do according to the individual menus are selected.
     * @param args not important
     */
    public static void main(String[] args) {

        ArrayList<User> userArray = new ArrayList<User>();
        ArrayList<Shipment> shipmentsArray = new ArrayList<Shipment>();
        ArrayList<Branch> branchArray = new ArrayList<Branch>();

        Administrator administrator;
        BranchEmployee branchEmployee;
        TransportationPersonnel transportationPersonnel;
        Customer customer;

        // add to first administrator to userArray..
        Scanner scanner = new Scanner(System.in);
        User admin = new Administrator("root", "1");
        userArray.add(admin);


        String userID, password;
        int chooseUser = 0;

        while (chooseUser != -1) {
            int choose = -1;
            System.out.println("1. Login as Administrator");
            System.out.println("2. Login as Branch Employee");
            System.out.println("3. Login as Transportation Personnel");
            System.out.println("4. Login as Customer");
            System.out.println("-1. Exit");
            chooseUser = scanner.nextInt();


            if( chooseUser != -1) {
                scanner.nextLine();
                System.out.print("Please enter your ID :");
                userID = scanner.nextLine();
                System.out.print("Please enter your password :");
                password = scanner.nextLine();

                User user = findUser(userArray, userID, password);
                if (user instanceof Administrator && chooseUser == 1) {
                    administrator = (Administrator) user;
                    while (choose != 0) {
                        administrator.menu();
                        choose = scanner.nextInt();

                        switch (choose) {
                            case 1:
                                scanner.nextLine();
                                System.out.print("Please enter the branch name :");
                                String branchName = scanner.nextLine();
                                Branch branch = new Branch(branchName);
                                branchArray.add(branch);
                                System.out.println("Branch " + branchName + " Recorded !");
                                break;

                            case 2:
                                scanner.nextLine();
                                System.out.print("Please enter the branch name :");
                                branchName = scanner.nextLine();
                                branch = findBranch(branchArray, branchName);
                               try{
                                    if(branch == null)
                                        throw new NullPointerException();
                                    else {
                                        branchArray.remove(branch);
                                        System.out.println("Branch " + branchName + " Removed !");
                                    }
                                }
                                catch(NullPointerException e){
                                    System.out.println("Removed failed : There is no branch with this name.");
                                }catch (Exception e){
                                   System.out.println(e.getMessage());
                               }
                               break;

                            case 3:
                                scanner.nextLine();
                                System.out.print("Please enter the branch employee name :");
                                String id = scanner.nextLine();
                                System.out.print("Please enter the branch name :");
                                branchName = scanner.nextLine();
                                branchEmployee = new BranchEmployee(id, branchName + "2020BE", branchName);
                                branch = findBranch(branchArray, branchName);
                                try {
                                    if(branch == null)
                                        throw new NullPointerException();
                                    else{
                                        branch.getBranchEmployeesArray().add(branchEmployee);
                                        userArray.add(branchEmployee);
                                        System.out.println("Branch Employee " + id + " Recorded !");
                                    }
                                }catch (NullPointerException e){
                                    System.out.println("Recorded failed : There is no branch whit this name.");
                                }catch (Exception e){
                                    System.out.println(e.getMessage());
                                }
                                break;

                            case 4:
                                scanner.nextLine();
                                System.out.print("Please enter the branch employee name :");
                                id = scanner.nextLine();
                                System.out.print("Please enter the branch name :");
                                branchName = scanner.nextLine();
                                branch = findBranch(branchArray, branchName);
                                try {
                                    branchEmployee = branch.findBranchEmployee(id);
                                    if( branch == null || branchEmployee == null)
                                        throw new  NullPointerException();
                                    else{
                                        branch.getBranchEmployeesArray().remove(branchEmployee);
                                        userArray.remove(branchEmployee);
                                        System.out.println("Branch Employee " + id + " Removed !");
                                    }
                                }catch (NullPointerException e){
                                    System.out.println("Removed failed : There is no branch or employee with this name.");
                                }catch (Exception e){
                                    System.out.println(e.getMessage());
                                }
                                break;

                            case 5:
                                scanner.nextLine();
                                System.out.print("Please enter the transportation personnel name :");
                                id = scanner.nextLine();
                                System.out.print("Please enter the branch name :");
                                branchName = scanner.nextLine();
                                transportationPersonnel = new TransportationPersonnel(id, branchName + "2020TP", branchName);
                                branch = findBranch(branchArray, branchName);
                                try{
                                    if(branch == null)
                                        throw new NullPointerException();
                                    else{
                                        branch.getTransportationPersonnelArray().add(transportationPersonnel);
                                        userArray.add(transportationPersonnel);
                                        System.out.println("Transportation Personnel " + id + " Recorded !");
                                    }
                                } catch (NullPointerException e){
                                    System.out.println("Recorded failed : There is no branch with this name.");
                                }catch (Exception e){
                                    System.out.println(e.getMessage());
                                }
                                break;

                            case 6:
                                scanner.nextLine();
                                System.out.print("Please enter the transportation personnel name :");
                                id = scanner.nextLine();
                                System.out.print("Please enter the branch name :");
                                branchName = scanner.nextLine();
                                branch = findBranch(branchArray, branchName);
                                try{
                                    transportationPersonnel = branch.findTransportationPersonnel(id);
                                    if(branch == null || transportationPersonnel==null)
                                        throw new NullPointerException();
                                    else{
                                        branch.getTransportationPersonnelArray().remove(transportationPersonnel);
                                        userArray.remove(transportationPersonnel);
                                        System.out.println("Transportation Personnel " + id + " Removed !");
                                    }
                                } catch (NullPointerException e){
                                    System.out.println("Removed failed : There is no branch or personnel with this name.");
                                } catch (Exception e){
                                    System.out.println("!!!!!");
                                    System.out.println(e.getMessage());
                                }
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
                                scanner.nextLine();
                                System.out.print("Please enter the receiver name :");
                                String receiver  = scanner.nextLine();
                                System.out.print("Please enter the sender name :");
                                String sender = scanner.nextLine();
                                System.out.print("Please enter the information for shipment :");
                                String info = scanner.nextLine();
                                Shipment shipment = new Shipment(receiver, sender, info);
                                shipmentsArray.add(shipment);
                                System.out.println("Shipment " + sender + " to " + receiver + " Recorded !");
                                break;

                            case 2:
                                scanner.nextLine();
                                System.out.print("Please enter the tracking number:");
                                int trackingNumber = scanner.nextInt();
                                shipment = findShipment(shipmentsArray, trackingNumber);
                                try{
                                    if(shipment == null)
                                        throw new NullPointerException();
                                    else{
                                        shipmentsArray.remove(shipment);
                                        System.out.println("Shipment (" + trackingNumber  + ") Removed !");
                                    }
                                }catch (NullPointerException e){
                                   System.out.println("Removed failed : There is no shipment with this tracking number.");
                                }catch (Exception e){
                                   System.out.println(e.getMessage());
                                }
                                break;

                            case 3:
                                scanner.nextLine();
                                System.out.print("Please enter customer name:");
                                String id = scanner.nextLine();
                                System.out.print("Please enter customer password:");
                                password = scanner.nextLine();
                                customer = new Customer(id, password);
                                userArray.add(customer);
                                System.out.println("Customer " + id + " Recorded !");
                                break;

                            case 4:
                                scanner.nextLine();
                                System.out.print("Please enter customer name:");
                                id = scanner.nextLine();
                                System.out.print("Please enter customer password:");
                                password = scanner.nextLine();
                                customer = (Customer) findUser(userArray, id, password);
                                try{
                                    if(customer == null)
                                        throw new NullPointerException();
                                    else{
                                        userArray.remove(customer);
                                        System.out.println("Customer " + id + " Removed !");
                                    }
                                }catch (NullPointerException e){
                                    System.out.println("Removed failed : There is no customer with this name.");
                                }catch (Exception e){
                                    System.out.println(e.getMessage());
                                }
                                break;

                            case 5:
                                scanner.nextLine();
                                System.out.print("Please enter package tracking number :");
                                trackingNumber = scanner.nextInt();
                                scanner.nextLine();
                                System.out.print("Please enter package status :");
                                String status = scanner.nextLine();
                                shipment = findShipment(shipmentsArray, trackingNumber);
                                try {
                                    if(shipment == null)
                                        throw new NullPointerException();
                                    else{
                                        shipment.setStatus(status);
                                        System.out.println("Status Recorded !" );
                                    }
                                }catch (NullPointerException e){
                                    System.out.println("Recorded failed : There is no shipment with this tracking number .");
                                }catch (Exception e){
                                    System.out.println(e.getLocalizedMessage().toString());
                                }
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
                                scanner.nextLine();
                                System.out.print("Please enter the tracking number :");
                                int trackingNumber = scanner.nextInt();
                                scanner.nextLine();
                                System.out.print("Please enter package status :");
                                String status = scanner.nextLine();
                                Shipment shipment = findShipment(shipmentsArray, trackingNumber);
                                try{
                                    if(shipment == null)
                                        throw new NullPointerException();
                                    else{
                                        shipment.setStatus(status);
                                        System.out.println("Status Updated !");
                                    }
                                }catch (NullPointerException e){
                                    System.out.println("Update failed: There is no shipment with this tracking number.");
                                }catch (Exception e){
                                    System.out.println(e.getMessage());
                                }
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
                                try{
                                    if(shipment == null)
                                        throw new NullPointerException();
                                    else{
                                        System.out.println(shipment.getSender());
                                        System.out.println(shipment.getReceiver());
                                        System.out.println(shipment.getStatus());
                                        System.out.println(shipment.getInfo());
                                    }
                                } catch (NullPointerException e){
                                    System.out.println("There is no shipment with this tracking number.");
                                }catch (Exception e){
                                    System.out.println(e.getMessage());
                                }
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
