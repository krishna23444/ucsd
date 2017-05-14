/**
 * A Comparator that can count the number of times its
 * compare() method has been called.
 * Its compare() method uses the fact that its arguments are Comparable, and
 * just compares them using their natural ordering.
 * @author Paul Kube (c) 2005
 */
public class CountingComparator<T extends Comparable<T>>
  implements java.util.Comparator<T> {

  private int count;

  /**
   * Create a new CountingComparator, with initial count == 0.
   */
  public CountingComparator() { count = 0; }

  /**
   * Compare two Comparable objects, and increment the comparison count.
   * @param o1 The first object
   * @param o2 The second object.
   * @return <code> o1.compareTo(o2)</code>
   */
  public int compare(T o1, T o2) {
    count++;
    return o1.compareTo(o2);
  }

  /**
   * Reset the comparison count for this CountingComparator to 0.
   */
  public void reset() { count = 0; }

  /**
   * Return the current comparison count.
   * @return the current comparison count.
   */
  public int getCount() { return count; }

}
