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
    public void findBranchEmployeeInBranchEmployeesAray(){
        Branch branch = new Branch("branchname");
        BranchEmployee branchEmployee = new BranchEmployee("id","pw","branchname");
        branch.getBranchEmployeesArray().add(branchEmployee);

        Assertions.assertEquals(branchEmployee, branch.findBranchEmployee( branchEmployee.getId() )  );
    }

    @Test
    public void findTransportationEmployeeInTransportationPersonnelsAray(){
        Branch branch = new Branch("branchname");
        TransportationPersonnel transportationPersonnel = new TransportationPersonnel("id","pw","branchname");
        branch.getTransportationPersonnelArray().add(transportationPersonnel);

        Assertions.assertEquals(transportationPersonnel,branch.findTransportationPersonnel(transportationPersonnel.getId()));
    }

}