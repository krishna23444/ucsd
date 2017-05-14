import java.util.HashSet;
import java.util.Set;

public class Ops {
  public static final String ADD = "op:numeric-add";
  public static final String SUBTRACT = "op:numeric-subtract";
  public static final String MULTIPLY = "op:numeric-multiply";
  public static final String DIVIDE = "op:numeric-divide";
  public static final String INTEGER_DIVIDE = "op:numeric-integer-divide";
  public static final String MOD = "op:numeric-mod";
  public static final String UNARY_PLUS = "op:numeric-unary-plus";
  public static final String UNARY_MINUS = "op:numeric-unary-minus";
  public static final String TO = "op:to";
  public static final String AND = "op:and";
  public static final String OR = "op:or";
  public static final String EQUALS = "op:equals";
  public static final String NOT_EQUALS = "op:not-equals";
  public static final String LESS_THAN = "op:less-than";
  public static final String GREATER_THAN = "op:greater-than";
  public static final String LESS_EQUALS = "op:less-than-equals";
  public static final String GREATER_EQUALS = "op:greater-than-equals";

  public static final String[] UNARY_OPS = {
    UNARY_PLUS, UNARY_MINUS
  };

  public static final String[] BINARY_OPS = {
    ADD, SUBTRACT, MULTIPLY, DIVIDE, MOD, INTEGER_DIVIDE, EQUALS,  TO, AND,
    OR, NOT_EQUALS, LESS_THAN, GREATER_THAN, LESS_EQUALS, GREATER_EQUALS
  };
  
  private static boolean func_name_set_built = false;
  private static Set<String> func_name_set = new HashSet<String>();
  
  private static void buildFuncNameSet() {
    for (String s : UNARY_OPS)
      func_name_set.add(s);
    for (String s: BINARY_OPS)
      func_name_set.add(s);
    
    func_name_set_built = true;
  }
  
  public static final boolean contains(String name) {
    if (!func_name_set_built)
      buildFuncNameSet();
    
    return func_name_set.contains(name);
  }
}
