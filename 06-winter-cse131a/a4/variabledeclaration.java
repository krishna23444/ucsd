/**
 * Has one child, an ExprList, which holds the value of this variable.
 */
public class VariableDeclaration extends VarNameNode implements Visitable {
  private String datatype;

  public VariableDeclaration(String varName, String datatype) {
    super(varName);
    this.datatype = datatype;
  }
  
  public VariableDeclaration(String varName) {
    this(varName, Types.ANY);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String getDatatype() {
    return datatype;
  }

  public boolean equals(Object o) {
    if (!(o instanceof VariableDeclaration))
      return false;

    VariableDeclaration var_dec = (VariableDeclaration)o;
    return super.equals(o) && datatype.equals(var_dec.datatype);
  }

  public int hashCode() {
    return super.hashCode() + datatype.hashCode();
  }

  public String toString() {
    return datatype.toString();
  }
}
