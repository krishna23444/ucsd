public class NamedNode extends ExpressionNode implements Visitable {
  private String name;

  public NamedNode(String name) {
    this.name = name;
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String getName() {
    return name;
  }

  public String toString() {
    return "Variable name = " + name + "\n";
  }
}
