public class Funcs {
  public static final String CONCAT = "concat";
  public static final String DOCUMENT = "document";
  public static final String ENODE = "enode";
  public static final String TNODE = "tnode";
  public static final String ADD_CHILD_NODE = "addChildNode";
  public static final String CHILDREN = "children";
  public static final String TAGNAME = "tagname";
  public static final String SET_ATTR_ENV = "setAttrEnv";
  public static final String GET_ATTR_ENV = "getAttrEnv";
  public static final String ATTR_ENV = "attrenv";
  public static final String ADD_ATTR = "addAttribute";
  public static final String GET_ATTR_KEYS = "getAttributeKeys";
  public static final String GET_ATTR_VALUE = "getAttributeValue";
  public static final String IS_NODE = "isNode";
  public static final String IS_ENODE = "isENode";
  public static final String IS_TNODE = "isTNode";
  public static final String STRING = "string";
  public static final String DECIMAL = "decimal";
  public static final String INTEGER = "integer";
  public static final String LENGTH = "length";
  public static final String FIRST = "first";
  public static final String TAIL = "tail";

  public static final String [] NO_ARG_FUNC = {
    ATTR_ENV
  };

  public static final String [] UNARY_FUNC = {
    DOCUMENT, CHILDREN, TAGNAME, GET_ATTR_ENV,
    GET_ATTR_KEYS, IS_NODE, IS_ENODE, IS_TNODE, STRING, DECIMAL,
    INTEGER, LENGTH, FIRST, TAIL
  };
  
  public static final String [] BINARY_FUNC = {
    CONCAT, ENODE, ADD_CHILD_NODE, SET_ATTR_ENV, GET_ATTR_VALUE
  };

  public static final String [] TERTIARY_FUNC = {
    TNODE, ADD_ATTR
  };
}
