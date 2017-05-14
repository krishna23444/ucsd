/**
 * Has one element, an ExpressionNode whose value will be returned.
 */
public class Return extends ASTNode implements Visitable {
  public Return(ASTNode var) {
    super();
    addChild(var);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public boolean equals(Object o) {
    if (!(o instanceof Return))
      return false;

    return super.equals(o);
  }
  
  public int hashCode() {
    return super.hashCode();
  }
}
