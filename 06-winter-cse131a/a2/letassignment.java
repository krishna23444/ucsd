public class LetAssignment extends VarNameNode implements Visitable {
  public LetAssignment(String varName) {
    super(varName);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String toString() {
    return "LetAssignment\n";
  }
}
