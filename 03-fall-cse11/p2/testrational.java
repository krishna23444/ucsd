/*
 * TestRational.java
 *
 * Created on October 13, 2003, 1:44 AM
 */

//package P2;

/**
 *
 * @author  Nitay J
 */
public class TestRational {
  /**
   * @param args the command line arguments
   */
  public static void main(String[] args)
  {
    Rational r1, r2;
    long num, denom;
    char done = 'n';

    while(done != 'y')
    {
      System.out.println("\n\nTesting constructors, accessors, toString():");
      r1 = new Rational();
      System.out.println("Rational created using default constructor:");
      System.out.println("Using accessors, this is " +
			 r1.getNumerator() + "/" + r1.getDenominator());
      System.out.println("Using toString(), this is " + r1.toString());

      System.out.println("\nTesting 1-arg constructor:");
      System.out.print("Enter numerator: ");
      num = SavitchIn.readLineLong();
      r1 = new Rational(num);
      System.out.println("Using accessors, this is " +
      r1.getNumerator() + "/" + r1.getDenominator());
      System.out.println("Using toString(), this is " + r1.toString());

      System.out.println("\nTesting 2-arg constructor:");
      System.out.print("Enter first numerator: ");
      num = SavitchIn.readLineLong();
      System.out.print("Enter first denominator: ");
      denom = SavitchIn.readLineLong();
      r1 = new Rational(num, denom);
      System.out.println("Using accessors, this is " +
			 r1.getNumerator() + "/" + r1.getDenominator());
      System.out.println("Using toString(), this is " + r1.toString());

      System.out.println("------------------");
      System.out.print("Enter second numerator: ");
      num = SavitchIn.readLineLong();
      System.out.print("Enter second denominator: ");
      denom = SavitchIn.readLineLong();
      r2 = new Rational(num, denom);
      System.out.println("Using accessors, this is " +
			 r2.getNumerator() + "/" + r2.getDenominator());
      System.out.println("Using toString(), this is " + r2.toString());

      System.out.println("\nTesting numeric operations:");
      System.out.println(r1.toString() + " + " + r2.toString() + " = " +
			 r1.add(r2).toString());
      System.out.println(r1.toString() + " - " + r2.toString() + " = " + 
			 r1.subtract(r2).toString());
      System.out.println(r1.toString() + " * " + r2.toString() + " = " + 
			 r1.multiply(r2).toString());
      System.out.println(r1.toString() + " / " + r2.toString() + " = " + 
			 r1.divide(r2).toString());

      System.out.println("\nTesting equality test:");
      System.out.print(r1.toString() + " and " + r2.toString() + " are ");
      if(r1.equals(r2) == true)
        System.out.println("equal.");
      else
        System.out.println("not equal.");

      System.out.println("\nTesting conversion to double:");
      System.out.println(r1.toString() + " as a double is " + r1.toDouble());
      System.out.println(r2.toString() + " as a double is " + r2.toDouble());

      System.out.println("\nTesting normalization:");
      System.out.print(r1.toString() + " reduced is ");
      r1.reduce();
      System.out.println(r1.toString());
      System.out.print(r2.toString() + " reduced is ");
      r2.reduce();
      System.out.println(r2.toString());

      System.out.print("\nDone testing? (y or n): ");
      done = SavitchIn.readLineNonwhiteChar();
    }
  } 
}



