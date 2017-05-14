/**
 * Has 2 attributes: varName and datatype, varName is stored in parent class.
 */
public class ArgumentDeclaration extends VarNameNode implements Visitable {
  private String datatype;

  public ArgumentDeclaration(String varName, String datatype) {
    super(varName);
    this.datatype = datatype;
  }

  public ArgumentDeclaration(String varName) {
    this(varName, Types.ANY);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String getDatatype() {
    return datatype;
  }

  public boolean equals(Object o) {
    if (!(o instanceof ArgumentDeclaration))
      return false;
    
    ArgumentDeclaration arg_decl = (ArgumentDeclaration)o;
    return super.equals(o) && datatype.equals(arg_decl.datatype);
  }

  public int hashCode() {
    return datatype.hashCode() + super.hashCode();
  }
  
  public String toString() {
    return datatype + " -- " + getChildren();
  }
}
