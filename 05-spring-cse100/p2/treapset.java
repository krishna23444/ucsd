/*
 * Filename: TreapSet.java
 * Author: Nitay Joffe, Alvin Jeng
 * Date: Apr 21, 2005
 * Project: cse100-p2
 */

import java.util.*;

/**
 * Treap implementation of a SortedSet. Uses random priorities in heap order
 * together with keys in binary search tree order. Implements basic one-item
 * methods, and throws exceptions for the optional, unimplemented methods. The
 * node class is written in a separate class file, and the iterator is written
 * as an inner class because it makes it much easier and more efficient to code
 * an iterator when it has access to the collection its iterating through.
 * @author Nitay Joffe (njoffe), Alvin Jeng (apjeng)
 * @version Apr 21, 2005
 */
public class TreapSet<E> implements SortedSet<E> {
    private Comparator <? super E> c = null;
    private TreapSetNode<E> root = null;
    private int numItems = 0;

    /**
     * Default constructor. Use E's compareTo function (assume it implements
     * Comparable since no Comparator was passed in).
     */
    public TreapSet() {
        this(null);
    }

    /**
     * Constructor taking in Comparator to use for comparing data
     * @param c the generic Comparator treap should use
     */
    public TreapSet(Comparator<? super E> c) {
        this.c = c;
    }

    /**
     * Get treap's comparator (or null if using E's Comparable)
     * @return  Comparator if treap is using one, null otherwise
     * @see java.util.SortedSet#comparator()
     */
    public Comparator<? super E> comparator() {
        return c;
    }

    /**
     * Get first (lowest) element in treap
     * @return  lowest element in treap
     * @throws NoSuchElementException   if treap is empty
     * @see java.util.SortedSet#first()
     */
    public E first() {
        if(isEmpty()) {             // treap empty, no last element
            throw new NoSuchElementException();
        }
        TreapSetNode<E> tn = root;  // lowest element is all the way left
        while(tn.left != null) {
            tn = tn.left;
        }
        return tn.getData();
    }

    /**
     * Get last (highest) element in treap
     * @return  highest element in treap
     * @throws NoSuchElementException   if treap is empty
     * @see java.util.SortedSet#last()
     */
    public E last() {
        if(isEmpty()) {             // treap empty, no last element
            throw new NoSuchElementException();
        }
        TreapSetNode<E> tn = root;  // highest element is all the way right
        while(tn.right != null) {
            tn = tn.right;
        }
        return tn.getData();
    }

    /**
     * Get size of treap 
     * @return  number of elements in treap 
     * @see java.util.Collection#size()
     */
    public int size() {
        return numItems;
    }

    /**
     * Test for empty treap.
     * @return  <code>true</code> if treap is empty
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
        if(c == null) {             // no comparator passed in, use compreTo
            return ((Comparable<E>)e1).compareTo(e2);
        }
        else {                      // use comparator passed in
            return c.compare(e1, e2);
        }
    }

    /**
     * Check whether object is in treap.
     * @param o Object to search for in treap
     * @return  <code>true</code> if o is in treap
     * @throws ClassCastException   if o is not an E
     * @see java.util.Collection#contains(java.lang.Object)
     */
    public boolean contains(Object o) {
        try {
            lookup((E)o);
            return true;
        } catch(NoSuchElementException e) {
            return false;
        }
    }

    /**
     * Searches for the element E in the tree 
     * @param o the element E to search for in the tree
     * @throws NoSuchElementException   if element E is not found in treap
     */
    private TreapSetNode<E> lookup(E o) {
        TreapSetNode<E> tn = root;
        int diff = 0;
        while(tn != null) {
            diff = compare(o, tn.getData());
            if(diff < 0) {                      // item less than, go left
                tn = tn.left;
            }
            else if(diff == 0) {                // found our item
                return tn;
            }
            else {                              // item greater than, go right
                tn = tn.right;
            }
        }
        throw new NoSuchElementException();
    }

    /**
     * Removes Object o from the treap. Treap doesn't change if o wasn't
     * originally in the treap.
     * @param o the Object to remove
     * @return  <code>true</code> if Object was originally in treap
     * @throws ClassCastException   if o is not an E
     * @see java.util.Collection#remove(java.lang.Object)
     */
    public boolean remove(Object o) {
        try {
            // get object to remove
            TreapSetNode<E> tn = lookup((E)o);

            // ripple node down to leaf
            tn.rippleDown();                        
            // clear parent's pointer to this node
            if(tn.parent != null) {                 
                if(tn.parent.left == tn) {
                    tn.parent.left = null;
                }
                else {
                    tn.parent.right = null;
                }
            }

            numItems--;
            return true;
        } catch(NoSuchElementException e) {
            return false;
        }
    }

    /**
     * Inner class implementing Iterator for our treap. Has access to all of
     * the treap's variables and methods, which makes it easy to do its job.
     */
    private class treapIter implements Iterator<E> {
        private TreapSetNode<E> last = null, next = root;

        /**
         * Constructor for iterator, set next to first (lowest) item
         */
        public treapIter() {
            if(!isEmpty()) {              // start next at leftmost node
                while(next.left != null) {
                    next = next.left;
                }
            }
        }
        
