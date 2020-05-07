package test.company;

import com.company.*;
import org.junit.Test;
import org.junit.jupiter.api.Assertions;

import java.util.ArrayList;

public class MainTest {

    @Test
    public void findShipmentInstanceInShipmentArray(){
        ArrayList<Shipment> shipments = new ArrayList<Shipment>();
        Shipment shipment = new Shipment("receiver","sender","info");
        shipments.add(shipment);

        Assertions.assertEquals(shipment, Main.findShipment(shipments,shipment.getTrackingNumber()));
    }

     @Test
    public void findBranchInstanceInBranchArray(){
        ArrayList<Branch> branches = new ArrayList<Branch>();
        Branch branch = new Branch("branchname");
        branches.add(branch);

        Assertions.assertEquals(branch, Main.findBranch(branches,branch.getName()));
    }

    @Test
    public void findUserInstanceInUserArray(){
        ArrayList<User> users = new ArrayList<User>();
        Administrator administrator = new Administrator("id","password");
        users.add(administrator);

        Assertions.assertEquals(administrator, Main.findUser(users,administrator.getId(),administrator.getPassword()));
    }





}