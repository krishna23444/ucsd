public class ExprList extends ExpressionNode implements Visitable {
  public ExprList() {
    super();
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String toString() {
    return "ExprList\n";
  }
}
