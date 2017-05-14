public class IfThenElseExpr extends ExpressionNode implements Visitable {
  public IfThenElseExpr(ASTNode if_node, ASTNode then_node, ASTNode else_node) {
    super();
    addChild(if_node);
    addChild(then_node);
    addChild(else_node);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String toString() {
    return "IfThenElseExpr\n";
  }
}
