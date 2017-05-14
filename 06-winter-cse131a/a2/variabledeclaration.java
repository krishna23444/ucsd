public class VariableDeclaration extends VarNameNode implements Visitable {
  private String datatype;

  public VariableDeclaration(String varName, String datatype) {
    super(varName);
    this.datatype = datatype;
  }
  
  public VariableDeclaration(String varName) {
    this(varName, "onyx.types.AnyType");
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String getDatatype() {
    return datatype;
  }

  public String toString() {
    return "VariableDeclaration\n";
  }
}
