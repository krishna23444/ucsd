/*
 * Filename: HuffmanTree.java
 * Author: Nitay Joffe, Kevin Clark
 * Date: Apr 3, 2005
 * 
 * Project: cse100 - P1
 * Description: Abstract base class for Huffman Trees
 */

/**
 * Abstract Class for Huffman Trees. Contains basic information needed for all
 * trees. Encoding and Decoding trees extend this and add their functionality.
 * 
 * @author Nitay Joffe (njoffe), Kevin Clark (k1clark)
 * @version Apr 3, 2005
 */
public abstract class HuffmanTree { 
    private HuffmanNode root;
      
    /**
     * Default constructor - initialize all private variables to 0 (null).
     */
    public HuffmanTree() {
        this(null);
    }
  
    /**
     * Constructor to initialize root node.
     * 
     * @param root  HuffmanNode which is root of HuffmanTree
     */
    public HuffmanTree(HuffmanNode root) {
        this.root = root;
    }
    
    /**
     * Getter for root.
     * @return the HuffmanNode in root
     */
    public HuffmanNode getRoot() {
        return root;
    }
    
    /**
     * Setter for root.
     * @param root the HuffmanNode to set root to
     */
    public void setRoot(HuffmanNode root) {
        this.root = root;
    }
}
