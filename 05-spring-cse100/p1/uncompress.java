/*
 * Filename: Uncompress.java
 * Author: Nitay Joffe, Kevin Clark
 * Date: Apr 9, 2005
 * 
 * Project: cse100 - P1
 * Description: File decompression using Huffman Coding algorithm.
 */

import java.io.*;

/**
 * Uncompresses a file passed in using the Huffman Coding algorithm. The tree
 * to use during the decompressing is created by extracting information inside
 * the header of the file. The header was written by the Huffman Encoding Tree
 * using a predefined format (4 byte size, 1 byte numChars, 2 bytes data:level
 * per leaf node going left to right along the tree). The Decoding tree is
 * creating by using a stack to push on nodes while their level is different
 * from the one before it (to the left of it), and pop the stack to create 
 * parent nodes when the levels match. With this method eventually all the nodes
 * get joined together by the parent nodes and all there is left is the root
 * node. Reading the compressed file is done bit-by-bit using the bit buffer
 * class BitOutputStream. For the actual decompressing we use the HUffman
 * Decoding Tree like a state machine which we traverse through; every time we
 * read a 1 we go right, and every time we read a 0 we go left. When we hit a
 * leaf node we print its character and move back to the root node. This is done
 * in a loop until all the characters have been read, at which point the file
 * has been decompressed.
 * 
 * @author Nitay Joffe (njoffe), Kevin Clark (k1clark)
 * @version Apr 9, 2005
 */
public class Uncompress {
    /**
     * Uncompress args[0], writing to args[1],  using the Huffman Tree written
     * in the header of args[0]. Uses HuffDecodeTree class to create and tree
     * and get all the information required to decoede the file.
     * 
     * @param args  input file to uncompress, and output file to write to
     */
    public static void main(String[] args) {
        DataInputStream fin = null;
        BitInputStream bin = null;
        DataOutputStream fout = null;
        HuffDecodeTree ht = null;
        HuffDecodeNode current = null;
        int bit = 0, size = 0;
        
        // check for correct number of arguments passed in
        if( args.length != 2 ) {
            System.err.println("usage: java Uncompress infile outfile");
            System.exit(1);
        }
        
        try {
            fin = new DataInputStream(new BufferedInputStream(new FileInputStream(args[0])));        
            fout = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(args[1])));
            
            // build the HuffDecodeTree used for decoding
            ht = new HuffDecodeTree();
            size = ht.buildTree(fin);
            
            // traverse the decoding tree while reading in the file bit-by-bit.
            // when a leaf node is encountered, print out its data and move back
            // to the root node. Thus our tree is basically a state machine that
            // the input file goes through bit-by-bit.
            bin = new BitInputStream(fin);
            for(int i = 0; i < size; i++) {
                current = (HuffDecodeNode)ht.getRoot();
                while((bit = bin.readBit()) != Compress.EOF) {
                    if(bit != 0) {      // read non-zero (aka 1), go right           
                        current = (HuffDecodeNode) current.rightChild;
                    }
                    else {              // read zero, go left
                        current = (HuffDecodeNode) current.leftChild;
                    }
                    // check if at a leaf, if so then print data and start over
                    if(current.leftChild == null && current.rightChild == null) {
                        fout.write(current.getData());
                        break;
                    }
                }
            }
            fout.close();           // calls flush before it closes
            bin.close();
        }
        catch(IOException e) {
            e.printStackTrace();
            System.exit(1);
        }
    }
}
