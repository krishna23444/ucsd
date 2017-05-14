/*
 * Filename: SkipListSetNode.java
 * Author: Nitay Joffe, Alvin Jeng
 * Date: Apr 21, 2005
 * Project: cse100-p2
 */

import java.util.*;

/**
 * A node in the SkipList. Stores generic data and level, which is created
 * randomly by the encompassing SkipList when it calls constructor. Also has an
 * array of forward pointers, made public so that it will be easy for the
 * SkipList class to manuever quickly.
 * @author Nitay Joffe (njoffe), Alvin Jeng (apjeng)
 * @version Apr 21, 2005
 */
public class SkipListSetNode<E> {
    private E data = null;
    private int level = 0;
    public SkipListSetNode<E> [] forward = null;

    /**
     * Constructor taking in data and level
     * @param data  item of generic type E to hold in this node
     * @param level integer level of this node (how many forward pointers)
     */
    SkipListSetNode(E data, int level) {
        this.data = data;
        this.level = level;
        forward = (SkipListSetNode<E> []) new SkipListSetNode[level];
    }

    /**
     * Getter for data.
     * @return  data of genetic type E stored in this node
     */
    public E getData() {
        return data;
    }
    
    /**
     * Getter for level.
     * @return  integer level of this node
     */
    public int getLevel() {
        return level;
    }

    /**
     * String representation of skiplist, useful for printing.
     * @return  String representation of this skiplist
     */
    public String toString() {
        String str = "";
        str += "data: " +data+ "  level: " +level+ "  has ptrs ->\n";
        for(int i = 0; i < level && forward[i] != null; i++) {
            str += "  ptr level: " +i+ "  points to:" +
                "  data: " + forward[i].getData() +
                "  level: " + forward[i].getLevel() + "\n";
        }
        return str;
    }
}
