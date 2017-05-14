/**
 * Has one attribute, the name of the variable.
 */
public class Variable extends ExpressionNode implements Visitable {
  private String name;

  public Variable(String name) {
    this.name = name;
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String getName() {
    return name;
  }

  public boolean equals(Object o) {
    if (!(o instanceof Variable))
      return false;
    
    return super.equals(o) && name.equals(((Variable)o).name);
  }
  
  public int hashCode() {
    return super.hashCode() + name.hashCode();
  }

  public String toString() {
    return name;
  }
}
