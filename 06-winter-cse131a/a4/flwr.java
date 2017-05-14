/**
 * Has at least one element: Return. Can have an amount of mixed ForAssignment
 * and LetAssignment, along with an optional Where, which come before the Return
 * in the child list.
 */
public class Flwr extends ExpressionNode implements Visitable {
  public Flwr(ASTNode ... nodes) {
    addChildren(nodes);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public boolean equals(Object o) {
    if (!(o instanceof Flwr))
      return false;
    
    return super.equals(o);
  }

  public int hashCode() {
    return super.hashCode();
  }
  
  public void makeChildNodes() {
    ASTNode current_child, next_child;
    
    // Iterate through children
    for (int i = 0; i < numChildren() - 1; i++) {
      current_child = getChild(i);
      
      // If this is a non-expression node, attach each child of the 
      // non-expression node to its next sibling
      if (current_child.getClass() == NonExpressionNode.class) {
        for (int j = 0; j < current_child.numChildren() - 1; j++)
           current_child.getChild(j).addChild(current_child.getChild(j + 1));
        
        current_child = current_child.getLastChild();
      }
      
      next_child = getChild(i + 1);
      if (next_child.getClass() ==  NonExpressionNode.class)
        next_child = next_child.getFirstChild();
      
      current_child.addChild(next_child);
    }
  }
}
