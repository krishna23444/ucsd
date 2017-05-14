import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import onyx.xml.OnyxAttributeEnvironment;
import onyx.xml.OnyxElement;

/**
 * Driver for onyx parser.
 * @author Kevin Clark
 * @author Nitay Joffe
 */
public class ParserDriver {
  /**
  * @param args  Array of command line args passed in, args[0] = input file.
  */
  public static void main(String [] args) {
    EmitXMLVisitor visitor = new EmitXMLVisitor();

    try {
      Parser parser = new Parser(new Scanner(new FileReader(args[0])));

      // print stack traces for debugging
      if (args.length > 1 && args[1].equals("debug"))
        Parser.debug = true;

      // parse
      Query query = (Query)parser.parse().value;

      // create abstract syntax tree
      query.accept(visitor);

      // write output to file, if not debugging
      if (args.length <= 1 || !args[1].equals("debug")) {
        PrintWriter output = new PrintWriter(new FileWriter(args[0] + ".xml"));
        output.print(visitor.getRoot() + "");
        output.close();
      }
    } catch (LexicalErrorException e) {
      OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
      attr.add("line", (new Integer(e.getLine())).toString());
      attr.add("column", (new Integer(e.getColumn())).toString());
      OnyxElement error = new OnyxElement("syntaxError", attr);
      
      try {
        PrintWriter output = new PrintWriter(new FileWriter(args[0] + ".xml"));
        output.print(error + "");
        output.close();
      } catch (IOException ioe) {
        ioe.printStackTrace(System.err);
      }
    } catch (FileNotFoundException e) {
      e.printStackTrace(System.err);
    } catch (IOException e) {
      e.printStackTrace(System.err);
    } catch (Exception e) {
      e.printStackTrace(System.err);
    }
  }
}
