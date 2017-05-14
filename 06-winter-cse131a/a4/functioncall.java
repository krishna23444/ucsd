/**
 * Has one attribute: the name of the function.
 * Has zero or more elements which are the arguments to the function.
 */
public class FunctionCall extends ExpressionNode implements Visitable {
  private String name;

  public FunctionCall(String name) {
    this.name = name;
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String getName() {
    return name;
  }

  public boolean equals(Object o) {
    if (!(o instanceof FunctionCall))
      return false;
    
    return super.equals(o) && name.equals(((FunctionCall)o).name);
  }
  
  public int hashCode() {
    return super.hashCode() + name.hashCode();
  }

  public String toString() {
    return name;
  }
}
