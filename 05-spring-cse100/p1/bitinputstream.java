/*
 * Filename: BitInputStream.java
 * Author: Nitay Joffe, Kevin Clark
 * Date: Apr 5, 2005
 * 
 * Project: cse100 - P1
 * Description: An InputStream wrapper to read bit-by-bit.
 */

import java.io.*;

/**
 * InputStream wrapper to read in bits.
 * 
 * @author Nitay Joffe (njoffe), Kevin Clark (k1clark)
 * @version Apr 5, 2005
 */
public class BitInputStream {
    private InputStream fin;
    private int buff, loc;  // buffer and location of which bit we're on
    
    /**
     * Create new object wrapped around the input stream passed in.
     * 
     * @param fin   the InputStream to read in from
     */
    public BitInputStream(InputStream fin) {
        this.fin = fin;
        buff = 0;       // clear buffer
        loc = -1;       // need to read a byte before we can return any bits
    }
    
    /**
     * Read a bit from the buffer, fill buffer with next byte if empty. Returns -1
     * when end of file (EOF) is reached.
     * 
     * @return              the next bit in the buffer
     * @throws IOException  if an IO error occurs
     */
    public int readBit() throws IOException {
        if(loc < 0) {               //read in a new byte if needed
            if((buff = fin.read()) == Compress.EOF) {
                return buff;
            }
            loc = 7;
        }
        return (buff & (0x1 << loc--));    //return next bit and decrement loc
    }
    
    /**
     * Close the stream associated with this object.
     * 
     * @throws IOException  if an IO error occurs
     */
    public void close() throws IOException {
        fin.close();
    }
}
