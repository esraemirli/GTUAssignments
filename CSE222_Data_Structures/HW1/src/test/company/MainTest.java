package test.company;

import com.company.*;
import org.junit.Test;
import org.junit.jupiter.api.Assertions;

import static org.junit.Assert.*;

public class MainTest {
    @Test
    public void addShipmentInstanceToShipmentArray(){
        Shipment[] shipments = new Shipment[20];
        Shipment shipment = new Shipment("receiver","sender","info");
        Assertions.assertEquals(shipments, Main.addShipment(shipments,shipment));
    }
    @Test
    public void removeShipmentInstanceToShipmentArray(){
        Shipment[] shipments = new Shipment[20];
        Shipment shipment = new Shipment("receiver","sender","info");
        Assertions.assertEquals(shipments, Main.removeShipment(shipments,shipment));
    }
    @Test
    public void findShipmentInstanceInShipmentArray(){
        Shipment[] shipments = new Shipment[20];
        Shipment shipment = new Shipment("receiver","sender","info");
        Main.addShipment(shipments,shipment);

        Assertions.assertEquals(shipment, Main.findShipment(shipments,shipment.getTrackingNumber()));
    }

    @Test
    public void addBranchInBranchArray(){
        Branch[] branches = new Branch[20];
        Branch branch = new Branch("branchname");

        Assertions.assertEquals(branches,Main.updateToBranchArray(branches,branch,"add"));
    }
    @Test
    public void removeBranchInBranchArray(){
        Branch[] branches = new Branch[20];
        Branch branch = new Branch("branchname");
        Main.updateToBranchArray(branches,branch,"add");
        Assertions.assertEquals(branches,Main.updateToBranchArray(branches,branch,"remove"));
    }

    @Test
    public void findBranchInstanceInBranchArray(){
        Branch[] branches = new Branch[20];
        Branch branch = new Branch("branchname");
        Main.updateToBranchArray(branches,branch,"add");

        Assertions.assertEquals(branch, Main.findBranch(branches,branch.getName()));
    }

    @Test
    public void addUserInUserArray(){
        User[] users = new User[20];
        Administrator administrator = new Administrator("id","password");

        Assertions.assertEquals(users,Main.updateToUserArray(users,administrator,"add"));
    } @Test
    public void removeUserInUserArray(){
        User[] users = new User[20];
        Administrator administrator = new Administrator("id","password");
        Main.updateToUserArray(users,administrator,"add");

        Assertions.assertEquals(users,Main.updateToUserArray(users,administrator,"remove"));
    }

    @Test
    public void findUserInstanceInUserArray(){
        User[] users = new User[20];
        Administrator administrator = new Administrator("id","password");
        Main.updateToUserArray(users,administrator,"add");

        Assertions.assertEquals(administrator, Main.findUser(users,administrator.getId(),administrator.getPassword()));
    }





}