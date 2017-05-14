/**
 * An abstract syntax tree node with a name.
 */
public class NamedNode extends ExpressionNode implements Visitable {
  private String name;

  public NamedNode(String name) {
    this.name = name;
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String getName() {
    return name;
  }

  public boolean equals(Object o) {
    if (!(o instanceof NamedNode))
      return false;
    
    return super.equals(o) && name.equals(((NamedNode)o).name);
  }
  
  public int hashCode() {
    return super.hashCode() + name.hashCode();
  }
  
  public String toString() {
    return name + " -- " + getChildren();
  }
}
