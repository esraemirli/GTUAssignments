package com.company;

import java.util.Random;

public class Shipment {

    private int trackingNumber;
    private String receiver;
    private String sender;
    private String status;
    private  String info;

    public Shipment(String receiver, String sender,String info) {
        this.receiver = receiver;
        this.sender = sender;
        this.info = info;
        this.status = "Your package set out to be delivered.";
        this.trackingNumber = new Random().nextInt((100000 - 10000) + 1) + 100000;
        System.out.println(trackingNumber);
    }

    public Shipment() {
        this.status = "Your package set out to be delivered.";
        this.trackingNumber = new Random().nextInt((100000 - 10000) + 1) + 100000;
        System.out.println(trackingNumber);
    }

    /**
     * getter of Tracking Number of Shipment
     * @return int
     */
    public int getTrackingNumber() {
        return trackingNumber;
    }


    public void setTrackingNumber(int trackingNumber) {
        this.trackingNumber = trackingNumber;
    }

    /**
     * getter of Information of Shipment
     * @return String
     */
    public String getInfo() {
        return info;
    }

    public void setInfo(String info) {
        this.info = info;
    }

    public void setReceiver(String receiver) {
        this.receiver = receiver;
    }

    public void setSender(String sender) {
        this.sender = sender;
    }

    /**
     * setter of current status of shipment
     * @param status given status by employee
     */
    public void setStatus(String status) {
        this.status = status;
    }

    /**
     * getter of receiver information
     * @return String;
     */
    public String getReceiver() {
        return receiver;
    }

    /**
     * getter of sender information
     * @return String
     */
    public String getSender() {
        return sender;
    }

    /**
     * getter of status of shipment
     * @return String
     */
    public String getStatus() {
        return status;
    }
}
