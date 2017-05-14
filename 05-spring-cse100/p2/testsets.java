/*
 * Filename: TestSets.java
 * Author: Nitay Joffe, Alvin Jeng
 * Date: Apr 21, 2005
 * Project: cse100-p2
 */

import java.util.*;

/**
 * TestSet tests the efficiency of the three different types of sorted sets,
 * which are: TreapSets, SkipListSets, and the redblack Trees implemented
 * with the Java package. This class will do 16384/N tests of each, and for
 * Each test it will attempt to locate N values in the sets.
 * The initial is to locate objects that are actually contained in the sets,
 * and the second portion is to locate objects that are not in the sets.
 * The output will give a table of comparisons between the MIN amount of
 * compares used, the AVG compares used in all of the tests, and the MAX.
 * @author Nitay Joffe (njoffe), Alvin Jeng (apjeng)
 * @version April 21, 2005
 */
public class TestSets {
   /**
    * The command line program that will test the different Sorted Sets
    */
   public static void main (String args[]) {
      CountingComparator<Integer> TS   = new CountingComparator();
      CountingComparator<Integer> SL   = new CountingComparator();
      CountingComparator<Integer> TR   = new CountingComparator();
      TreapSet<Integer> ts             = new TreapSet(TS);
      SkipListSet<Integer> sl          = new SkipListSet(SL);
      java.util.TreeSet<Integer> tr    = new java.util.TreeSet(TR);
      int max[]      = new int[3];        // Array to Hold max compares
      int min[]      = new int[3];        // Array to hold min compares
      int total[]    = new int[3];        // Array to hold totals
      int current[]  = new int[3];        // Array to hold temp getCount
      int avg[]      = new int[3];        // Avg for each Sets
      int MAXNUM     = 16384;

      System.out.println("Successful finds; number of comparisons" +
                         " MIN/AVG/MAX:\n" +
                         "Statistics collected over 16384/N runs, " +
                         "for each N.\n" +
                         "\t\tTreap\t\tSkipList\tTreeSet");

      /* Test from 2^3 - 1 objects in set, to n^15-1 objects */
      for (int i = 3; i < 15; i++) {
         int N = (int)Math.pow(2,i) - 1;

         /* Reset all the arrays */
         for (int a = 0; a < 3; a++) {
            max[a] = -1;
            min[a] = -1;
            total[a] = 0;
         }

         /* Add N odd numbers into the Set */
         for (int k = 0; k < N; k++)  {
            ts.add(2*k + 1);
            sl.add(2*k + 1);
            tr.add(2*k + 1);
         }

         /* Clear the coutn of the compares after adding */
         TS.reset();
         SL.reset();
         TR.reset();

         /* Commit to 16384/N number of attempts */
         for (int track = 0; track < MAXNUM/N; track++) {

            // Now do N locates
            for (int k = 0; k < N; k++) {
               // Check of the sets contain the odd number
               ts.contains(2*k + 1);
               sl.contains(2*k + 1);
               tr.contains(2*k + 1);

               // Obtain the amount of compares used within this check
               current[0] = TS.getCount();
               current[1] = SL.getCount();
               current[2] = TR.getCount();

               // Assign max, min, and increment total
               for (int c = 0; c < 3; c++) {

                  total[c] += current[c];
                  if ((current[c] > max[c]) || (max[c] == -1)) 
                     max[c] = current[c];

                  if ((current[c] < min[c]) || (min[c] == -1)) 
                     min[c] = current[c];
               }

               // Reset the number of compares
               TS.reset();
               SL.reset();
               TR.reset();
            }
         }

         // Compute the avg for all of the sets
         for (int b = 0; b < 3; b++) 
            avg[b] = total[b] / (MAXNUM);

         // Output the table results
         System.out.printf("N=%05d:\t%02d/%02d/%02d\t%02d/%02d/%02d\t",
            N, min[0], avg[0], max[0], min[1], avg[1], max[1]);
         System.out.printf("%02d/%02d/%02d\n", min[2], avg[2], max[2]);
      }

      System.out.println("\nUnsuccessful finds; number of comparisons" +
                         " MIN/AVG/MAX:\n" +
                         "Statistics collected over 16384/N runs, " +
                         "for each N.\n" +
                         "\t\tTreap\t\tSkipList\tTreeSet");

      // Do 2^3-1 => 2^15-1 objects stored in the sets
      for (int i = 3; i < 15; i++) {
         int N = (int)Math.pow(2,i) - 1;

         // Reset all of the max/min/total values
         for (int a = 0; a < 3; a++) {
            max[a] = -1;
            min[a] = -1;
            total[a] = 0;
         }

         // Add in all of the odd numbers up to N numbers in the set
         for (int k = 0; k < N; k++)  {
            ts.add(2*k + 1);
            sl.add(2*k + 1);
            tr.add(2*k + 1);
         }

         // Reset the amount of compares
         TS.reset();
         SL.reset();
         TR.reset();

         // Now do maxnum (16384) / N attempts  
         for (int track = 0; track < MAXNUM/N; track++) {

            // Now do N locates
            for (int k = 0; k < N; k++) {
               // See if the set contains an EVEN number, but it won't
               ts.contains(2*k);
               sl.contains(2*k);
               tr.contains(2*k);

               // Get all of the current getCounts
               current[0] = TS.getCount();
               current[1] = SL.getCount();
               current[2] = TR.getCount();

               // Set all of the min/max values and increment totals
               for (int c = 0; c < 3; c++) {
                  total[c] += current[c];
                  if ((current[c] > max[c]) || (max[c] == -1)) 
                     max[c] = current[c];

                  if ((current[c] < min[c]) || (min[c] == -1)) 
                     min[c] = current[c];
               }

               // Reset all of the values
               TS.reset();
               SL.reset();
               TR.reset();
            }
         }

         // Calculate the AVG
         for (int b = 0; b < 3; b++) 
            avg[b] = total[b] / (MAXNUM);

         System.out.printf("N=%05d:\t%02d/%02d/%02d\t%02d/%02d/%02d\t",
            N, min[0], avg[0], max[0], min[1], avg[1], max[1]);
         System.out.printf("%02d/%02d/%02d\n", min[2], avg[2], max[2]);
      }
   }
}
