import java.util.Arrays;
import onyx.xml.OnyxTextNode;
import onyx.xml.OnyxNode;

public class FunctionPrototype implements Comparable<FunctionPrototype> {
  private String name;
  private String return_type;
  private ParamType[] params;
  
  // create function prototype where argument names don't matter
  public FunctionPrototype(String name, String ret_type, String ... types) {
    this.name = name;
    this.return_type = ret_type;
    params = new ParamType[types.length];
    for (int i = 0; i < types.length; ++i)
      params[i] = new ParamType("$arg" + i, types[i]);
  }
  
  public FunctionPrototype(String name, String ret_type, ParamType ... params) {
    this.name = name;
    this.return_type = ret_type;
    this.params = params;
  }
  
  // No args function
  public FunctionPrototype(String name, String ret_type) {
    this(name, ret_type, new ParamType[0]);
  }

  public FunctionPrototype(String name, ParamType ... params) {
    this(name, Types.ANY, params);
  }

  public String getName() {
    return name;
  }
  
  public String getReturnType() {
    return return_type;
  }

  public int compareTo(FunctionPrototype fp) {
    return toPrefixErrorString().compareTo(fp.toPrefixErrorString());
//    if (name.length() < fp.name.length())
//      return -1;
//    else if (name.length() == fp.name.length())
//      return name.compareTo(fp.name);
//    else
//      return 1;
  }

  public ParamType getParam(int i) {
    return params[i];
  }

  public boolean isPrefixOf(FunctionPrototype fp) {
    //System.err.println("name equals: " + fp.name.equals(name));
    //System.err.println("instances: " + equalParamInstances(fp));
    return fp.name.equals(name) && equalParamInstances(fp);
  }
  
  public boolean isNamePrefixOf(FunctionPrototype fp) {
    return fp.name.startsWith(name) && equalNumParams(fp);
  }

  private boolean equalName(FunctionPrototype fp) {
    return name.equals(fp.name);
  }

  private boolean equalReturnType(FunctionPrototype fp) {
    return return_type.equals(fp.return_type);
  }
  
  private boolean equalNumParams(FunctionPrototype fp) {
    return params.length == fp.params.length;
  }

  private boolean equalParamInstances(FunctionPrototype fp) {
    if (!equalNumParams(fp))
      return false;
    
    for (int i = 0; i < params.length; ++i) {
      if (!params[i].isInstanceOf(fp.params[i])) {
        System.err.println(params[i] + "not instance of " + fp.params[i]);
        return false;
      }
    }
    return true;
  }

  private boolean equalParamTypes(FunctionPrototype fp) {
    if (!equalNumParams(fp))
      return false;
    
    return Arrays.equals(params, fp.params);
  }

  public boolean equals(Object o) {
    if (!(o instanceof FunctionPrototype))
      return false;

    FunctionPrototype fp = (FunctionPrototype)o;
    // return_type is not used in equals comparison
    return equalName(fp) && equalParamTypes(fp);
    //return equalName(fp) && equalParamInstances(fp);
  }

  private boolean exactlyEqualParams(FunctionPrototype fp) {
    if (!equalNumParams(fp))
      return false;
    
    for (int i = 0; i < params.length; ++i)
      if (!params[i].exactlyEquals(fp.params[i]))
        return false;
    
    return true;
  }

  public boolean exactlyMatches(FunctionPrototype fp) {
    return equalName(fp) && equalReturnType(fp) && exactlyEqualParams(fp);
  }
  
  /**
   * Create printable onyx form of this prototype. Used by error messages in
   * things like dynamic errors and finding possible function matches.
   */
  public String toCandidateErrorString() {
    String s = "";
    
    // apparently we're not supposed to print AnyType as a return value
    if (!return_type.equals(Types.ANY))
      s += return_type + " ";
    
    s += name + "(";
    for (int i = 0; i < params.length; ++i) {
      s += params[i].getType();
      if (i < params.length - 1)
        s += ",";
    }
    return s + ")";
  }
  
  /**
   * Create printable onyx form of this prototype. Used by error messages in
   * things like dynamic errors and finding possible function matches.
   */
  public String toPrefixErrorString() {
    String s = "";

    s += return_type + " ";
    
    s += name + "(";
    for (int i = 0; i < params.length; ++i) {
      s += params[i].getType();
      if (i < params.length - 1)
        s += ",";
    }
    return s + ")";
  }

  public OnyxNode toOnyxNode(String tag_name) {
    return new OnyxTextNode(tag_name, toPrefixErrorString());
  }
  
  public OnyxNode toPossibleMatchOnyxNode() {
    return toOnyxNode("PossibleMatch");
  }
  
  public static final int CANNOT_PROMOTE = -1;
  /**
   * Compute number of promotions required to turn this function prototype into
   * the passed in one. If it is not possible, -1 is returned (should we throw?)
   */
  public int numPromotionsTo(FunctionPrototype func) {
    if (params.length != func.params.length)
      return CANNOT_PROMOTE;

    int num_promotions = 0;
    int current_promotion;
    for (int i = 0; i < params.length; ++i) {
      current_promotion = params[i].numPromotionTo(func.params[i]);
      
      if (current_promotion == ParamType.CANNOT_PROMOTE)
        return CANNOT_PROMOTE;
      
      num_promotions += current_promotion;
    }
    
    return num_promotions;
  }

  public AnyType[] promoteTypesToMatch(AnyType[] args) {
    AnyType[] new_args = new AnyType[args.length];
    
    for (int i = 0; i < params.length; ++i) {
      new_args[i] = params[i].promoteTo(args[i]);
      if (new_args[i] == null)
        return null;
    }
    
    return new_args;
  }

  public int hashCode() {
    // return_type is not used in hash code calculation
    //int code = name.hashCode();
    //for (int i = 0; i < params.length; i++) {
    //  code += params[i].hashCode() % (i + 1);
    //}
    //return code;
    return name.hashCode() + Arrays.hashCode(params);
  }
  
  public String toString() {
    return toCandidateErrorString();
  }
}
