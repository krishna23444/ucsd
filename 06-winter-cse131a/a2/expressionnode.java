public class ExpressionNode extends ASTNode {
  public ExpressionNode() {
    super();
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }
}
