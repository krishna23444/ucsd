public class Operator extends NamedNode implements Visitable {
  // unary operator
  public Operator(String name, ExpressionNode child) {
    super(name);
    addChild(child);
  }

  // binary operator
  public Operator(String name, ExpressionNode left, ExpressionNode right) {
    super(name);
    addChild(left);
    addChild(right);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String toString() {
    return "Operator: name = " + getName() + "\n";
  }
}
