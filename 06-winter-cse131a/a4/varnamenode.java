/**
 * Parent of all NonExpressionNodes that have a name.
 */
public class VarNameNode extends NonExpressionNode implements Visitable {
  private String varName;

  public VarNameNode(String varName) {
    this.varName = varName;
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }
  
  public boolean equals(Object o) {
    if (!(o instanceof VarNameNode))
      return false;
    
    return super.equals(o) && varName.equals(((VarNameNode)o).varName);
  }
  
  public int hashCode() {
    return super.hashCode() + varName.hashCode();
  }

  public String getName() {
    return varName;
  }

  public String toString() {
    return varName;
  }
}
