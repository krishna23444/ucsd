/**
 * Parent of all nodes that are expressions, in particular: Variable, Constant,
 * FunctionCall, Operator, Flwr, IfThenElseExpr, and ExprList.
 */
public class ExpressionNode extends ASTNode {
  public ExpressionNode(ASTNode ... nodes) {
    addChildren(nodes);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }
  
  public boolean equals(Object o) {
    if (!(o instanceof ExpressionNode))
      return false;
    
    return super.equals(o);
  }
  
  public int hashCode() {
    return super.hashCode();
  }
  
  public String toString() {
    return super.toString();
  }
}
