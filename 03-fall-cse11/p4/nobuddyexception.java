/*
Project: P4
File: NoBuddyException.java
By: Nitay Joffe
Date: Oct 29, 2003
login: cs11fcy

Class Description -
NoBuddyException extends RuntimeExcetion, and is thrown when an invalid buddy
is being used to access data on the server (which doesn't exist since that buddy
 does not exist). 
This exception does NOT need to be in the throws clause of a method to be used.
This exception is used in the BuddyAccount class.
*/

public class NoBuddyException extends RuntimeException
{
  public NoBuddyException(String message)
  {
    super(message);
  }
}
