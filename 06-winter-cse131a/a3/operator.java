/**
 * Has one attribute, the name of the operator, stored in the parent class.
 * Hash zero or more ExpressionNodes representing the parameters to use.
 */
public class Operator extends NamedNode implements Visitable {
  public Operator(String name, ExpressionNode ... nodes) {
    super(name);
    addChildren((ASTNode [])nodes);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public ASTNode getArg(int num) {
    return getChild(num);
  }

  public boolean equals(Object o) {
    if (!(o instanceof Operator))
      return false;

    return super.equals(o);
  }
  
  public int hashCode() {
    return super.hashCode();
  }

  public String toString() {
    return getName() + " -- " + getChildren();
  }
}
