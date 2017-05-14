public class VarNameNode extends NonExpressionNode implements Visitable {
  private String varName;

  public VarNameNode(String varName) {
    this.varName = varName;
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String getVarName() {
    return varName;
  }

  public String toString() {
    return "Variable name = " + varName + "\n";
  }
}
