public class ArgumentDeclaration extends VarNameNode implements Visitable {
  private String datatype;

  public ArgumentDeclaration(String varName, String datatype) {
    super(varName);
    this.datatype = datatype;
  }

  public ArgumentDeclaration(String varName) {
    this(varName, "onyx.types.AnyType");
  }

  public String getDatatype() {
    return datatype;
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }
}
