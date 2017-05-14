/**
 * Parent of all nodes that can contain, or be contained, in ExpressionNodes.
 */
public class NonExpressionNode extends ASTNode {
  public NonExpressionNode(ASTNode ... nodes) {
    addChildren(nodes);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public boolean equals(Object o) {
    if (!(o instanceof NonExpressionNode))
      return false;
    
    return super.equals(o);
  }

  public int hashCode() {
    return super.hashCode();
  }
}
