/*
 * Filename: HuffDecodeTree.java
 * Author: Nitay Joffe, Kevin Clark
 * Date: Apr 6, 2005
 * 
 * Project: cse100 - P1
 * Description: Huffman Tree used for decoding files.
 */

import java.io.*;
import java.util.*;

/**
 * Huffman Tree for decoding files. Builds itself by reading a header and
 * creating HuffDecodeNodes with appropriate data and levels as necessary. Used
 * by Uncompress to decompress files.
 * 
 * @author Nitay Joffe (njoffe), Kevin Clark (k1clark)
 * @version Apr 6, 2005
 */
public class HuffDecodeTree extends HuffmanTree {
    /**
     * Default constructor - calls super.
     */
    public HuffDecodeTree() {
        this(null);
    }

    /**
     * Constructor initializing root node.
     * 
     * @param root
     */
    public HuffDecodeTree(HuffmanNode root) {
        super(root);
    }
    
    /**
     * Reads a header from the DataInputStream passed in and builds a tree.
     * Returns the size of the file in bytes.
     * 
     * @param   fin     DataInputStream to read header from
     * @return  integer representing size of file in bytes
     * @throws  IOException if an IO error occurs
     */
    public int buildTree(DataInputStream fin) throws IOException {
        int size = 0;
        int numLeaves = 0;
        Stack st = null;                // for building tree from leaf header
        HuffDecodeNode node = null;     // working node

        if((size = fin.readInt()) != 0) {   // first int = size of file
            numLeaves = fin.read();         // 5th byte = number of characters
            
            // create stack for making tree, push on first (leftmost) leaf
            st = new Stack();
            st.ensureCapacity(numLeaves);       // prevent dynamic growth         
            
            // build the tree from left to right using the leaf nodes
            // read in all the leaf nodes from left to right, for each one
            // pop the stack and create parent nodes until the levels do not
            // match, and then push whatever is left back on to the stack
            for(int i = 0; i < numLeaves; i++) {
                node = new HuffDecodeNode((byte)fin.read(), fin.read());
                while(!st.empty() && (node.getLevel() == ((HuffDecodeNode)st.peek()).getLevel())) {
                    node = new HuffDecodeNode((HuffDecodeNode)st.pop(), node);
                }
                st.push(node);
            }
            if(numLeaves != 1) {                        // last node is root
                setRoot((HuffDecodeNode)st.pop());          
            }
            else {              // last node is only node, need to create a root
                setRoot(new HuffDecodeNode());
                getRoot().rightChild = (HuffDecodeNode) st.pop();
            }
        }
        return size;
    }
}
