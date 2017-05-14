/*
 * Filename: SkipListSet.java
 * Author: Nitay Joffe, Alvin Jeng
 * Date: Apr 21, 2005
 * Project: cse100-p2
 */

import java.util.*;

/**
 * SkipList extension of a SortedSet. Each node has a level, chosen at random
 * (using java's random class) associated with it, which determines how many
 * forward pointers it has. The level is at least one and at most MAXLEVEL,
 * defined in this file to be 16. The forward pointers are used to skip some
 * elements in order to get to the required element faster. In effect this
 * skiplist is a sorted linked list with each node having additional pointers
 * to skip over some values. We implemented the node for this skiplist in
 * another class, and the iterator as an inner class within this one since it
 * made the Iterator's tasks much easier.
 * @author Nitay Joffe (njoffe), Alvin Jeng (apjeng)
 * @version Apr 21, 2005
 */
public class SkipListSet<E> implements SortedSet<E> {
    public static final int MAXLEVEL = 16;
    public static final double PROB = .5;

    private Random rand = new Random();
    private Comparator<? super E> c = null;
    private SkipListSetNode<E> header = null;
    private int numItems = 0;

    /**
     * Default constructor. Use E's compareTo function (assume it implements
     * Comparable since no Comparator was passed in).
     */
    public SkipListSet() {
        this(null);
    }

    /**
     * Constructor taking in Comparator to use for comparing data
     * @param c the generic Comparator treap should use
     */
    public SkipListSet(Comparator<? super E> c) {
        this.c = c;
        header = new SkipListSetNode<E>(null, MAXLEVEL);
    }

    /**
     * Get Comparator being used, or null if natural ordering through elements'
     * Comparable interface is being used.
     * @return  Comparator if one is being used, null otherwise
     * @see java.util.SortedSet#comparator()
     */
    public Comparator<? super E> comparator() {
        return c;
    }

    /**
     * Get first (lowest) element in skiplist
     * @return  lowest element in skiplist
     * @throws  NoSuchElementException  if skiplist is empty
     * @see java.util.SortedSet#first()
     */
    public E first() {
        if(isEmpty()) {
            throw new NoSuchElementException();
        }
        return header.forward[0].getData();
    }

    /**
     * Get last (highest) element in skiplist
     * @return  highest element in skiplist
     * @throws  NoSuchElementException  if skiplist is empty
     * @see java.util.SortedSet#last()
     */
    public E last() {
        if(isEmpty()) {
            throw new NoSuchElementException();
        }
        SkipListSetNode<E> sln = header;
        // walk through skiplist till the end using the biggest jumps possible
        for(int i = MAXLEVEL - 1; i >= 0; i--) {
            while(sln.forward[i] != null) {
                sln = sln.forward[i];
            }
        }
        return sln.getData();
    }

    /**
     * Get number of items in skiplist
     * @return  number of elements in skiplist
     * @see java.util.Collection#size()
     */
    public int size() {
        return numItems;
    }

    /**
     * Test for empty skiplist
     * @return  <code>true</code> if skiplist is empty
     * @see java.util.Collection#isEmpty()
     */
    public boolean isEmpty() {
        return (numItems == 0);
    }

    /**
     * Comparison function using either passed in Comparator or E's compareTo
     * function, assuming E has appropriately implemented Comparable<E>.
     * @throws ClassCastException   if E does not implement Comparable<E>
     */
    private int compare(E e1, E e2) {
        if(c == null) {
            return ((Comparable<E>)e1).compareTo(e2);
        }
        else {
            return c.compare(e1, e2);
        }
    }

