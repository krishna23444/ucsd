public class ForAssignment extends VarNameNode implements Visitable {
  public ForAssignment(String varName) {
    super(varName);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String toString() {
    return "ForAssignment\n";
  }
}
