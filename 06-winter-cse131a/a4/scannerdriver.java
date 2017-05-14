import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.FileWriter;
import java.io.PrintWriter;
import onyx.xml.OnyxAttributeEnvironment;
import onyx.xml.OnyxElement;
import onyx.xml.OnyxTextNode;
import java_cup.runtime.Symbol;

/**
 * Driver for onyx lexical scanner.
 * @author Kevin Clark
 * @author Nitay Joffe
 */
public class ScannerDriver {
  /**
   * Scans an onyx file using the Scanner class created by jflex, and outputs
   * onyx xml tokens to a file with the same name as the input and a .xml
   * extension.
   * @param args  Array of command line arguments passed in, args[0] = input
   *              file.
   */
  public static void main(String [] args) {
    try {
      Scanner scanner = new Scanner(new FileReader(args[0]));

      Symbol token;

      OnyxAttributeEnvironment attributes =
        new OnyxAttributeEnvironment();

      attributes.add("file", args[0]);
      OnyxElement root = new OnyxElement("OnyxSource", attributes);

      do {
        token = scanner.next_token();
        attributes = new OnyxAttributeEnvironment();
      
        attributes.add("column", (token.right) + "");
        attributes.add("id", token.sym + "");
        attributes.add("line", (token.left) + "");

        if (token.value != null)
          root.addChild(new OnyxTextNode("token", attributes,
                                                 token.value+""));
        else
          root.addChild(new OnyxElement("token", attributes));

      } while (token.sym != sym.EOF);

      if (args.length < 2) {
        PrintWriter output = new PrintWriter(new FileWriter(args[0] + ".xml"));
        output.println(root + "");
        output.close();
      } else
        System.err.println(root + "");
    } catch (FileNotFoundException e) {
      e.printStackTrace(System.out);
    } catch (IOException e) {
      e.printStackTrace(System.out);
    }
  }
}
