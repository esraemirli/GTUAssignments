package com.company;

import java.io.*;

/** Class for a binary tree that stores type E objects.
*   @author Koffman and Wolfgang
* */

public class BinaryTree < E >
    implements Serializable {

  /** Class to encapsulate a tree node. */
  protected static class Node < E >
      implements Serializable {
    // Data Fields
    /** The information stored in this node. */
    protected E data;

    protected Node < E > left;

    protected Node < E > right;


    public Node(E data) {
      this.data = data;
      left = null;
      right = null;
    }


    public String toString() {
      return data.toString();
    }
  }

  protected Node < E > root;

  public BinaryTree() {
    root = null;
  }

  protected BinaryTree(Node < E > root) {
    this.root = root;
  }


  public BinaryTree(E data, BinaryTree < E > leftTree,
                    BinaryTree < E > rightTree) {
    root = new Node < E > (data);
    if (leftTree != null) {
      root.left = leftTree.root;
    }
    else {
      root.left = null;
    }
    if (rightTree != null) {
      root.right = rightTree.root;
    }
    else {
      root.right = null;
    }
  }


  public BinaryTree < E > getLeftSubtree() {
    if (root != null && root.left != null) {
      return new BinaryTree < E > (root.left);
    }
    else {
      return null;
    }
  }


    public BinaryTree<E> getRightSubtree() {
        if (root != null && root.right != null) {
            return new BinaryTree<E>(root.right);
        } else {
            return null;
        }
    }


    public E getData() {
        if (root != null) {
            return root.data;
        } else {
            return null;
        }
    }

  public boolean isLeaf() {
    return (root.left == null && root.right == null);
  }

  public String toString() {
    StringBuilder sb = new StringBuilder();
    preOrderTraverse(root, 1, sb);
    return sb.toString();
  }


  private void preOrderTraverse(Node < E > node, int depth,
                                StringBuilder sb) {
    for (int i = 1; i < depth; i++) {
      sb.append("  ");
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

  public static BinaryTree < String > readBinaryTree(BufferedReader bR) throws IOException {
    // Read a line and trim leading and trailing spaces.
    String data = bR.readLine().trim();
    if (data.equals("null")) {
      return null;
    }
    else {
      BinaryTree < String > leftTree = readBinaryTree(bR);
      BinaryTree < String > rightTree = readBinaryTree(bR);
      return new BinaryTree < String > (data, leftTree, rightTree);
    }
  }

}
