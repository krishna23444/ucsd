/*
Project: P4
File: NoUserException.java
By: Nitay Joffe
Date: Oct 29, 2003
login: cs11fcy

Class Description -
NoUserException extends Excetion, and is thrown when an invalid user is trying 
to access data on the server (which doesn't exist since that user does not
exist). 
This exception must be declared in the throws clause of a method to be used.
This exception is used in the TransAccount class.
*/

public class NoUserException extends Exception
{
  public NoUserException(String message)
  {
    super(message);
  }
}
