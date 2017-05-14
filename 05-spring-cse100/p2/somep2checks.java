import java.util.*;

/**
 * Check some basic things about TreapSet and SkipListSet
 */
public class SomeP2Checks {

  public static void main(String args[]) {

    int probs = 0;

    SortedSet<String> t;	
    SortedSet<String> s;	

    // check default ctor for TreapSet
    t = new TreapSet<String>();
    // add an object
    if(!t.add(new String("foo"))) {
      System.out.println("Problem #1");
      probs++;
    }
    // see if it's there
    if(!t.contains((Object)(new String("foo")))) {
      System.out.println("Problem #2");
      probs++;
    }

    // check default ctor for SkipListSet
    s = new SkipListSet<String>();
    // add an object
    if(!s.add(new String("foo"))) {
      System.out.println("Problem #3");
      probs++;
    }
    // see if it's there
    if(!s.contains((Object)(new String("foo")))) {
      System.out.println("Problem #4");
      probs++;
    }

    // check one-arg ctor for TreapSet, inconsistent Comparator
    // testing to make sure Comparator is used
    t =
      new TreapSet<String>(new Comparator<String>()
	{ public int compare(String o1, String o2) {return 1;} });
    // add an object
    if(!t.add(new String("foo"))) {
      System.out.println("Problem #5");
      probs++;
    }
    // see if it's there (should say no because the Comparator never returns 0)
    if(t.contains(new String("foo"))) {
      System.out.println("Problem #6");
      probs++;
    }

    // check one-arg ctor for SkipListSet, inconsistent Comparator
    // testing to make sure Comparator is used
    s =
      new SkipListSet<String>(new Comparator<String>()
	{ public int compare(String o1, String o2) {return 1;} });
    // add an object
    if(!s.add(new String("foo"))) {
      System.out.println("Problem #7");
      probs++;
    }
    // see if it's there (should say no because the Comparator never returns 0)
    if(s.contains(new String("foo"))) {
      System.out.println("Problem #8");
      probs++;
    }

    // check non-Comparable data, 1-arg ctor
    SortedSet<Thing> tt =
      new TreapSet<Thing>(new Comparator<Thing>()
	{ public int compare(Thing o1, Thing o2)
	    { return o1.s.compareTo(o2.s); } });
    // add an object
    if(!tt.add(new Thing(new String("foo")))) {
      System.out.println("Problem #9");
      probs++;
    }
    // see if it's there
    if(!tt.contains(new Thing(new String("foo")))) {
      System.out.println("Problem #10");
      probs++;
    }

    // check non-Comparable data, 1-arg ctor
    SortedSet<Thing> st =
      new SkipListSet<Thing>(new Comparator<Thing>()
	{ public int compare(Thing o1, Thing o2)
	    { return o1.s.compareTo(o2.s); } });
    // add an object
    if(!st.add(new Thing(new String("foo")))) {
      System.out.println("Problem #11");
      probs++;
    }
    // see if it's there
    if(!st.contains(new Thing(new String("foo")))) {
      System.out.println("Problem #12");
      probs++;
    }

    System.out.println("Some simple checks found " + probs + " problems.");

  }

  private static class Thing {
    private String s;
    Thing(String s) {this.s = s;}
  }

}
  
