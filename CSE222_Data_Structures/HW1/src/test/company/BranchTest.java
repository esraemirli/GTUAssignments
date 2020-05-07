package test.company;

import com.company.Branch;
import com.company.BranchEmployee;
import com.company.TransportationPersonnel;
import com.company.User;
import org.junit.Test;
import org.junit.jupiter.api.Assertions;

import static org.junit.Assert.*;

public class BranchTest {

    @Test
    public void addBranchEmployeeToArrayTest() {
        Branch branch = new Branch("branchname");
        BranchEmployee branchEmployee = new BranchEmployee("id","pw","branchname");

        Assertions.assertTrue(branch.addToArray(branchEmployee));
    }

    @Test
    public void addTranspotationPersonnelToArrayTest(){
        Branch branch = new Branch("branchname");
        TransportationPersonnel transportationPersonnel = new TransportationPersonnel("id","pw","branchname");

        Assertions.assertTrue(branch.addToArray(transportationPersonnel));
    }

    @Test
    public void removeBranchEmployeeToArrayTest() {
        Branch branch = new Branch("branchname");
        BranchEmployee branchEmployee = new BranchEmployee("id","pw","branchname");
        branch.addToArray(branchEmployee);
        branch.removeToArray(branchEmployee);
    }

    @Test
    public void removeTranspotationPersonnelToArrayTest(){
        Branch branch = new Branch("branchname");
        TransportationPersonnel transportationPersonnel = new TransportationPersonnel("id","pw","branchname");
        branch.addToArray(transportationPersonnel);
        branch.removeToArray(transportationPersonnel);
    }

    @Test
    public void findBranchEmployeeInBranchEmployeesAray(){
        Branch branch = new Branch("branchname");
        BranchEmployee branchEmployee = new BranchEmployee("id","pw","branchname");
        branch.addToArray(branchEmployee);

        Assertions.assertEquals(branchEmployee, branch.findEmployee(branchEmployee.getId()));
    }

    @Test
    public void findTransportationEmployeeInTransportationPersonnelsAray(){
        Branch branch = new Branch("branchname");
        TransportationPersonnel transportationPersonnel = new TransportationPersonnel("id","pw","branchname");
        branch.addToArray(transportationPersonnel);

        Assertions.assertEquals(transportationPersonnel,branch.findEmployee(transportationPersonnel.getId()));
    }

}