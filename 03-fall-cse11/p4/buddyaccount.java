/*
Project: P4
File: BuddyAccount.java
By: Nitay Joffe
Date: Oct 29, 2003
login: cs11fcy

Class Description -
BuddyAccount extends BetterAccount, and adds functionality to make an object
especially suited for a friend's account on the server. This class handles
the storing of the buddy's login for authentication with the server, and it also
handles the permissions of what you may/may not do to a buddy's account. It
throws exceptions to let the user know they have made an illegal move while
still maintaining the flow of the program, rather then crashing.
*/

public class BuddyAccount extends BetterAccount
{
  private Person buddy;

  public BuddyAccount(String username, String password) throws NoBuddyException
  {
    //store the username separately, since the buddy object stores the nickname
    buddy = new Person(username, password);
    
    //retreive and store buddy's information - nickname, password, email, trades
    super.setPerson(getInformation());
  }

  public Person getInformation() throws NoBuddyException
  {
    String reply = ElevenClient.sendMessage("GETINFO\t" + buddy.getNickname() +
					    "\t" + buddy.getPassword());

    if(reply.equals("ERROR"))
    {
      //server returned error, so throw an exception describing the error
      throw new NoBuddyException("No Buddy, or incorrect Password for Buddy");
    }
    else
    {
      //create and return a new BetterPerson from server's reply
      return new BetterPerson(reply.substring(reply.indexOf('\t') + 1));
    }
  }

  public String updateInformation(String newNick, String newPass)
       throws UnsupportedOperationException
  {
    throw new UnsupportedOperationException("Invalid Operation");
  }

  public double checkBalance() throws NoBuddyException
  {
    String reply = ElevenClient.sendMessage("BALANCE\t" + buddy.getNickname() +
					    "\t" + buddy.getPassword());
    
    if(reply.equals("ERROR"))
    {
      //server returned error, so throw an exception describing the error
      throw new NoBuddyException("No Buddy, or incorrect Password for Buddy");
    }    
    else
    {
      //extract amount field from reply string (using BetterPerson.getField)
      //parse the amount as a double, and normalize it to human readable form
      return Double.parseDouble(BetterPerson.getField(reply, 2, ' ')) / 100;
    }
  }

  public String transferToAcct(String user, double amount, String comment)
       throws UnsupportedOperationException
  {
    throw new UnsupportedOperationException("Invalid Operation");
  }
}






