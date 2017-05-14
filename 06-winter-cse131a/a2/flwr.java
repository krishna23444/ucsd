public class Flwr extends ExpressionNode implements Visitable {
  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String toString() {
    return "FLWR\n";
  }
}
