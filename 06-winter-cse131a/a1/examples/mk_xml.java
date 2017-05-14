import onyx.xml.*;

public class mk_xml
{

  public static void main(String[] argv ) {

	OnyxAttributeEnvironment attrenv = new OnyxAttributeEnvironment();
	attrenv.add("A1", "left");
	attrenv.add("A2", "3.14159");
	OnyxElement root = new OnyxElement("root",attrenv);

	root.addChild(new OnyxTextNode("one", 1+""));
	root.addChild(new OnyxTextNode("two", 2+""));
	root.addChild(new OnyxTextNode("three", 3+""));


	System.out.print(root+"");

   }

}
