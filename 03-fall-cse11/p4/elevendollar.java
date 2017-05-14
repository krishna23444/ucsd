/*
Project: P4
File: ElevenDollar.java
By: Nitay Joffe
Date: Oct 29, 2003
login: cs11fcy

Class Description -
ElevenDollar is the user interface to the Account/Person software collection.
It is the front end, from the user's point of view. It prints a menu to the
screen and performs certain actions depending on varying user inputs. It also
handles the catching of exceptions that could be throws by the underlying
software, along with the switching between TransAccount and BuddyAccount.
*/

public class ElevenDollar
{
  public static void main(String args[])
  {
    //create an account to work with
    BetterAccount Accnt = new TransAccount();

    //stores the user's choice from the menu
    char choice = 'Y';

    //temporary variable used as middlemen from user input to method calling
    String user, nick, pass, email, trade, cmnt;
    double amnt;

    do
    {
      //output menu
      System.out.println("\nWelcome to the 11-dollar banking system");
      System.out.println("A)Retrieve personal information");
      System.out.println("B)Update account's personal information");
      System.out.println("C)Check account's balance");
      System.out.println("D)Transfer money to another account");
      System.out.println("E)View transaction history");
      System.out.println("F)Select a different account");
      System.out.println("X)Exit");

      //get user choice and convert to upper case
      choice = Character.toUpperCase(SavitchIn.readLineNonwhiteChar());

      //call appropriate method according to user input request
      switch(choice)
	{
	case 'A':
	  //get user input
	  System.out.println("Which account?(Leave blank for current account)");
	  user = SavitchIn.readLine();

	  //get user information from server and output using toString method
	  if(user.equals(""))
	    try
	    {
	      System.out.println(Accnt.getInformation().toString());
	    }
	    catch(NoBuddyException e)
	    {
	      System.out.println(e.getMessage());
	    }
	  else
	    System.out.println(Accnt.getInformation(user).toString());
	  break;

	case 'B':
	  //get nickname, password, email, and trades input
	  System.out.println("New Nickname?");
	  nick = SavitchIn.readLine();
	  System.out.println("New Password?");
	  pass = SavitchIn.readLine();
	  
	  if(Accnt instanceof BuddyAccount)
	  {
	    //try to update account's information
	    try
	    {
		Accnt.updateInformation(nick, pass);
	    }
	    //unsupported operation occured
	    catch(UnsupportedOperationException e)
	    {
	      System.out.println(e.getMessage());
	    }
	  }
	  //account is a TransAccount
	  else
	  {
	    System.out.println("New Email?");
	    email = SavitchIn.readLine();
	    System.out.println("New Trades?");
	    trade = SavitchIn.readLine();

	    System.out.println(Accnt.updateInformation(nick, pass,
						     email, trade));
	  }
	  break;

	case 'C':
	  // try to check account's balance
	  try
	  {
	    System.out.println("Account has " + Accnt.checkBalance() + " dollars");
	  }
	  //catch no buddy exception, if one occures
	  catch(NoBuddyException e)
	  {
	    System.out.println(e.getMessage());
	  }
	  break;

	case 'D':
	  //get user, amount, and comment input
	  System.out.println("To which account will you transfer?");
	  user = SavitchIn.readLine();
	  System.out.println("How much money?");
	  amnt = SavitchIn.readLineDouble();
	  System.out.println("Optional Comment?");
	  cmnt = SavitchIn.readLine();

	  //try to transfer money to user's account
	  try
	  {
	    System.out.println(Accnt.transferToAcct(user, amnt, cmnt));
	  }
	  //unsupported operation
	  catch(UnsupportedOperationException e)
	  {
	    System.out.println(e.getMessage());
	  }
	  break;

	case 'E':
	  if(!(Accnt instanceof TransAccount))
	    System.out.println("Invalid Operation");
	  else
	  {
	    //try to retrieve user's account history
	    try
	    {
	      System.out.println(((TransAccount)Accnt).getHistory());
	    }
	    //user does not exist
	    catch(NoUserException e)
	    {
	      System.out.println(e.getMessage());
	    }
	  }
	  break;

	case 'F':
	  System.out.println("Which user?(Leave blank for your own)");
	  user = SavitchIn.readLine();
	  
	  if(user.equals(""))
	  {
	    Accnt = new TransAccount();
	  }
	  else
	  {
	    System.out.println("Account password?");
	    pass = SavitchIn.readLine();
	    
	    //try to create a new BuddyAccount using inputted values
	    try
	    {
	      Accnt = new BuddyAccount(user, pass);
	    }
	    //invalid buddy or password input
	    catch(NoBuddyException e)
	    {
	      System.out.println(e.getMessage());
	    }
	  }
	  break;

	case 'X':
	  break;

	default:
	  break;
	}
    } while(choice != 'X');
  }
}





