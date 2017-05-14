public class Account
{
  private Person localPerson;

//constructors
  public Account()
  {
    localPerson = getInformation();
  }

  public Person getInformation()
  {
    String reply = ElevenClient.sendMessage("GETINFO");
    
    //test server reply to make sure it is in the correct form, and if so then
    //return a new Person object constructed using the netString constructor
    if(reply.indexOf('\t') >= 0)
      if(reply.substring(0,reply.indexOf('\t')).equals("YOURINFO"))
	return new Person(reply.substring(reply.indexOf('\t') + 1));

    //otherwise, nothing to return, so return a null pointer
    return null;
  }

  public Person getInformation(String username)
  {
    String reply = ElevenClient.sendMessage("GETINFO\t" + username);

    //test server reply to make sure it is in the correct form, and if so then
    //return a new Person object constructed using the netString constructor
    if(reply.indexOf('\t') >= 0)
      if(reply.substring(0, reply.indexOf('\t')).equals("USERINFO"))
	return new Person(reply.substring(reply.indexOf('\t') + 1));

    //otherwise, nothing to return, so return a null pointer
    return null;
  }

  public String updateInformation(String newNick, String newPass)
  {
    //update local account
    localPerson.setNickname(newNick);
    localPerson.setPassword(newPass);

    //update the server's account
    return ElevenClient.sendMessage("UPDATEINFO\t" + newNick + "\t" + newPass);
  }

  public double checkBalance()
  {
    String reply = ElevenClient.sendMessage("BALANCE");

    //check for any errors, and return a -1 if so
    if(reply.equals("ERROR")) return -1.0;

    //return double parsed from server return string, converted into dollars
    return Double.parseDouble(reply.substring(reply.lastIndexOf(' ') + 1)) /100;
  }

  public String transferToAcct(String user, double amount, String comment)
  {
    //convert amount to cents and round to nearest cent
    amount = Math.round(amount * 100);

    //transfer money using sendMessage method
    return ElevenClient.sendMessage("TRANSFER\t" + user + "\t" +
				    (int)amount + "\t" + comment);
  }
}