        /**
         * Returns true if a call to next() would return an object, aka if
         * the iterator hasn't gone through the entire treap yet and there are
         * more objects.
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
            last = next;
            findNext();     // call findNext to set next pointer
            return last.getData();
        }

        /**
         * Set next pointer to successor of last element
         */
        private void findNext() {
             // node has a right child, its left subtree is successor
            if(next.right != null) {
                next = next.right;
                while(next.left != null) {
                    next = next.left;
                }
            }
            // node is left child, parent is successor
            else if(next.parent != null && next.parent.left == next) {
                next = next.parent;
            }
            // node is right child, climb up right spine till we find a node
            // that is a left child, that node is the successor
            else {
                while(next.parent != null && next.parent.right == next) {
                    next = next.parent;
                }
                next = next.parent;
            }
        }

        /**
         * Remove element returned by the last next() method call.
         * @throws IllegalStateException    if next method has not been called
         *      or remove method has already been called after last next call
         */
        public void remove() {
            if(last == null) {
                throw new IllegalStateException();
            }
            
            // ripple node down to leaf
            last.rippleDown();

            // delete parent's pointer to node
            if(last.parent != null) {
                if(last.parent.left == last) {
                    last.parent.left = null;
                }
                else {
                    last.parent.right = null;
                }
            }

            last = null;
            numItems--;
        }
    }
    
    /**
     * Get an Iterator for this treap, organized in sorted manner. Uses private
     * inner class to do all the dirty work.
     * @return  Iterator representing treap
     * @see java.lang.Iterable#iterator()
     */
    public Iterator<E> iterator() {
        return new treapIter();
    }

    /**
     * Return an array of Objects representing this treap, sorted from lowest
     * to highest priority.
     * @return  array of Objects corresponding to sorted elements in treap
     * @see java.util.Collection#toArray()
     */
    public Object[] toArray() {
        return null;
    }


    /**
     * Insert element into treap if it doesn't already exist
     * @param o the element E to insert
     * @return  <code>true</code> if method call changed treap
     * @see java.util.Collection#add(java.lang.Object)
     */
    public boolean add(E o) {
        int diff = 0;
        TreapSetNode<E> tn = root;
        if(isEmpty()) {                     // treap empty, create a root
            root = new TreapSetNode<E>(o);
        }
        else {
            while(tn != null) {
                diff = compare(o, tn.getData());
                if(diff < 0) {                              // go left
                    if(tn.left != null) {
                        tn = tn.left;
                    }
                    else {                  // no left child, found insert spot
                        tn.left = new TreapSetNode<E>(o, tn);
                        tn = tn.left;
                        break;
                    }
                }
                else if(diff == 0) {        // found item, don't insert
                    return false;
                }
                else {                                      // go right
                    if(tn.right != null) {  
                        tn = tn.right;
                    }
                    else {                  // no right child, found insert spot
                        tn.right = new TreapSetNode<E>(o, tn);
                        tn = tn.right;
                        break;
                    }
                }
            }
            goUp(tn, false);            // ripple node up to correct location
        }
        numItems++;
        return true;
    }

    /**
     * Empties treap.
     * @see java.util.Collection#clear()
     */
    public void clear() {
        root = null;
        numItems = 0;
    }

    /**
     * Bubble up the passed in node by rotating it with its parent. First we
     * find out which child the node is, then we ues the corresponding avl
     * rotation (with node priorities) to rotate the node up accordingly. There
     * are six pointers that need to be changed, since two nodes are moving,
     * and each node has three pointers, left, right, and parent.
     * @param toadd the TreapSetNode to bubble up
     * @param deleting  boolean flag to tell if we're deleting or not
     */
    private void goUp(TreapSetNode<E> toadd, boolean deleting) {
      if (toadd == root)
         return;

      TreapSetNode<E> tmpparent = toadd.parent;

      // ensure paren's priority is lower -> need to rotate
      if((toadd.getPriority() <= tmpparent.getPriority()) && !deleting)
          return;

      // Time to do a left rotation if it is a right child
      if (tmpparent.right == toadd) {
         if (toadd.left != null) { 
            tmpparent.right = toadd.left;
            (toadd.left).parent = tmpparent;
         }
         else
            tmpparent.right = null;

         if (tmpparent.parent != null) {
            TreapSetNode<E> tmppparent = tmpparent.parent;
            if (tmppparent.left == tmpparent) {
               tmppparent.left = toadd;
               toadd.parent = tmppparent;
            } else {
               tmppparent.right = toadd;
               toadd.parent = tmppparent;
            }
         } else {
            // new root
            root = toadd;
            toadd.parent = null;
         }
         toadd.left = tmpparent;
         tmpparent.parent = toadd;
      } else {
         if (toadd.right != null) {
            tmpparent.left = toadd.right;
            (toadd.right).parent = tmpparent;
         }
         else
            tmpparent.left = null;

         if (tmpparent.parent != null) {
            TreapSetNode<E> tmppparent = tmpparent.parent;
            if (tmppparent.left == tmpparent) {
               tmppparent.left = toadd;
               toadd.parent = tmppparent;
            } else {
               tmppparent.right = toadd;
               toadd.parent = tmppparent;
            }
         } else {
            // new root
            root = toadd;
            toadd.parent = null;
         }
         toadd.right = tmpparent;
         tmpparent.parent = toadd;
      }
      goUp(toadd, false);
    } 


    /**
     * String representation of TreapSet, useful for printing out object.
     * @return String   representation of this treap
     */
    public String toString() {
        return "numItems: " + numItems + "\nroot ->  " + root;
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
