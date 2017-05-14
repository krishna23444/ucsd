/*
Project: P4
File: TransAccount.java
By: Nitay Joffe
Date: Oct 29, 2003
login: cs11fcy

Class Description -
TransAccount extends BetterAccount and adds to it one method which retrieves
the transaction history of the account from the server. Everything else
is inherited, including constructor (although constructors aren't actually
inherited, the default constructor which java creates automaticall simply
calls the superclass's constructor, which is also done by java automatically).
*/

public class TransAccount extends BetterAccount
{
  public String getHistory() throws NoUserException
  {
    String reply = ElevenClient.sendMessage("GET_HISTORY");
  
    //throw a no user exception if the server returned an error
    if(reply.equals("ERROR"))
      throw new NoUserException("Invalid User");
    else
      return reply;
  }
}
