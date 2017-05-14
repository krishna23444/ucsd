/*
 * Filename: Compress.java
 * Author: Nitay Joffe, Kevin Clark
 * Date: Apr 3, 2005
 * 
 * Project: cse100 - P1
 * Description: File compression using Huffman Coding algorithm.
 */

import java.io.*;

/**
 * Compresses a file passed in using the Huffman Coding algorithm. The Huffman
 * Tree is built using the java api's PriorityQueue together with a forest of
 * nodes that are compared to each other by the frequencies of their data. The
 * Huffman Tree is used to write out the header to the compressed file before
 * any actual compression gets done. For the actual compression, in order to
 * speed up the process, we have the Huffman Encoding Tree create an array of
 * encodings for each byte, which allows O(1) encoding of every byte we read in.
 * The BitOutputStream class is used for writing the encodings out bit-by-bit.
 * 
 * As a side-note, we wrote our own PriorityQueue implementation in PQueue.java.
 * It works 100% functionality wise, we can swap it in and everything works in
 * terms of uncompressing after compressing, BUT, it always produces a bigger
 * compressed file for some reason. We couldn't figure it out and decided to
 * just swap in the java one. If the graders/TAs have any time to look at our
 * PriorityQueue code and give us some feedback on what they think might be
 * wrong, we would be very grateful. We appreciate the help.
 * 
 * @author Nitay Joffe (njoffe), Kevin Clark (k1clark)
 * @version Apr 3, 2005
 */
public class Compress {
    //define size of alphabet (1 byte: 0-255 -> 256), used in all files
    public static int ALPHABETSIZE = 256;
    public static int EOF = -1;
    
    /**
     * Compress args[0], writing to args[1], using HuffEncodeTree object which
     * implements the Huffman Coding method.
     * 
     * @param args  input file to read from, output file to write to
     */
	public static void main(String[] args) {
        DataInputStream fin = null;
        DataOutputStream fout = null;
        BitOutputStream bout = null;
        HuffEncodeTree ht = new HuffEncodeTree();
        HuffmanEncoding encodeArr[] = new HuffmanEncoding[ALPHABETSIZE];        

        // check for correct arguments passed in
	    if(args.length != 2) {
			System.err.println("Usage: java Compress infile outfile");
            System.exit(1);
		}
           
        try {
            // open input file for reading and output file for writing
            fin = new DataInputStream(new BufferedInputStream(new FileInputStream(args[0])));
            fout = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(args[1])));
            
            // use HuffmanEncodeTree to create an encoding array and write header
            encodeArr = ht.getEncodingArr(fin);
            ht.writeHeader(fout);  
            
            // reopen input file to read and attach BitOutputStream to output file
            fin.close();
            fin = new DataInputStream(new BufferedInputStream(new FileInputStream(args[0])));
            bout =  new BitOutputStream(fout);
            
            // encode the file using encoding array from HuffEncodeTree
            int buff = 0;
            while((buff = fin.read()) != EOF) {
                for(int i = encodeArr[buff].getNumBits(); i > 0; i--) {
                    bout.writeBit((encodeArr[buff].getCode() >> (i-1)));
                }
            }
            
            // close the streams
            fin.close();
            bout.close();
        }
        catch (IOException e) {
            e.printStackTrace();
            System.exit(1);
        }
	}
}
