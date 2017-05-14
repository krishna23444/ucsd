/**
 * Hash one attribute: a name, stored in the parent class.
 * Has one element: an ExpressionNode that generates a sequence of values.
 */
public class ForAssignment extends VarNameNode implements Visitable {
  public ForAssignment(String varName) {
    super(varName);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public boolean equals(Object o) {
    if (!(o instanceof ForAssignment))
      return false;
    
    return super.equals(o);
  }

  public int hashCode() {
    return super.hashCode();
  }
}
