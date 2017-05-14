public class ParamType extends AnyType {
  private String name;
  private String type;
  
  
  public ParamType(ArgumentDeclaration a) {
    this(a.getName(), a.getDatatype());
  }
  
  public ParamType(String type) {
    this("no_name", type);
  }
  
  public ParamType(String name, String type) {
    this.name = name;
    this.type = type;
  }
  
  public boolean equals(Object o) {
    if (!(o instanceof ParamType))
      return false;

    // name doesn't matter for equality checking
    return typeEquals((ParamType)o);
  }
  
  public boolean exactlyEquals(ParamType pt) {
    return nameEquals(pt) && typeEquals(pt);
  }
  
  private static boolean isAnyType(ParamType pt) {
    return pt.type.equals(Types.ANY);
  }
  
  private static boolean isSequenceType(ParamType pt) { 
    return pt.type.equals(Types.SEQUENCE);
  }
  
  public String getName() {
    return name;
  }
  
  public String getType() {
    return type;
  }
  
  public int hashCode() {
    // name not used for hashCode
    return type.hashCode();
  }

  private boolean nameEquals(ParamType pt) {
    return name.equals(pt.name);
  }

  public static final int CANNOT_PROMOTE = -1;
  /**
   * calculate number of promotions required to promote this object's type to
   * that of the ParamType passed in. If promotion is not possible, an error
   * value, specifically -1, is returned. In all cases this function should
   * return either 0 for no promotion required or promotion is a free
   * conversion, 1 for a promotion is required, or -1 for promotion impossible.
   */
  public int numPromotionTo(ParamType pt) {
    // same type -> no promotion necessary
    if (typeEquals(pt))
      return 0;
    
    // destination type is AnyType -> free promotion
    if (pt.type.equals(Types.ANY))
      return 0;
    
    //  integer, decimal, boolean, or string to AnySimpleType -> free promotion
    if (Types.isAnySimpleType(type) && pt.type.equals(Types.ANY_SIMPLE))
      return 0;
    
    // integer or decimal to NumberType -> free promotion
    if (Types.isNumberType(type) && pt.type.equals(Types.NUMBER))
      return 0;
    
    // enode, tnode, node or attr_env to xml -> free promotion
    if (Types.isXmlType(type) && pt.type.equals(Types.XML))
      return 0;
    
    // enode or tnode to node -> free promotion
    if (Types.isNodeType(type) && pt.type.equals(Types.NODE))
      return 0;

    // integer to decimal
    if (type.equals(Types.INTEGER) && pt.type.equals(Types.DECIMAL))
      return 1;

    // any type to a single sequence
    if (pt.type.equals(Types.SEQUENCE))
      return 1;

    return CANNOT_PROMOTE;
  }

  /**
   * Promote argument to an object of this type.
   * A promotion is either converting an integer to a decimal, or converting
   * any single item to a sequence containing that one item. Also, coercing
   * an element of any onyx type to an AnyType is considered a free promotion.
   */
  public AnyType promoteTo(AnyType p) {
    // same type, no conversion needed
    if (type.equals(Types.nameOf(p)))
      return p;
    
    // anything -> AnyType, no conversion needed
    if (type.equals(Types.ANY))
      return p;

    // integer, decimal, boolean, or string -> AnySimpleType, no conversion
    if (type.equals(Types.ANY_SIMPLE) && p instanceof AnySimpleType)
      return p;
    
    // integer or decimal -> NumberType, no conversion needed
    if (type.equals(Types.NUMBER) && p instanceof NumberType)
      return p;
    
    // enode, tnode or attr_env -> XmlType, no conversion needed
    if (type.equals(Types.XML) && p instanceof XmlType)
      return p;

    // enode or tnode -> NodeType, no conversion needed
    if (type.equals(Types.NODE) && p instanceof NodeType)
      return p;

    // IntegerType -> DecimalType
    if (p.getClass().equals(IntegerType.class) && type.equals(Types.DECIMAL))
      return ((IntegerType)p).toDecimalType();
    
    // Anything -> Single Sequence
    if (type.equals(Types.SEQUENCE))
      return new SequenceType(p);
    
    return null;
  }
  
  public String stringOf() {
    throw new RuntimeException("why did you call stringOf on ParamType?");
  }
  
  public String toString() {
    return name + ":" + type;
  }
  
  private boolean typeEquals(ParamType pt) {
    return type.equals(pt.type);
  }
  
  private boolean isDecendant(ParamType pt) {
    if ((type.equals(Types.ENODE) || type.equals(Types.TNODE)
        || type.equals(Types.NODE)) && pt.type.equals(Types.NODE))
      return true;
    
    if ((type.equals(Types.DECIMAL) || type.equals(Types.INTEGER)
        || type.equals(Types.NUMBER)) && pt.type.equals(Types.NUMBER))
        return true;
    
    if ((type.equals(Types.DECIMAL) || type.equals(Types.INTEGER)
        || type.equals(Types.NUMBER) || type.equals(Types.BOOLEAN)
        || type.equals(Types.STRING) || type.equals(Types.ANY_SIMPLE))
        && pt.type.equals(Types.ANY_SIMPLE))
      return true;

    return false;
  }
  
  public boolean isInstanceOf(ParamType pt) {
      return (numPromotionTo(pt) != CANNOT_PROMOTE);
    //return typeEquals(pt) || isAnyType(pt) || isDecendant(pt);
  }
}
