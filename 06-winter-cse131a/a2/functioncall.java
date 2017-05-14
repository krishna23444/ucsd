public class FunctionCall extends ExpressionNode implements Visitable {
  private String name;

  public FunctionCall(String name) {
    this.name = name;
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String getName() {
    return name;
  }

  public String toString() {
    return "FunctionCall: name = " + name + "\n";
  }
}
