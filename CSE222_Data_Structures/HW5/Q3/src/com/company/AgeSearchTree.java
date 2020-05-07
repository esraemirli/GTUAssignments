package com.company;

import java.io.Serializable;

public class AgeSearchTree<E extends Comparable< E > > extends BinarySearchTree<E>{

    protected static class Node < E > implements Serializable {

        protected E data;

        protected Node< E > left;

        protected Node< E > right;

        public Node(E data) {
            this.data = data;
            left = null;
            right = null;
        }

        public String toString() {
            AgeData ageData = (AgeData) data;
            return String.valueOf(ageData.age) + " " + String.valueOf(ageData.numberOfPeople);
        }
    }
    private Node root;

    /**
     * Makes adding by calling the required method to add the
     * given ageData object to the appropriate place in the tree.
     * @param item ageData object
     * @return if registration is successful true, otherwise false
     */
    @Override
    public boolean add(E item) {
        root = add(root, item);
        return addReturn;
    }

    /**
     * Adds to the right or left of the tree by looking at the age value in the given item.
     * If that age already exists, it increases the number. It doesn't add new.
     * @param localRoot root of the tree to be saved
     * @param item ageData object to be saved in the tree
     * @return root of the tree from which adding is made
     */
    private Node< E > add(Node< E > localRoot, E item) {
        if (localRoot == null) {
            // item is not in the tree � insert it.
            addReturn = true;
            return new Node< E >(item);
        }
        else if (item.compareTo(localRoot.data) == 0) {
            // item is equal to localRoot.data
            AgeData ageData = (AgeData) localRoot.data;
            ageData.numberOfPeople++;

            localRoot.data = ( E ) ageData;
            addReturn = false;
            return localRoot;
        }
        else if (item.compareTo(localRoot.data) < 0) {
            // item is less than localRoot.data
            localRoot.left = add(localRoot.left, item);
            return localRoot;
        }
        else {
            // item is greater than localRoot.data
            localRoot.right = add(localRoot.right, item);
            return localRoot;
        }
    }

    /**
     * Searches by calling the required method to find the given ageData object in the tree.
     * @param target object to be found
     * @return If found, that object is returned,
     * otherwise the exception is thrown due to problems in toString in main.
     * Returns the assigned object as default -1.
     */
    @Override
    public E find(E target) {
        try {
            if ( ( E ) find(root, target) == null)
                throw  new NullPointerException("This age could not be found !");
        } catch ( NullPointerException e ) {
            System.out.println(e.getMessage());
            return ( E ) new AgeData();
        }
        return ( E ) find(root, target);
    }

    /**
     *
     * @param localRoot root of the tree to be search
     * @param target item ageData object to be find in the tree
     * @return the found object is returned or null
     */
    private E find(Node< E > localRoot, E target) {
        if (localRoot == null)
            return null;

        // Compare the target with the data field at the root.
        int compResult = target.compareTo(localRoot.data);
        if (compResult == 0)
            return localRoot.data;
        else if (compResult < 0)
            return find(localRoot.left, target);
        else
            return find(localRoot.right, target);
    }

    /**
     * the method required to find smaller ones is called.
     * @param target to find out how many values are smaller than it.
     * @return small number of elements
     */
    public int youngerThan(int target){
       return youngerThan(root, ( E ) new AgeData(target),0);
    }

    /**
     *  Finds how many values are smaller than target.
     * @param localRoot root of the searched tree
     * @param target to find out how many values are smaller than it.
     * @param count is set to 0 at the beginning, will be increased as find
     * @return
     */
    private int youngerThan(Node< E > localRoot, E target,int count) {
        if (localRoot == null)
            return count;

        // Compare the target with the data field at the root.
        int compResult = target.compareTo(localRoot.data);
        if (compResult == 0) {
            return  youngerThan(localRoot.left, target, count);
        }
        else if (compResult < 0) {
            return youngerThan(localRoot.left, target, count);
        }
        else{
            AgeData ageData = (AgeData) localRoot.data;
            count += ageData.numberOfPeople; //++ ydı..
            return youngerThan(localRoot.right, target, youngerThan(localRoot.left,target,count));
        }
    }
    /**
     * the method required to find bigger ones is called.
     * @param target to find out how many values are bigger than it.
     * @return bigger number of elements
     */
    public int olderThan(int target){
        return olderThan(root, ( E ) new AgeData(target),0);
    }
    /**
     *  Finds how many values are bigger than target.
     * @param localRoot root of the searched tree
     * @param target to find out how many values are bigger than it.
     * @param count is set to 0 at the beginning, will be increased as find
     * @return
     */
    private int olderThan(Node< E > localRoot, E target,int count) {
        if (localRoot == null)
            return count;

        int compResult = target.compareTo(localRoot.data);
        if (compResult == 0)
            return  olderThan(localRoot.right, target, count);

        else if (compResult < 0) {
            AgeData ageData = (AgeData) localRoot.data;
            count += ageData.numberOfPeople; //++ ydı.

            return olderThan(localRoot.left, target, olderThan(localRoot.right,target,count));
        }
        else
            return olderThan(localRoot.right, target, count);
    }

