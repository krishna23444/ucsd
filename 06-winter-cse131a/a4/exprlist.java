/**
 * Has any number of children, which are all ExpressionNodes that are not other
 * ExprLists. When an ExprList is added, the whole thing is flattened.
 */
public class ExprList extends ExpressionNode implements Visitable {
  public ExprList(ASTNode ... children) {
    addChildren(children);
  }

  /**
   * override ASTNode's addChild to handle flattening of multiple ExprLists.
   */
  public void addChild(ASTNode node) {
    if (node instanceof ExprList)
      addChildren(node.getChildren());
    else
      super.addChild(node);
  }
  
  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public boolean equals(Object o) {
    if (!(o instanceof ExprList))
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
