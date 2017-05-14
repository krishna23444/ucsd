/**
 * Has two attributes: the name of the function and the datatype it returns.
 * Has zero or more ArgumentDeclaration elements for the arguments.
 * Has one ExpressionNode element for the function body.
 */
public class FunctionDeclaration extends ASTNode implements Visitable {
  private String funcName;
  private String datatype;

  // no return type, no args
  public FunctionDeclaration(String funcName, ExpressionNode code) {
    this(funcName, Types.ANY, code);
  }

  // no return type
  public FunctionDeclaration(String name, ASTNode args, ExpressionNode code) {
    this(name, args, Types.ANY, code);
  }

  // no args
  public FunctionDeclaration(String funcName, String datatype,
                             ExpressionNode code) {
    super();
    this.funcName = funcName;
    this.datatype = datatype;
    addChild(code);
  }

  // everything
  public FunctionDeclaration(String funcName, ASTNode args, String datatype,
                             ExpressionNode code) {
    super();
    this.funcName = funcName;
    this.datatype = datatype;
    addChildren(args.getChildren());
    addChild(code);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String getName() {
    return funcName;
  }

  public String getDatatype() {
    return datatype;
  }
  
  public ASTNode getBody() {
    return getLastChild();
  }

  public boolean takesParams() {
    return numChildren() > 1;
  }

  public boolean equals(Object o) {
    if (!(o instanceof FunctionDeclaration))
      return false;
    
    FunctionDeclaration func_decl = (FunctionDeclaration)o;
    return super.equals(o) && funcName.equals(func_decl.funcName)
           && datatype.equals(func_decl.datatype);
  }

  public int hashCode() {
    return super.hashCode() + funcName.hashCode() + datatype.hashCode();
  }

  public String toString() {
    String s = funcName + "(";
    
    for (int i = 0; i < numChildren() - 1; ++i)
      s += getChild(i).toString();
    
    return s + ") -> \"" + datatype + "\" {" + getBody() + "}";
  }
}
