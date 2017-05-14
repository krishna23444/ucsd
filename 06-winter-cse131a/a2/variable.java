public class Variable extends ExpressionNode implements Visitable {
  private String name;

  public Variable(String name) {
    this.name = name;
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String getName() {
    return name;
  }

  public String toString() {
    return "Variable name=" + name + "\n";
  }
}