    /**
     * Makes removing by calling the required method to remove the
     * given ageData object to the appropriate place in the tree.
     * @param target ageData object
     * @return if remove process is successful true, otherwise false
     */
    @Override
    public boolean remove(E target) {
        return delete(target) != null;
    }

    /**
     * Makes removing by calling the required method to remove the
     * given ageData object to the appropriate place in the tree.
     * @param target ageData object
     * @return if removing is successful true, otherwise false
     */
    public E delete(E target) {
        root = delete(root, target);
        return deleteReturn;
    }
    /**
     * Removes to the right or left of the tree by looking at the age value in the given item.
     * If that age already exists, it decreases the number. If number is 1, removes.
     * @param localRoot root of the tree to be removed
     * @param item ageData object to be removed in the tree
     * @return root of the tree from which removing is made
     */
    private Node< E > delete(Node< E > localRoot, E item) {
        if (localRoot == null) {
            // item is not in the tree.
            deleteReturn = null;
            return localRoot;
        }

        // Search for item to delete.
        int compResult = item.compareTo(localRoot.data);
        if (compResult < 0) {
            // item is smaller than localRoot.data.
            localRoot.left = delete(localRoot.left, item);
            return localRoot;
        }
        else if (compResult > 0) {
            // item is larger than localRoot.data.
            localRoot.right = delete(localRoot.right, item);
            return localRoot;
        }
        else {
            // item is at local root.
            AgeData ageData = (AgeData) localRoot.data;
            if(ageData.numberOfPeople > 1) {
                ageData.numberOfPeople--;
                localRoot.data = ( E ) ageData;
                return localRoot;
            }
            else {
                deleteReturn = localRoot.data;
                if (localRoot.left == null) {
                    // If there is no left child, return right child
                    // which can also be null.
                    return localRoot.right;
                }
                else if (localRoot.right == null) {
                    // If there is no right child, return left child.
                    return localRoot.left;
                }
                else {
                    // Node being deleted has 2 children, replace the data
                    // with inorder predecessor.
                    if (localRoot.left.right == null) {
                        // The left child has no right child.
                        // Replace the data with the data in the
                        // left child.
                        localRoot.data = localRoot.left.data;
                        // Replace the left child with its left child.
                        localRoot.left = localRoot.left.left;
                        return localRoot;
                    }
                    else {
                        // Search for the inorder predecessor (ip) and
                        // replace deleted node�s data with ip.
                        localRoot.data = findLargestChild(localRoot.left);
                        return localRoot;
                    }
                }
            }
        }
    }

    private E findLargestChild(Node< E > parent) {
        // If the right child has no right child, it is
        // the inorder predecessor.
        if (parent.right.right == null) {
            E returnValue = parent.right.data;
            parent.right = parent.right.left;
            return returnValue;
        }
        else {
            return findLargestChild(parent.right);
        }
    }

    private void preOrderTraverse(Node< E > node, int depth, StringBuilder sb) {
        for (int i = 1; i < depth; i++) {
            //sb.append("  ");
        }
        if (node == null) {
            sb.append("null\n");
        }
        else {
            sb.append(node.toString());
            sb.append("\n");
            preOrderTraverse(node.left, depth + 1, sb);
            preOrderTraverse(node.right, depth + 1, sb);
        }
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        preOrderTraverse(root, 1, sb);
        return sb.toString();
    }
}
