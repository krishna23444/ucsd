/*
 * Filename: BitOutputStream.java
 * Author: Nitay Joffe, Kevin Clark
 * Date: Apr 5, 2005
 * 
 * Project: cse100 - P1
 * Description: An OutputStream wrapper for writing bit-by-bit.
 */

import java.io.*;

/**
 * OutputStream wrapper for writing bits.
 * 
 * @author Nitay Joffe (njoffe), Kevin Clark (k1clark)
 * @version Apr 5, 2005
 */
public class BitOutputStream {
    private OutputStream fout;
    private int buff, loc;      // buffer and location of which bit we're on
    
    /**
     * Constructor to wrap this object's OutputStream around the passed in one.
     * 
     * @param fout  the stream to send output to
     */
    public BitOutputStream(OutputStream fout) {
        this.fout = fout;
        buff = 0;           // clear buffer
        loc = 7;            // start buffer at MSB
    }

    /**
     * Flush the bit buffer by writing out the current byte.
     * 
     * @throws IOException  if an IO error occurs
     */
    public void flush() throws IOException {
        fout.write(buff);
    }
    
    /**
     * Appends the least significant bit in the parameter to the buffer.
     * 
     * @throws IOException  if an IO error occurs
     */
    public void writeBit(int bit) throws IOException {
        buff |= (bit << loc);
        if(--loc < 0) {     //decrement to next bit, write out if buffer full
            flush();
            buff = 0;       // reset buffer and location
            loc = 7;
        }
    }

    /**
     * Closes the associated stream, flushing any bits in buffer first.
     * 
     * @throws IOException  if an IO error occurs
     */
    public void close() throws IOException {
        flush();
        fout.close();
    }
}
