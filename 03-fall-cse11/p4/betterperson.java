/*
Project: P4
File: BetterPerson.java
By: Nitay Joffe
Date: Oct 29, 2003
login: cs11fcy

Class Description -
BetterPerson extends Person, which has two private variables as follows:
String nickname and String password. This class adds the String email and
String trades to the Person object, along with providing the appropriate
constructors, mutators/accessors, and utility functions.

This class also has the public static method getField which can be used
to extract fields from a string that has fields separated by a given delimiter.
*/

public class BetterPerson extends Person
{
  private String email, trades;

  public BetterPerson(String name, String password, String email, String trades)
  {
    //use superclass's constructor to initialize name and password
    super(name, password);
    this.email = email;
    this.trades = trades;
  }

  public BetterPerson(String netString)
  {
    //netString is in form of [name"\t"password"\t"email"\t"trades]
    //use superclass's constructor to initialize name and password
    super(netString);
    this.email = getField(netString,2,'\t');
    this.trades = getField(netString,3,'\t');
  }

  public String getEmail() { return email; }
  public String getTrades() { return trades; }

  public void setEmail(String email) { this.email = email; }
  public void setTrades(String trades) { this.trades = trades; }

  public String toString()
  {
    //use superclass's toString function, and append this object's variables
    return super.toString() + "\nEmail:" + email + "\nTrades:" + trades;
  }

  public String netString()
  {
    //use superclass's toString function, and append this object's variables
    return super.netString() + "\t" + email + "\t" + trades;
  }

  //helper function to parse a string, using delimiters, and return field
  public static String getField(String s, int field, char delim)
  {
    //return entire string if there are no delimiters
    if(s.indexOf(delim) == -1) return s;

    //used to store beginning and end of the field, respectively
    //initialized to position of first field (field #0)
    int fStart = 0, fEnd = s.indexOf(delim);

    //loop until correct field is reached, or until end of string
    //return an empty string if a field out of range is requested
    for(int i = 0; i < field; i++)
    {
      //out of bounds field requested, return empty string
      if(fEnd == s.length()) return "";
      
      //beginning of next field = end of previous field + 1
      fStart = fEnd + 1;

      //end of next field = position of next delimiter
      fEnd = s.indexOf(delim, fStart);

      //if there is no next delimiter, end of next field = end of string
      if(fEnd == -1) fEnd = s.length();
    }

    //return substring <field requested> in range of fStart and fEnd
    return s.substring(fStart, fEnd);
  }
}









