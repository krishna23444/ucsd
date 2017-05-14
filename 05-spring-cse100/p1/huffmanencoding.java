/*
 * Filename: HuffmanEncoding.java
 * Author: Nitay Joffe, Kevin Clark
 * Date: Apr 3, 2005
 * 
 * Project: cse100 - P1
 * Description: Object representing a Huffman Code, holds data and bitmask
 */

/**
 * Object represeting Huffman Encoding, holds the data and number of bits that
 * are important (actually part of the encoding). Used in HuffmanTree and for
 * hashing encoded values. 
 * 
 * @author Nitay Joffe (njoffe), Kevin Clark (k1clark)
 * @version Apr 3, 2005
 */
public class HuffmanEncoding {
    private int code, numBits;  // encoding and number of bits of importance
    
    /**
     * Default Constructor - set number of bits in code to 0.
     */
    public HuffmanEncoding() {
        numBits = 0;
    }
    
    /**
     * Constructor initializing the code and number of bits of importance.
     * 
     * @param code      code to set
     * @param numBits   number of important bits in code
     */
    public HuffmanEncoding(int code, int numBits) {
        this.code = code;
        this.numBits = numBits;
    }

    /**
     * Getter for code.
     * @return the int in code.
     */
    public int getCode() {
        return code;
    }
    
    /**
     * Setter for code.
     * @param code the int to set.
     */
    public void setCode(int code) {
        this.code = code;
    }
    
    /**
     * Getter for numBits.
     * @return the int in numBits.
     */
    public int getNumBits() {
        return numBits;
    }
    
    /**
     * Setter for numBits.
     * @param numBits the int to set.
     */
    public void setNumBits(int numBits) {
        this.numBits = numBits;
    }
}
