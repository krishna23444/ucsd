public class ElevenDollar
{
  public static void main(String args[])
  {
    //create an account to work with
    Account Accnt = new Account();

    //stores the user's choice from the menu
    char choice = 'Y';

    //temporary variable used as middlemen from user input to method calling
    String user, nickname, password, comment;
    double amnt;

    do
    {
      //output menu
      System.out.println("\nWelcome to the 11-dollar banking system");
      System.out.println("A)Retrieve personal information");
      System.out.println("B)Update your personal information");
      System.out.println("C)Check your balance");
      System.out.println("D)Transfer money to another account");
      System.out.println("X)Exit");

      //get user choice and convert to upper case
      choice = SavitchIn.readLineNonwhiteChar();
      choice = Character.toUpperCase(choice);

      //call appropriate method according to user input request
      switch(choice)
	{
	case 'A':
	  //get user input
	  System.out.println("Which user?(Leave blank to retrieve your own)");
	  user = SavitchIn.readLine();

	  //get user information from server and output using toString method
	  if(user == "" || user == null)
	    System.out.println(Accnt.getInformation().toString());
	  else
	    System.out.println(Accnt.getInformation(user).toString());
	  break;

	case 'B':
	  //get nickname and password input
	  System.out.println("New Nickname?");
	  nickname = SavitchIn.readLine();
	  System.out.println("New Password?");
	  password = SavitchIn.readLine();

	  //update account information and output server's response
	  System.out.println(Accnt.updateInformation(nickname,password));
	  break;

	case 'C':
	  //check account's balance
	  System.out.println("You have " + Accnt.checkBalance() + " dollars.");
	  break;

	case 'D':
	  //get user, amount, and comment input
	  System.out.println("To which account will you transfer?");
	  user = SavitchIn.readLine();
	  System.out.println("How much money?");
	  amnt = SavitchIn.readLineDouble();
	  System.out.println("Optional Comment?");
	  comment = SavitchIn.readLine();

	  //transfer money and output server's response
	  System.out.println(Accnt.transferToAcct(user, amnt, comment));
	  break;

	case 'X':
	  break;

	default:
	  break;
	}
    } while(choice != 'X');
  }
}





