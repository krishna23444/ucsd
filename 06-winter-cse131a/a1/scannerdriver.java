import java.io.*;

import java_cup.runtime.*;
import onyx.xml.*;

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
  public static void main(String args[]) {
    try {
      Scanner scanner = new Scanner(new FileReader(args[0]));
      PrintWriter output = new PrintWriter(new FileWriter(args[0] + ".xml"));

      Symbol token;
      int offset;

      OnyxAttributeEnvironment attributes = new OnyxAttributeEnvironment();

      attributes.add("file", args[0]);
      OnyxElement root = new OnyxElement("OnyxSource", attributes);

      do {
        token = scanner.next_token();
        attributes = new OnyxAttributeEnvironment();
        
        // Offset to make line and column counting start at 1
        offset = (token.sym != onyx_sym.EOF) ? 1 : 0;

        attributes.add("column", (token.right + offset) + "");
        attributes.add("id", token.sym + "");
        attributes.add("line", (token.left + offset) + "");

        if (token.value != null)
          root.addChild(new OnyxTextNode("token", attributes, token.value+""));
        else
          root.addChild(new OnyxElement("token", attributes));

      } while (token.sym != onyx_sym.EOF);

      output.println(root + "");
      output.close();
    }
    catch (FileNotFoundException e) {
      e.printStackTrace(System.out);
    }
    catch (IOException e) {
      e.printStackTrace(System.out);
    }
  }
}
