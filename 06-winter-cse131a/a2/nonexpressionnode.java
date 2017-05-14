public class NonExpressionNode extends ASTNode {
  public void accept(Visitor visitor) {
    visitor.visit(this);
  }
}
