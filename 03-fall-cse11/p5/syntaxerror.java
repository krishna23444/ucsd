/*
Project: P5
File: SyntaxError.java
By: Nitay Joffe
Date: Nov 6, 2003
login: cs11fcy

Class Description:
 *SyntaxError extends Excetion. SyntaxError is used in the aguac class. It is
 *Thrown/used to describe syntax errors that may occur in an agua file. Syntax
 *errors in agua files come in a wide variety and range from invalid number of
 *tokens to a miscorrect spelling of a command, or even nto capitalizing.
*/


public class SyntaxError extends Exception
{
  public SyntaxError(String message)
  {
    super(message);
  }  
}
