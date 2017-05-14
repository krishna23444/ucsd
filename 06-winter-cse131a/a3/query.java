/**
 * Has zer or more FunctionDeclaration or VariableDeclaration elements, followed
 * by an ExprList representing the query body.
 */
public class Query extends ASTNode implements Visitable {
  public Query(ASTNode prolog, ASTNode body) {
    super();
    addChildren(prolog, body);
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public boolean equals(Object o) {
    if (!(o instanceof Query))
      return false;

    return super.equals(o);
  }

  public int hashCode() {
    return super.hashCode();
  }
}