    /**
     * Find out if passed in item exists in skiplist
     * @param o the Object to search for
     * @return  <code>true</code> if o is in skiplist
     * @throws ClassCastException   if o is not an E
     * @see java.util.Collection#contains(java.lang.Object)
     */
    public boolean contains(Object o) {
        E e = (E) o;
        SkipListSetNode<E> sln = header;
        // search through skiplist using biggest jumps until we get to a point
        // where the next node is greater than, or equal to, the item we're
        // searching for we are at the end of the list
        for(int i = MAXLEVEL - 1; i >= 0; i--) {
            while(sln.forward[i] != null &&
                    compare(e, sln.forward[i].getData()) > 0) {
                sln = sln.forward[i];
            }
        }
        // at end of list or next item not what looking for, search failed
        if(sln.forward[0] == null ||
                compare(e, sln.forward[0].getData()) != 0) {
            return false;
        }
        else {              // next item equates, search succeeded
            return true;
        }
    }

    /**
     * Get an Iterator for this skiplist, organized in sorted manner. Uses
     * private inner class to do all the dirty work.
     * @return  Iterator representing skiplist
     * @see java.lang.Iterable#iterator()
     */
    public Iterator<E> iterator() {
        return new skiplistIter();
    }

    /**
     * Inner lcass implementing Iterator for the skipList. Has access to all of
     * the skiplist's variables and methods which makes it easy to work with.
     */
    private class skiplistIter implements Iterator<E> {
        private SkipListSetNode<E> [] update = (SkipListSetNode<E> [])
            new SkipListSetNode[MAXLEVEL];
        private SkipListSetNode<E> last = null, next = header.forward[0];

        /**
         * Constructor for iterator, set next to first (lowest) item
         */
        public skiplistIter() {
            for(int i = 0; i < MAXLEVEL; i ++) {
                update[i] = header;
            }
        }

        /**
         * Returns true if a call to next would return an object, aka if the
         * iterator hasn't gone all the way through and there are items
         * remaining.
         * @return  <code>true</code> if next pointer is not null
         */
        public boolean hasNext() {
            return (next != null);
        }

        /**
         * Get the next element in the iteration.
         * @throws NoSuchElementException   if iteration has no more elements
         */
        public E next() {
            if(next == null) {
                throw new NoSuchElementException();
            }
            // update predecessor pointers if last item wasn't removed
            if(last != null) {
                for(int i = 0; i < last.getLevel(); i++) {
                    update[i] = last;
                }
            }
            last = next;
            next = next.forward[0];
            return last.getData();
        }

        /**
         * Remove element returned by the last next method call.
         * @throws IllegalStateException    if next method has not been called
         *      or remove method has already been called after last next call
         */
        public void remove() {
            if(last == null) {
                throw new IllegalStateException();
            }
            // splice out node and update all its predecessor's forward ptrs
            for(int i = 0; i < last.getLevel(); i++) {
                update[i].forward[i] = last.forward[i];
            }
            last = null;
            numItems--;
        }
    }

    /**
     * Get an array representation of this skiplist, in sorted order 
     * @return  array of Objects corresponding to sorted skiplist elements
     * @see java.util.Collection#toArray()
     */
    public Object[] toArray() {
        Object [] arr = new Object[numItems];
        SkipListSetNode<E> sln = header.forward[0];
        // list is in order, so use level 1 pointers to build sorted array
        for(int i = 0; i < numItems; i++) {
            arr[i] = sln.getData();
            sln = sln.forward[0];
        }
        return arr;
    }

    /**
     * Insert element into the skiplist if it doesn't already exist.
     * @param o the element E to insert.
     * @return  <code>true</code> if method call changed skiplist
     */
    public boolean add(E o) {
        SkipListSetNode<E> [] update = (SkipListSetNode<E> [])
            new SkipListSetNode[MAXLEVEL];
        SkipListSetNode<E> sln = header;
        int diff = 0;
        for(int i = MAXLEVEL - 1; i >= 0; i--) {    // search for insert pos
            while(sln.forward[i] != null) {
               if((diff = compare(o, sln.forward[i].getData())) > 0) {
                   sln = sln.forward[i];    // item bigger, move to next node
               }
               else if(diff == 0) {         // found item, no insert necessary
                   return false;
               }
               else {                       // item smaller, move to next ptr
                   break;
               }
            }
            update[i] = sln;        // add node to list of nodes to be updated
        }
        sln = new SkipListSetNode<E>(o, randomLevel());     // create new node

        // splice new node into list
        for(int i = 0; i < sln.getLevel(); i++) {
            sln.forward[i] = update[i].forward[i];
            update[i].forward[i] = sln;
        }
        numItems++;
        return true;
    }

