import java.util.ArrayList;

public abstract class ASTNode implements Visitable {
  protected ArrayList<ASTNode> children;

  public abstract void accept(Visitor visitor);

  public ASTNode() {
    children = new ArrayList<ASTNode>();
  }

  public void addChild(ASTNode child) {
    if (child != null)
      children.add(child);
  }

  public boolean hasChildren() {
    return children.size() > 0;
  }

  public int numChildren() {
    return children.size();
  }

  public ASTNode getChild(int i) {
    return children.get(i);
  }
}
