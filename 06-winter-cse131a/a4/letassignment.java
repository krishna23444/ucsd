/**
 * Has one attribute, the variable name, stoed in the parent class.
 * Has one element, the value this variable should be set to.
 */
public class LetAssignment extends VarNameNode implements Visitable {
  public LetAssignment(String varName) {
    super(varName);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public boolean equals(Object o) {
    if (!(o instanceof LetAssignment))
      return false;
    
    return super.equals(o);
  }

  public int hashCode() {
    return super.hashCode();
  }
}
