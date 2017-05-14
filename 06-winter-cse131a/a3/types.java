import java.util.HashMap;
import java.util.Map;

public class Types  {
  public static final String ANY        = "onyx.types.AnyType";
  public static final String XML        = "onyx.types.XmlType";
  public static final String NODE       = "onyx.types.Node";
  public static final String TNODE      = "onyx.types.TNode";
  public static final String ENODE      = "onyx.types.ENode";
  public static final String ATTR_ENV   = "onyx.types.AttrEnv";  
  public static final String ANY_SIMPLE = "onyx.types.AnySimpleType";
  public static final String NUMBER     = "onyx.types.Number";  
  public static final String INTEGER    = "onyx.types.Integer";
  public static final String DECIMAL    = "onyx.types.Decimal";
  public static final String BOOLEAN    = "onyx.types.Boolean";
  public static final String STRING     = "onyx.types.String";
  public static final String SEQUENCE   = "onyx.types.Sequence";
  
  public static boolean isNodeType(String type) {
    return type.equals(ENODE) || type.equals(TNODE) || type.equals(NODE);
  }

  public static boolean isXmlType(String type) {
    return isNodeType(type) || type.equals(ATTR_ENV) || type.equals(XML);
  }

  public static boolean isNumberType(String type) {
    return type.equals(DECIMAL) || type.equals(INTEGER) || type.equals(NUMBER);
  }

  public static boolean isAnySimpleType(String type) {
    return isNumberType(type) || type.equals(BOOLEAN) || type.equals(STRING)
           || type.equals(ANY_SIMPLE);
  }
  
  private static boolean class_string_map_built = false;
  private static boolean string_class_map_built = false;
  private static Map<Class, String> class_str_map;
  private static Map<String, Class> str_class_map;
  
  private static void buildMap() {
    class_str_map = new HashMap<Class, String>();
    
    class_str_map.put(AnyType.class, ANY);
    class_str_map.put(AnySimpleType.class, ANY_SIMPLE);
    class_str_map.put(NumberType.class, NUMBER);
    class_str_map.put(IntegerType.class, INTEGER);
    class_str_map.put(DecimalType.class, DECIMAL);
    class_str_map.put(BooleanType.class, BOOLEAN);
    class_str_map.put(StringType.class, STRING);
    class_str_map.put(SequenceType.class, SEQUENCE);
    class_str_map.put(XmlType.class, XML);
    class_str_map.put(NodeType.class, NODE);
    class_str_map.put(TNodeType.class, TNODE);
    class_str_map.put(ENodeType.class, ENODE);
    class_str_map.put(AttrEnvType.class, ATTR_ENV);
    
    class_string_map_built = true;
    
    str_class_map = new HashMap<String, Class>();
    
    str_class_map.put(ANY, AnyType.class);
    str_class_map.put(ANY_SIMPLE, AnySimpleType.class);
    str_class_map.put(NUMBER, NumberType.class);
    str_class_map.put(INTEGER, IntegerType.class);
    str_class_map.put(DECIMAL, DecimalType.class);
    str_class_map.put(BOOLEAN, BooleanType.class);
    str_class_map.put(STRING, StringType.class);
    str_class_map.put(SEQUENCE, SequenceType.class);
    str_class_map.put(XML, XmlType.class);
    str_class_map.put(NODE, NodeType.class);
    str_class_map.put(TNODE, TNodeType.class);
    str_class_map.put(ENODE, ENodeType.class);
    str_class_map.put(ATTR_ENV, AttrEnvType.class);
    
    string_class_map_built = true;
  }
  
  public static final String nameOf(Object obj) {
    if (!class_string_map_built)
      buildMap();
    return class_str_map.get(obj.getClass());
  }
  
  public static final Class classOf(String str) throws ClassNotFoundException {
    if (!string_class_map_built)
      buildMap();
    if (str_class_map.get(str) == null)
      throw new ClassNotFoundException();
    return str_class_map.get(str);
  }
}
