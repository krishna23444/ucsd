import java.io.PrintStream;

import onyx.xml.OnyxAttributeEnvironment;
import onyx.xml.OnyxElement;
import onyx.xml.OnyxTextNode;
//import java.io.FileReader;

/**
 * Driver for onyx parser.
 * @author Kevin Clark
 * @author Nitay Joffe
 */
public class Interpreter {
  private static final String ONYX_ROOT_ELEMENT = "onyx-result";
  private static final String ONYX_RESULT = "Result";
  private static final String PARSE_ERROR = "onyx.error.ParserError";
  private static final String SEMANTIC_ERROR = "onyx.error.SemanticError";
  private static final String STATIC_ERROR = "StaticError";
  private static final String DYNAMIC_ERROR = "DynamicError";
  private static final String ERROR_MESSAGE = "ErrorMessage";
  private static final String MIXED_CONTENT = 
    "Top level sequence cannot contain mixed content: ";

  private static OnyxAttributeEnvironment createEnv(int line, int column) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("line", new Integer(line).toString());
    attr.add("column", new Integer(column).toString());
    return attr;
  }
  
  private static OnyxElement createRootNode(String name) {
    OnyxElement node = new OnyxElement(name);
    return node;
  }
  
  private static int numOnyxNodesInSequence(SequenceType seq) {
    int num = 0;
    for (int i = 0; i < seq.size(); ++i)
      if (seq.get(i) instanceof NodeType)
        num++;
    return num;
  }
  
  public static void main(String [] args) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    EvalVisitor visitor = new EvalVisitor();
    OnyxElement root = null;
    PrintStream output = System.out;
    
    try {
      Parser parser = new Parser(new Scanner(System.in));
      
      visitor.visit((Query)parser.parse().value);
      
      AnyType retval = visitor.peek();
      
      root = new OnyxElement(ONYX_ROOT_ELEMENT);
      
      if (SequenceType.class == retval.getClass()) {
        SequenceType seq = (SequenceType)retval;
        int num_nodes = numOnyxNodesInSequence(seq);
        // sequence is full of onyx nodes, need to add each one to root
        if (num_nodes == seq.size())
          for (int i = 0; i < seq.size(); ++i)
            root.addChild(((NodeType)seq.get(i)).getNode());
        // sequence has no onyx nodes, need to create "Result" node with data
        else if (num_nodes == 0)
          root.addChild(new OnyxTextNode(ONYX_RESULT, retval.stringOf()));
        else
          throw new SemanticDynamicErrorException(MIXED_CONTENT
                                                  + "(" + retval.stringOf()
                                                  + ")");
      } else if (retval instanceof NodeType)
        root.addChild(((NodeType)retval).getNode());
      else
        root.addChild(new OnyxTextNode(ONYX_RESULT, retval.stringOf()));

      // lexical syntax error
    } catch (LexicalErrorException e) {
      output = System.err;
      root = new OnyxElement(PARSE_ERROR);
      attr = createEnv(e.getLine(), e.getColumn());
      root.addChild(new OnyxTextNode(STATIC_ERROR, attr,
                                     "Lexical Error: " + e.getMessage()));
      // grammatical parsing error
    } catch (SyntaxErrorException e) {
      output = System.err;
      root = new OnyxElement(PARSE_ERROR);
      attr = createEnv(e.getLine(), e.getColumn());
      
      root.addChild(new OnyxTextNode(STATIC_ERROR, attr, "Syntax Error"));
      
      
      // function prototype matching error with a list of matching prefixes
    } catch (FunctionPrototypeNotFoundPrefixesException e) {
      output = System.err;
      root = createRootNode(SEMANTIC_ERROR);
      attr = createEnv(e.getLine(), e.getColumn());
      OnyxElement dyn_error = new OnyxElement(DYNAMIC_ERROR, attr);
      dyn_error.addChild(new OnyxTextNode(ERROR_MESSAGE, e.getMessage()));
      for (FunctionPrototype fp : e.getPossibleMatches())
        dyn_error.addChild(fp.toPossibleMatchOnyxNode());
      root.addChild(dyn_error);
      
      // function prototype matching error with a list of all candidate matches
    } catch (FunctionPrototypeNotFoundCandidatesException e) {
      output = System.err;
      root = createRootNode(SEMANTIC_ERROR);
      attr = createEnv(e.getLine(), e.getColumn());
      OnyxElement dyn_error = new OnyxElement(DYNAMIC_ERROR, attr);
      dyn_error.addChild(new OnyxTextNode(ERROR_MESSAGE, e.getMessage()));
      for (String fp : e.getPossibleMatches())
        dyn_error.addChild(new OnyxTextNode("PossibleMatch", fp));
      root.addChild(dyn_error);
      
      // semantic abstract syntax tree static error
    } catch (SemanticStaticErrorException e) {
      output = System.err;
      root = createRootNode(SEMANTIC_ERROR);
      attr = createEnv(e.getLine(), e.getColumn());
      root.addChild(new OnyxTextNode(STATIC_ERROR, attr, e.getMessage()));
      
      // semantic dynamic runtime error
    } catch (SemanticDynamicErrorException e) {
      output = System.err;
      root = createRootNode(SEMANTIC_ERROR);
      if (e.getLine() != -1 && e.getColumn() != -1) {
        attr = createEnv(e.getLine(), e.getColumn());
        root.addChild(new OnyxTextNode(DYNAMIC_ERROR, attr, e.getMessage()));
      } else {
        root.addChild(new OnyxTextNode(DYNAMIC_ERROR, e.getMessage()));
      }
    } catch (Exception e) {
      e.printStackTrace();
    }
    
    output.println(root + "");
  }
}
