package com.company;

public class Branch {

    private String name;
    public BranchEmployee[] branchEmployeesArray = new BranchEmployee[100];
    public TransportationPersonnel[] transportationPersonnelsArray = new TransportationPersonnel[100];

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

    /**
     * adds to the appropriate array. (branchEmployeesArray or transportationPersonnelsArray)
     * @param _array instance of Branch Employee or Transportation Personnel
     * @return boolean
     */
    public boolean addToArray(User _array){
        if( _array instanceof BranchEmployee){
            BranchEmployee _branchEmployeeArray = (BranchEmployee) _array;
            for (int i = 0;; i++) {
                if(this.branchEmployeesArray[i] == null){
                    this.branchEmployeesArray[i] = _branchEmployeeArray;
                    this.branchEmployeesArray[i+1] = null;
                    return true;
                }
            }
        }
        else if( _array instanceof TransportationPersonnel){
            TransportationPersonnel transportationPersonnel = (TransportationPersonnel) _array;
            for (int i = 0;; i++) {
                if(this.transportationPersonnelsArray[i] == null){
                    this.transportationPersonnelsArray[i] = transportationPersonnel;
                    this.transportationPersonnelsArray[i+1] = null;
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * removes to the appropriate array. (branchEmployeesArray or transportationPersonnelsArray)
     * @param _instance instance of Branch Employee or Transportation Personnel
     */
    public boolean removeToArray( User _instance) {

        if( _instance instanceof BranchEmployee) {
            for (int i = 0; this.branchEmployeesArray[i] != null; i++) {
                if (this.branchEmployeesArray[i].id == _instance.id) {

                    for (int j = i; this.branchEmployeesArray[j] != null; j++) {
                        this.branchEmployeesArray[j] = this.branchEmployeesArray[j + 1];
                    }
                    return true;
                }
            }
        }
        else if( _instance instanceof TransportationPersonnel){
            for (int i = 0; this.transportationPersonnelsArray[i] != null; i++) {
                if (this.transportationPersonnelsArray[i].id == _instance.id) {

                    for (int j = i; this.transportationPersonnelsArray[j] != null; j++) {
                        this.transportationPersonnelsArray[j] = this.transportationPersonnelsArray[j + 1];
                    }
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * checks this instance is exist or not in arrays looking with parameter.
     * @param _id Branch Employee or Transportation Employee id
     * @return User
     */
    public User findEmployee(String _id){
        for (int i = 0;  this.branchEmployeesArray[i] != null; i++) {

            if( this.branchEmployeesArray[i].id.equals(_id) ) {
                return  this.branchEmployeesArray[i];
            }
        }

        for (int i = 0;  this.transportationPersonnelsArray[i]!=null; i++) {
            if( this.transportationPersonnelsArray[i].id.equals(_id) ) {
                return  this.transportationPersonnelsArray[i];
            }
        }

        return null;
    }
}
