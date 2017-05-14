import java.util.ArrayList;
import java.util.List;

/**
 * Superclass of all abstract syntax tree node. Stores a list of children, which
 * are all ASTNodes.
 */
public abstract class ASTNode implements Visitable {
  protected ArrayList<ASTNode> children;
  private int line;
  private int column;

  public ASTNode() {
    children = new ArrayList<ASTNode>();
  }

  public abstract void accept(Visitor visitor);

  public void addChild(ASTNode node) {
    addChildren(node);
  }
  
  public void addChildren(ASTNode ... nodes) {
    for (ASTNode node : nodes)
      if (node != null)
        children.add(node);
  }
  
  public void addChildren(List<ASTNode> nodes) {
    children.addAll(nodes);
  }

  public int getLine() {
    return line;
  }
  
  public int getColumn() {
    return column;
  }
  
  public void setLineAndColumn(int line, int column) {
    this.line = line;
    this.column = column;
  }

  public boolean hasChildren() {
    return numChildren() > 0;
  }

  public int numChildren() {
    return children.size();
  }

  public List<ASTNode> getChildren() {
    return children;
  }

  public ASTNode getChild(int i) {
    return children.get(i);
  }

  public ASTNode getFirstChild() {
    return getChild(0);
  }
  
  public ASTNode getSecondChild() {
    return getChild(1);
  }

  public ASTNode getLastChild() {
    return getChild(numChildren() - 1);
  }

  /**
   * ASTNodes are equal if they have the same children.
   */
  public boolean equals(Object o) {
    if (!(o instanceof ASTNode))
      return false;
    
    return children.equals(((ASTNode)o).children);
  }

  public int hashCode() {
    return children.hashCode();
  }
  
  public String toString() {
    return children.toString();
  }
}
