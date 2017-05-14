/*
Project: P4
File: BetterAccount.java
By: Nitay Joffe
Date: Oct 30, 2003
login: cs11fcy

Class Description:
BetterAccount extends Account, and adds on functionality by providing methods
which work with BetterPerson. Therefore, many of Account's methods are
overriden in this class so that they get/use the new (extended) datatype.
Those methods which don't rely on the Person class, such as transferToAcct
and checkBalance do not need to be overriden and are simply inherited.

This class also defines public static getInfo, which is used to parse
server replies and create BetterPerson objects (assuming the reply is in the
netString form). This method is used both by the default constructor and both
getInformation methods, therefore it is useful since it reuses code.
*/

public class BetterAccount extends Account
{
  public BetterAccount()
  {
    //use getInfo to set the Person stored in the superclass(Account)
    super.setPerson(getInfo(ElevenClient.sendMessage("GETINFO"), '\t'));
  }

  public Person getInformation()
  {
    //use getInfo to return a BetterPerson parsed from server response
    return getInfo(ElevenClient.sendMessage("GETINFO"), '\t');
  }

  public Person getInformation(String username)
  {
    //use getInfo to return a BetterPerson parsed from server response
    return getInfo(ElevenClient.sendMessage("GETINFO\t" + username), '\t');
  }

  public String updateInformation(String newNick, String newPass,
				  String newEmail, String newTrades)
  {
    //update the info on the server with parameters passed to this method
    return ElevenClient.sendMessage("UPDATEINFO\t" + newNick + "\t" + newPass +
				    "\t" + newEmail + "\t" + newTrades);
  }

  //helper function to parse server replies and return a BetterPerson object
  //expects a string in form *<delim>user<delim>pass<delim>email<delim>trades
  public static BetterPerson getInfo(String msg, char delim)
  {
    //String parameter passed (from server) is an error
    if(msg.equals("ERROR") == true)
      return null;

    //used to test whether msg is in correct (expected) form
    int i, delimLocation = msg.indexOf(delim);
    
    //test msg string using indexOf with incremental start positions
    for(i = 0; i < 4 && ++delimLocation > 0; i++)
      delimLocation = msg.indexOf(delim, delimLocation);

    //string has four delimiters as expected, so parse and return BetterPerson
    if(i == 4) return new BetterPerson(msg.substring(msg.indexOf('\t') + 1));

    //string parameter is not in the correct (expected) form
    else return null;
  }
}
