/*
 * Filename: HuffEncodeTree.java
 * Author: Nitay Joffe, Kevin Clark
 * Date: Apr 6, 2005
 * 
 * Project: cse100 - P1
 * Description: Tree for compressing files using Huffman Coding algorithm
 */

import java.io.*;
import java.util.*;

/**
 * Implementation of a tree specifically targeted for file compression using the
 * Huffman Coding algorithm.
 * 
 * @author Nitay Joffe (njoffe), Kevin Clark (k1clark)
 * @version Apr 6, 2005
 */
public class HuffEncodeTree extends HuffmanTree {
    private int freqArr[];
    private HuffEncodeNode leaves[];
    private int size;
    private byte numLeaves;

    /**
     * Default Constructor - create an empty tree and initialize arrays.
     */
    public HuffEncodeTree() {
        this(null);
    }

    /**
     * Constructor initializing the root node pointer.
     * 
     * @param root  HuffmanNode to use as this tree's root
     */
    public HuffEncodeTree(HuffmanNode root) {
        super(root);
        freqArr = new int[Compress.ALPHABETSIZE];
        leaves = new HuffEncodeNode[Compress.ALPHABETSIZE];
        size = 0;
        numLeaves = 0;
    }

    /**
     * Get the Huffman encoding array for encoding the input file passed in.
     * Reads the file through completely to gather frequency data, builds a
     * HuffmanTree from that data using a PriorityQueue, and finally arranges
     * the encodings of all the leaves in the HuffmanTree.
     * 
     * @param fin   the DataInputStream to read input from
     * @return      array HuffmanEncodings representing the encoding of each byte
     * @throws IOException
     */
    public HuffmanEncoding[] getEncodingArr(DataInputStream fin) throws IOException {      
        parseFrequencies(fin);
        buildTree();
        
        // get encodings for every possible data that has a leaf in our tree
        HuffmanEncoding encodeArr[] = new HuffmanEncoding[leaves.length];
        for(int i = 0; i < leaves.length; i++) {
            if(leaves[i] != null) {
                encodeArr[i] = leaves[i].getEncoding(); 
            }
        }
        return encodeArr;
    }
    
    /**
     * Read in the file until EOF is reached, filling a frequency array.
     * 
     * @param fin   DataInputStream to read input from
     * @throws IOException  if an IO error occurs
     */
    private void parseFrequencies(DataInputStream fin) throws IOException {
        int buff = 0;
        while((buff = fin.read()) != Compress.EOF) {
                freqArr[buff]++;
        }
    }
    
    /**
     * Builds a Huffman Tree for encoding from an array of frequencies. Uses
     * Java's PriorityQueue (we wrote our own, PQueue.java, that worked great
     * but for some reason made the file bigger even though the tree looked
     * exactly the same while debugging) to build the tree out of the 'forest'.
     */
    private void buildTree() {
//          our PQ had issues, switched to Java's PriorityQueue from java.util
//          PQueue forest = new PQueue();
        PriorityQueue forest = new PriorityQueue();
        HuffEncodeNode leftNode = null, rightNode = null;
        
        // fill priority queue with forest of HuffmanNode from frequencies array
        for(int i = 0; i < freqArr.length; i++) {
            if(freqArr[i] > 0) {
                leaves[i] = new HuffEncodeNode((byte)i, freqArr[i]);
                forest.add(leaves[i]);
                size += freqArr[i];             // size of file
                numLeaves++;                    // number of characters in file
            }
        }
        
        // use priority queue forest to build huffman tree from the bottom up
        while(!forest.isEmpty()) {
            //get the two smallest nodes in the forest
            leftNode = (HuffEncodeNode) forest.poll();
            rightNode = (HuffEncodeNode) forest.poll();
        
            // no more nodes in priority queue, left node was the last node
            if(rightNode == null) {
                break;
            }
            
            //create parent node from two children and insert back into PQ
            leftNode.parent = new HuffEncodeNode(leftNode, rightNode);
            rightNode.parent = leftNode.parent;
            forest.add(leftNode.parent);
        }
        if(numLeaves != 1) {                    // last node is root
            setRoot(leftNode);
        }
        else {                      // last node is only node, create a root
            setRoot(new HuffEncodeNode());
            getRoot().rightChild = leftNode;
            leftNode.setIsOneChild(true);
            leftNode.parent = getRoot();
        }
    }
    
    /**
     * Writes a header containing all the information needed to rebuild this
     * tree and uncompress the file associated with it. First writes 4 bytes
     * of the size of the file, followed by 1 byte of how many characters there
     * are, followed by 2 bytes of data:level information for all the leaf nodes
     * as seen by the tree from left to right.
     * 
     * @param fout          the DataOutputStream to write to
     * @throws IOException  if an IO error occurs
     */
    public void writeHeader(DataOutputStream fout) throws IOException {
        fout.writeInt(size);        // write size of file
        fout.write(numLeaves);      // write number of different chars in file

        // write the leaves of the tree with their data from left to right
        if(getRoot() != null) {
            writeLeaves(fout, (HuffEncodeNode)getRoot());
        }
        fout.flush();        
    }
    
    /**
     * Recursive function to print out data:level 2 byte pairs of all the leaf
     * nodes starting at the passed in node using a left-right-myself traversal
     * order.
     * 
     * @param fout          the DataOutputStream to write out to
     * @param node          the root HuffEncodeNode to start at
     * @throws IOException  if an IO error occurs
     */
    private void writeLeaves(DataOutputStream fout, HuffEncodeNode node) throws IOException {
        if(node.leftChild != null) {
            writeLeaves(fout, (HuffEncodeNode)node.leftChild);
        }
        
        if(node.rightChild != null) {
            writeLeaves(fout,(HuffEncodeNode)node.rightChild);
        }
        
        // node has no children, it is a leaf, write out its data and level
        if(node.rightChild == null && node.leftChild == null) {
            fout.writeByte(node.getData());
            fout.writeByte(node.getEncoding().getNumBits());
        }
    }
}
