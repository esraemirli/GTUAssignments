package com.company;

import java.util.Objects;

public abstract class User {

    String password;
    String id;

    /**
     * base of users
     */
    public abstract void menu();

    /**
     * getter of user id
     * @return String
     */
    public String getId() {
        return id;
    }

    public String getPassword() {
        return password;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        User user = (User) o;
        return Objects.equals(password, user.password) &&
                Objects.equals(id, user.id);
    }

    @Override
    public int hashCode() {
        return Objects.hash(password, id);
    }
}
