/*
 * Filename: HuffDecodeNode.java
 * Author: Nitay Joffe, Kevin CLark
 * Date: Apr 10, 2005
 * 
 * Project: cse100-P1
 * Description: Class of node for HuffDecodeTree.
 */

/**
 * Representation of a node in a HuffDecodeTree.
 * Holds level and data information.
 * 
 * @author Nitay Joffe (njoffe), Kevin Clark (k1clark)
 * @version Apr 10, 2005
 */
public class HuffDecodeNode extends HuffmanNode {
    private int level;
    
    /**
     * Default constructor. Data and level are set to 0.
     * 
     */
    public HuffDecodeNode() {
        this((byte)0, 0);
    }

    /**
     * Constructor to make HuffDecodeNode based on Data.
     * Level is set to 0.
     * 
     * @param data
     */
    public HuffDecodeNode(byte data) {
        this(data, 0);
    }

    /**
     * Constructor to make a parent node from two children.
     * 
     * @param leftChild     HuffDecodeNode
     * @param rightChild    HuffDecodeNode           
     */
    public HuffDecodeNode(HuffmanNode leftChild, HuffmanNode rightChild) {
        super(leftChild, rightChild);
        level = ((HuffDecodeNode)leftChild).getLevel() - 1;
    }
    
    /**
     * Constructor for creating a node with a given data and level.
     * 
     * @param data  the byte this node represents
     * @param level  the level of this node in the tree
     */
    public HuffDecodeNode(byte data, int level) {
        super(data);
        this.level = level;
    }

    /**
     * Comparison function. Had a purpose originally, but became unnecessay
     * and is now in here for inheritance of abstract class.
     * 
     * @param o     the Object to compare with
     * @return      0, equals, for all cases
     * @see java.lang.Comparable#compareTo(java.lang.Object)
     */
    public int compareTo(Object o) {
                                                                                                                                                                                                                                                                                                                                    
