package com.company;

public class AgeData implements Comparable<AgeData> {

    public int age;
    public int numberOfPeople = 0;

    /**
     * Called in case the wanted element is not found.
     */
    AgeData(){
        age = -1;
    }

    /**
     * to create data
     * @param age if invalid value sets the age variable, otherwise throw exception
     */
    AgeData(int age){
        try {
            if (age < 0)
                throw new Exception("Age cannot be less than 0 !");
        }catch ( Exception e ) {
            System.out.println(e.getMessage());
            return;
        }
        this.age = age;
        numberOfPeople++;
    }

    /**
     * It compares the value in the parameter with the age variable.
     * @param o element to compare
     * @return if age is small 1, if bigger -1, equal 0
     */
    @Override
    public int compareTo(AgeData o) {
        if( age < o.age )
            return -1;
        else if(age > o.age)
            return 1;
        return 0;
    }

    /**
     * to print the values as desired
     * @return age and numberOfPeople values as string
     */
    @Override
    public String toString() {
        return age + " - " + numberOfPeople;
    }
}
