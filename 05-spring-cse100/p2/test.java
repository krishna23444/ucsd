import java.util.*;

class Test {
    public static void main( String argv[] ) {
        /*
        SkipListSet<Integer> list = null;
        Iterator i = null;
        TreapSet<Integer> set = null;
        */
/*
        list = new SkipListSet<Integer>();
        list.add(5);
        list.add(2);
        list.add(6);
        list.add(2);
        System.err.println(list);

        System.err.println("List contains 2?: " + list.contains(2) );
        System.err.println(list);
        System.err.println("List contains 10?: " + list.contains(10) );
        System.err.println("Trying to remove 2: " + list.remove(2));
        System.err.println("Trying to remove 10: " + list.remove(10));

/*
        System.err.println("List is empty: " + list.isEmpty() );
        System.err.println("List has " + list.size() + " elements");
        System.err.println("First element in list is: " + list.first());
        System.err.println("Last element in list is: " + list.last());
        list.clear();
        System.err.println("List has been cleared");
        System.err.println("List now has " + list.size() + " elements");
        System.err.println(list);
//        System.err.println("First element in list is: " + list.first());
//        System.err.println("Last element in list is: " + list.last());


        list.add(5);
        list.add(1);
        list.add(4);
        list.add(2);
        list.add(3);

        i = list.iterator();
        while(i.hasNext()) System.err.println("i is: " + i.next());

        Object[] array = list.toArray();
        for(Object o: array) System.err.println("o is: " + o);
        array[3] = 75;
        for(Object o: array) System.err.println("o is: " + o);
        list = new SkipListSet<Integer>();

        list.add(5);
        list.add(1);
        list.add(4);
        list.add(2);
        list.add(3);

        i = list.iterator();
        System.err.println(list);
        System.err.println(i.next());
        System.err.println(i.next());
        i.remove();
        System.err.println(list);
        i = list.iterator();
        while(i.hasNext()) System.err.println("i is: " + i.next());

        System.err.println(list);
        i = null;
        System.err.println("Removing 3: " + list.remove(3));
        System.err.println(list);
*/
        TreapSet<Integer> set = new TreapSet<Integer>();
        int count = 30;
        for(int j = 1; j < count; j ++) { 
            set.add(2 * j + 1);
        }
        System.err.println("contains 40? " + set.contains(40));

        for(Integer k : set) System.err.print(k + " ");
        System.err.println("\n");

        System.err.println("First: " + set.first());
        System.err.println("Last: " + set.last());
        System.err.println("Contains 2? : " + set.contains(2));
        System.err.println("Contains 3? : " + set.contains(3));
        set.remove(3);
        System.err.println("removed 3...");
        System.err.println("Contains 3? : " + set.contains(3));
        System.err.println("Contains 6? : " + set.contains(6));
        set.remove(6);
        System.err.println("removed 6...");
        System.err.println("Contains 6? : " + set.contains(6));

        Iterator<Integer> i = set.iterator();
        System.err.println(i.next());
        System.err.println(i.next());
        System.err.println(i.next());
        System.err.println("removing last thing: ");
        i.remove();
        for(Integer k : set) System.err.print(k + " ");
        System.err.println();
        System.err.println(i.next());
        System.err.println(i.next());
        System.err.println("Removing 25?" + set.remove(25));
        System.err.println(i.next());
        System.err.println("removing last thing: ");
        i.remove();
        for(Integer k : set) System.err.print(k + " ");
        System.err.println();
        System.err.println(i.next());
        System.err.println("Removing 37?" + set.remove(37));
        System.err.println(i.next());
        System.err.println("removing last thing: ");
        i.remove();
        System.err.println(i.next());
        System.err.println("Removing 17?" + set.remove(17));
        System.err.println(i.next());
        System.err.println("removing last thing: ");
        i.remove();
        System.err.println(i.next());
        System.err.println("removing last thing: ");
        i.remove();
        for(Integer k : set) System.err.print(k + " ");
        System.err.println();

        /*
        CountingComparator<Integer> cc =  new CountingComparator();
        reapSet<Integer> ts = new TreapSet(cc);

        for(int i = 2; i < 15; i++) {
            ts.add(i);
        }
        System.err.println("\nCOMPARING...\n");
        ts.contains(5);
        */
    }
}
