/*
 * Filename: HuffEncodeNode.java
 * Author: Nitay Joffe, Kevin Clark
 * Date: Apr 6, 2005
 * 
 * Project: cse100 - P1
 * Description: Node in a HuffEncodeTree.
 */

/**
 * Node in a HuffEncodeTree. Stores Frequency, encoding, child identity
 * information (left or right), and data inherited from abstract class.
 * 
 * @author Nitay Joffe (njoffe), Kevin Clark (k1clark)
 * @version Apr 6, 2005
 */
public class HuffEncodeNode extends HuffmanNode {
    private int freq;                       //how many times this data was seen
    private HuffmanEncoding encoding;       //node's Huffman encoding in tree
    private boolean isOneChild;             //true if child is the right child
    
    /**
     * Default constructor - set node fields to 0.
     */
    public HuffEncodeNode() {
        this((byte)0,0);
    }
    
    /**
     * Constructor initializing data and frequency of node.
     * 
     * @param freq
     * @param data
     */
    public HuffEncodeNode(byte data, int freq) {
        super(data);
        this.freq = freq;
        encoding = new HuffmanEncoding();
    }

    /**
     * Constructor for creating a parent node out of two children.
     * 
     * @param leftChild
     * @param rightChild
     */
    public HuffEncodeNode(HuffEncodeNode leftChild, HuffEncodeNode rightChild) {
        super(leftChild, rightChild);
        freq = leftChild.freq + rightChild.freq;    //freq = sum of children
        encoding = new HuffmanEncoding();
        leftChild.isOneChild = false;               // identify children
        rightChild.isOneChild = true;
    }

    /**
     * Compare nodes using their frequency. Used for HuffmanTree creation from a
     * set of nodes (the 'forest').
     * 
     * @param o the Object to compare with
     * @see java.lang.Comparable#compareTo(java.lang.Object)
     */
    public int compareTo(Object o) {
        HuffEncodeNode hn = (HuffEncodeNode) o;
        return (freq - hn.freq);
    }

    /**
     * Get this node's encoding if it knows it, otherwise recursively call
     * parent's getEncoding to find out this node's encoding.
     * 
     * @return  node's encoding in HuffmanTree
     */
    public HuffmanEncoding getEncoding() {
        if(encoding.getNumBits() == 0) {        // no encoding set, use parent
            if(parent == null) {                // root returns blank encoding
                return new HuffmanEncoding(0, 0);
            }
            HuffmanEncoding parentCode = ((HuffEncodeNode)parent).getEncoding();
            
            // use parent's encoding with our identity to set our encoding
            encoding.setCode((parentCode.getCode() << 1) | (isOneChild ? 1 : 0));
            encoding.setNumBits((parentCode.getNumBits()+1));
        }
        return encoding;
    }
    
    /**
     * Setter for isOneChild, true if child is the right child.
     * @param isOneChild    boolean to set isOneChild to
     */
    public void setIsOneChild(boolean isOneChild) {
        this.isOneChild = isOneChild;
    }
    
    /**
     * Getter for isOneChild. Returns true if this node is a right child.
     * @return the boolean in isOneChild
     */
    public boolean isOneChild() {
        return isOneChild;
    }
}
