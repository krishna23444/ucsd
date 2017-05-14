/*
 * Filename: TreapSet.java
 * Author: Nitay Joffe, Alvin Jeng
 * Date: Apr 21, 2005
 * Project: cse100-p2
 */

import java.util.Random;

/**
 * A node in the treap. Stores data and priority along with pointers to the
 * parent and left and right children. Uses a static Random to create random
 * priorities for each new node when the constructor is called. Has methods for
 * avl rotations using the priority. Also has methods for rippling a node up to
 * its appropriate location or down to a leaf,  using the avl rotations.
 * @author Nitay Joffe (njoffe), Alvin Jeng (apjeng)
 * @version Apr 21, 2005
 */
public class TreapSetNode<E> {
    private static Random r = new Random();
    private E data = null;
    private int priority = 0;
    public TreapSetNode<E> left = null, right = null, parent = null;

    /**
     * Default constructor, used only when creating initial root node.
     * @param data  element E to store inside node.
     */
    public TreapSetNode(E data) {
        this(data, null);
    }

    /**
     * Constructor to make a node and attach to a parent
     * @param data      the element E to store inside the node
     * @param parent    the TreapSetNode<E> parent node of node being created 
     */
    public TreapSetNode(E data, TreapSetNode<E> parent) {
        priority = r.nextInt();
        this.data = data;
        this.parent = parent;
    }

    /**
     * Getter for data E stored in node.
     * @return  E - data stored in node
     */
    public E getData() {
        return data;
    }

    /**
     * Getter for priority of this node.
     * @return  int - priority of this node
     */
    public int getPriority() {
        return priority;
    }

    /**
     * Compare this node's priority with another node.
     * @param tn    TreapSetNode to compare priority with
     */
    public int comparePriority(TreapSetNode<E> tn) {
        return (priority - tn.priority);
    }

    /**
     * Ripples this node to the bottom (till it's a leaf), each time rotating
     * up the child with the highest priority in order to maintain heap
     * priority order. Returns the new root node, if the old 
     */
    public void rippleDown() {
        while(left != null || right != null) {
            // both children present, rotate child with higher priority
            if(left != null && right != null) {
                if(left.comparePriority(right) < 0) {
                    rotateWithRightChild();
                }
                else {
                    rotateWithLeftChild();
                }
            }
            else if(left == null) {     // no left child, rotate right child
                rotateWithRightChild();
            }
            else {                      // no right child, rotate left child
                rotateWithLeftChild();
            }
        }
    }

    /**
     * Performs a single left rotaiton with node's right child.
     */
    public void rotateWithRightChild() {
        TreapSetNode<E> chain = right;  // store right chain for later use
        right = chain.left;             // move left child of right child
        if(right != null) {             // set its pointer to us if not null
            right.parent = this;
        }
        chain.left = this;              // rotate right child up
        chain.parent = parent;          // set its parent ptr to our parent 
        if(parent != null) {            // set our parent's ptr to it
            if(parent.left == this) {
                parent.left = chain;
            }
            else {
                parent.right = chain;
            }
        }
        parent = chain;
    }

    /**
     * Performs a single right rotation with node's left child.
     */
    public void rotateWithLeftChild() {
        TreapSetNode<E> chain = left;   // store left chain for later use
        left = chain.right;             // move right child of left child
        if(left != null) {
            left.parent = this;
        }
        chain.right = this;             // rotate left child up
        chain.parent = parent;          // set its parent ptr to our parent
        if(parent != null) {            // set our parent's ptr to it
            if(parent.left == this) {
                parent.left = chain;
            }
            else {
                parent.right = chain;
            }
        }
        parent = chain;
    }
    
    /**
     * String representation of TreapSetNode, useful for printing out object.
     * @return String   representation of this treap
     */
    public String toString() {
        String str = "node data: " + data + "  priority: " + priority + "\n";
        if(parent != null) {
            str += "  parent - data: " + parent.data +
                "  priority: " + parent.priority + "\n";
        }
        if(left != null) {
            str += "  left - data: " + left.data +
                "  priority: " + left.priority + "\n";
        }
        if(right != null) {
            str += "  right - data: " + right.data +
                "  priority: " + right.priority + "\n";
        }
        if(left != null)
            str += left;
        if(right != null)
            str += right;
        return str;
    }
}
