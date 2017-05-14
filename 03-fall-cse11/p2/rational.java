/*
* Rational.java
*
* Created on October 9, 2003, 12:53 PM
*/

//package P2;

/**
*
* @author  Nitay J
*/
public class Rational {

  //private variables
  private long num, denom;

//constructors section  
  //default construtor, rational set to 0/1
  public Rational()
  {
    num = 0;
    denom = 1;
  }

  //constructor with numerator passed to it, ratioanl set to num/1
  public Rational(long num)
  {
    this.num = num;
    denom = 1;
  }

  //constructor with both parameters passed to it, set to num/denom
  public Rational(long num, long denom)
  {
      this.num = num;
      this.denom = denom;
  }

//accessors section
  //methods to allow public access to value in numerator and denominator
  public long getNumerator() { return num; }
  public long getDenominator() { return denom; }

//conversions section
  //convert object to a string
  public String toString()
  {
    return (String.valueOf(num) + "/" + String.valueOf(denom));
  }
  
  //convert object to a double
  public double toDouble()
  {
    return (double)num/denom;
  }

//mutators section
  //reduces this fraction to its simplest form
  public void reduce()
  {
    boolean negative = false;
    long gcd = 1;

    //test if Rational is negative
    //and if so, then remove & store negative for later
    if(num < 0)
    {
      negative = !negative;
      num *= -1;
    }
    if(denom < 0)
    {
      negative = !negative;
      denom = -1;
    }
    
    //use euclid's method to find greatest common divisor
    if(num > denom)
      gcd = euclid(num,denom);
    else
      gcd = euclid(denom,num);

    //divide both parts by greatest common divisor
    num /= gcd;
    denom /= gcd;

    //return the negative sign if there was one originally
    if(negative == true)
      num *= -1;
  }
  
//mathematics methods (equal, add, subtract, multiply, divide)
  //equality test for the Rational object
  public boolean equals(Object other) 
  {
    //test if Object is of the Rational class
    if (other == null) return false;
    if (! (other instanceof Rational) ) return false;
    Rational rother = (Rational) other;
    
    //create copies of Rational objects so not to alter originials
    Rational thisCp = new Rational(this.num, this.denom);
    Rational rotherCp = new Rational(rother.getNumerator(),
      rother.getDenominator());
    
    //reduce both Rational copy objects
    thisCp.reduce();
    rotherCp.reduce();
    
    //test if both numerator and denominator are equal
    if(thisCp.getNumerator() == rotherCp.getNumerator())
      if(thisCp.getDenominator() == rotherCp.getDenominator())
        return true;
    
    //if program gets here, objects are not equal
    return false;
  }

  //recursive euclid's method algorithm to find greatest common divisor
  private long euclid(long a, long b)
  {
    if(b == 0)
      return a;
    else
      return euclid(b,a % b);
  }

  //add this rational to parameter rational and return result
  public Rational add(Rational other)
  {
    long numResult, denomResult;
    
    //put both fractions in common denominator and add
    numResult = num * other.getDenominator() + denom * other.getNumerator();
    denomResult = denom * other.getDenominator();
    
    return new Rational(numResult, denomResult);
  }

  //subtract this rational by parameter rational
  public Rational subtract(Rational other)
  {
    long numResult, denomResult;
    
    //put both fractions in common denominator and subtract
    numResult = num * other.getDenominator() - denom * other.getNumerator();
    denomResult = denom * other.getDenominator();
    
    return new Rational(numResult, denomResult);
  }

  //multiply this rational with parameter rational and return result
  public Rational multiply(Rational other)
  {
    long numResult, denomResult;
    
    numResult = num * other.getNumerator();
    denomResult = denom * other.getDenominator();
    
    return new Rational(numResult, denomResult);
  }

  //divide this rational with parameter rational and return result
  public Rational divide(Rational other)
  {
    long numResult, denomResult;
    
    //use flip-and-multiply routine
    numResult = num * other.getDenominator();
    denomResult = denom * other.getNumerator();
    
    return new Rational(numResult, denomResult);
  }
}