    /**
     * Get random level for node creation using prob of this skiplist.
     * @return  an integer representing level new node should be
     */
    private int randomLevel() {
        int level = 1;
        while(rand.nextDouble() < PROB && level < MAXLEVEL) {
            level++;
        }
        return level;
    }

    /**
     * Removes object o from the skiplist, if it exists.
     * @param o the Object to remove.
     * @return  <code>true</code> if method call changed skiplist
     * @throws ClassCastException   if o is not an E
     * @see java.util.Collection#remove(java.lang.Object)
     */
    public boolean remove(Object o) {
        E e = (E) o;
        SkipListSetNode<E> [] update = (SkipListSetNode<E> [])
            new SkipListSetNode[MAXLEVEL];
        SkipListSetNode<E> sln = header;
        int diff = 0;
        for(int i = MAXLEVEL - 1; i >= 0; i--) {    // search for node 
            while(sln.forward[i] != null &&         // using biggest jumps
                    compare(e, sln.forward[i].getData()) > 0) {
                sln = sln.forward[i];
            }
            update[i] = sln;        // add pointers to update
        }
        // next node is null, the node to remove, or item is not in skiplist
        sln = sln.forward[0];
        if(sln == null || compare(e, sln.getData()) != 0) {
            return false;       // node null or item not in list
        }        
        // unsplice node from list using update pointers
        for(int i = 0; i < sln.getLevel(); i++) {
            update[i].forward[i] = sln.forward[i];
        }
        numItems--;
        return true;
    }

    /**
     * Empties SkipList 
     * @see java.util.Collection#clear()
     */
    public void clear() {
        for(int i = 0; i < MAXLEVEL; i++) {
            header.forward[i] = null;
        }
        numItems = 0;
    }
 
    /**
     * String representation of skiplist, useful for printing.
     * @return  String representation of this skiplist
     */
    public String toString() {
        SkipListSetNode<E> sln = header;
        String str = "numItems: " + numItems + "\n";
        for(int i = 0; i <= numItems; i++) {
            str += sln.toString();
            sln = sln.forward[0];
        }
        return str;
    }

    /**
     * Not Implemented. 
     * @throws UnsupportedOperationException    always - op not implemented
     */
    public <T> T[] toArray(T[] a) {
        throw new UnsupportedOperationException();
    }
    /**
     * Not Implemented. 
     * @throws UnsupportedOperationException    always - op not implemented
     */
    public boolean containsAll(Collection<?> c) {
        throw new UnsupportedOperationException();
    }
    /**
     * Not Implemented. 
     * @throws UnsupportedOperationException    always - op not implemented
     */
    public boolean addAll(Collection<? extends E> c) {
        throw new UnsupportedOperationException();
    }
    /**
     * Not Implemented.
     * @throws UnsupportedOperationException    always - op not implemented
     */
    public boolean retainAll(Collection<?> c) {
        throw new UnsupportedOperationException();
    }
    /**
     * Not Implemented.
     * @throws UnsupportedOperationException    always - op not implemented
     */
    public boolean removeAll(Collection<?> c) {
        throw new UnsupportedOperationException();
    }
    /**
     * Not Implemented.
     * @throws UnsupportedOperationException    always - op not implemented
     */
    public SortedSet<E> subSet(E fromElement, E toElement) {
        throw new UnsupportedOperationException();
    }
    /**
     * Not Implemented.
     * @throws UnsupportedOperationException    always - op not implemented
     */
    public SortedSet<E> headSet(E toElement) {
        throw new UnsupportedOperationException();
    }
    /**
     * Not Implemented.
     * @throws UnsupportedOperationException    always - op not implemented
     */
    public SortedSet<E> tailSet(E fromElement) {
        throw new UnsupportedOperationException();
    }
}
