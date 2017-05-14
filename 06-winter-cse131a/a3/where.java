public class Where extends ASTNode implements Visitable {
  public Where(ASTNode ... children) {
    for (ASTNode node : children)
      addChild(node);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public boolean equals(Object o) {
    if (!(o instanceof Where))
      return false;
    
    return super.equals(o);
  }

  public int hashCode() {
    return super.hashCode();
  }
}
