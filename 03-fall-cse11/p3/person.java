public class Person
{  
  private String nickname, password;

//constructors  
  public Person(String nickname, String password)
  {
    this.nickname = nickname;
    this.password = password;
  }

  public Person(String netString)
  {
    password = netString.substring(netString.indexOf('\t') + 1);
    nickname = netString.substring(0, netString.indexOf('\t'));
  }

//accessors
  public String getNickname() { return nickname; }
  public String getPassword() { return password; }

//mutators
  public void setNickname(String nickname) { this.nickname = nickname; }
  public void setPassword(String password) { this.password = password; }

  //equality test
  public boolean equals(Object other)
  {
    //test if parameter object is a Person object
    if(other == null) return false;
    if(! (other instanceof Person) ) return false;
    Person otherCp = (Person) other;

    //test if object members are equal
    if(nickname == otherCp.getNickname())
      if(password == otherCp.getPassword())
	return true;
    return false;
  }

  //convert object to String
  public String toString()
  {
    return ("Nickname:" + nickname + "\n" + "Password:" + password);
  }

  //convert object to format used for communicating with server
  public String netString()
  {
    return (nickname + "\t" + password);
  }
}










