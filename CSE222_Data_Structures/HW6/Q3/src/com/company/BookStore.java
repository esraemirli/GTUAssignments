package com.company;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class BookStore {
        //yazar   //kitapadı   //konum
    Map<String, Map<String, Set<String>>> map;

    BookStore() {
        map = new HashMap< >();
    }

    public void print(){
        for(Map.Entry<String, Map<String,Set<String>>> t :this.map.entrySet()){
            String key = t.getKey();
            System.out.println("Author Name: " + key);
            for (Map.Entry<String ,Set<String >> e : t.getValue().entrySet())
                System.out.println("\n\tBook Name: " + e.getKey()+ "\n\t\tLocations:" +e.getValue());
            System.out.println("\n\t\tStatus: Available" );
        }
        if(map.isEmpty())
            System.out.println("Status: Not-Available!");
    }


    public Map< String, Map< String, Set< String > > > getMap() {
        return map;
    }

    public void setMap(Map< String, Map< String, Set< String > > > map) {
        this.map = map;
    }
}
