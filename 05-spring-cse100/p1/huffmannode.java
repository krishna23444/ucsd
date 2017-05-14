/*
 * Filename: HuffmanNode.java
 * Author: Nitay Joffe, Kevin Clark
 * Date: Apr 3, 2005
 * 
 * Project: cse100 - P1
 * Description: Represents the node in a Huffman Coding Tree. Used in
 *              HuffmanTree to create the tree used in encoding/decoding text.
 */

/**
 * Node in a Huffman Tree - stores data and the frequency that data occured.
 * Has pointers to left and right children nodes, and its parent node.
 * Implements Comparable by comparing frequencies for natural ordering.
 * 
 * @author Nitay Joffe (njoffe), Kevin Clark (k1clark)
 * @version Apr 3, 2005
 */
public abstract class HuffmanNode implements Comparable {
    private byte data;
    public HuffmanNode parent, leftChild, rightChild;
    
    /**
     * Default constructor - initialize data and frequency to zero.
     */
    public HuffmanNode() {
        this((byte)0);
    }

    /**
     * Constructor to initialize data.
     * 
     * @param data
     */
    public HuffmanNode(byte data) {
        this.data = data;
        parent = leftChild = rightChild = null;
    }
  
    /**
     * Constructor for making parent for two child nodes.
     * 
     * @param leftChild the left child of this node
     * @param rightChild the right child of this node
     */
    public HuffmanNode(HuffmanNode leftChild, HuffmanNode rightChild) {
        this.leftChild = leftChild;
        this.rightChild = rightChild;
    }
    
    /**
     * Comparison function to sort nodes.
     * 
     * @param o the Object to compare this with
     */
    public abstract int compareTo(Object o);

    /**
     * Getter for data.
     * @return the byte in data
     */
    public byte getData() {
        return data;
    }
    
    /**
     * Setter for data.
     * @param data the byte to set
     */
    public void setData(byte data) {
        this.data = data;
    }
}
