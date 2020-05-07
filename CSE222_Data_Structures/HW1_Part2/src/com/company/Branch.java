package com.company;

import java.util.ArrayList;

public class Branch {

    private String name;
    ArrayList<BranchEmployee> branchEmployeesArray = new ArrayList<BranchEmployee>();
    ArrayList<TransportationPersonnel> transportationPersonnelArray = new ArrayList<TransportationPersonnel>();

    public Branch(String name) {
        this.name = name;
    }

    /**
     * getter of Branch Name
     * @return String
     */
    public String getName() {
        return name;
    }

    public ArrayList<BranchEmployee> getBranchEmployeesArray() {
        return branchEmployeesArray;
    }

    public ArrayList<TransportationPersonnel> getTransportationPersonnelArray() {
        return transportationPersonnelArray;
    }

    /**
     * Checks for wanted branch employee in the list.
     * @param _id branch employee id that given by user
     * @return branch employee instance or null
     */
    public BranchEmployee findBranchEmployee(String _id) {

        for (int i = 0; i < branchEmployeesArray.size(); i++) {
            if( branchEmployeesArray.get(i).id.equals(_id) ) {
                return branchEmployeesArray.get(i);
            }
        }
        return null;
    }

    /**
     * Checks for wanted transprtation personnel in the list.
     * @param _id transportation personnel id that given by user.
     * @return transportation personnel instance or null
     */
    public TransportationPersonnel findTransportationPersonnel(String _id) {

        for (int i = 0; i < transportationPersonnelArray.size(); i++) {
            if( transportationPersonnelArray.get(i).id.equals(_id) ) {
                return transportationPersonnelArray.get(i);
            }
        }
        return null;
    }


}
