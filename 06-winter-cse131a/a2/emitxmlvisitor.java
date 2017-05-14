import java.util.Stack;

import onyx.xml.*;

public class EmitXMLVisitor implements Visitor {
  private Stack<OnyxElement> st = new Stack<OnyxElement>();

  public OnyxNode getRoot() {
    return st.pop();
  }

  public void visit(ExpressionNode e) {
    visit_children(e);
  }

  public void visit(Query q) {
    OnyxElement root = new OnyxElement("onyx.ast.Query");
    set_parent_node(root);
    visit_children(q);
  }

  public void visit(FunctionDeclaration fd) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("funcName", fd.getName());
    attr.add("datatype", fd.getDatatype());

    OnyxElement node = new OnyxElement("onyx.ast.FunctionDeclaration", attr);

    build_child_nodes(fd, node);
  }
  
  public void visit(VariableDeclaration vd) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("varName", vd.getVarName());
    attr.add("datatype", vd.getDatatype());

    OnyxElement node = new OnyxElement("onyx.ast.VariableDeclaration", attr);

    build_child_nodes(vd, node);
  }
  
  public void visit(ArgumentDeclaration ad) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("varName", ad.getVarName());
    attr.add("datatype", ad.getDatatype());

    OnyxElement node = new OnyxElement("onyx.ast.ArgumentDeclaration", attr);

    parent_node().addChild(node);
  }
  
  public void visit(ForAssignment fa) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("varName", fa.getVarName());

    OnyxElement node = new OnyxElement("onyx.ast.ForAssignment", attr);

    build_child_nodes(fa, node);
  }
  
  public void visit(LetAssignment la) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("varName", la.getVarName());

    OnyxElement node = new OnyxElement("onyx.ast.LetAssignment", attr);

    build_child_nodes(la, node);
  }

  public void visit(Where w) {
    OnyxElement node = new OnyxElement("onyx.ast.Where");
    build_child_nodes(w, node);
  }

  public void visit(Return r) {
    OnyxElement node = new OnyxElement("onyx.ast.Return");
    build_child_nodes(r, node);
  }

  public void visit(Variable v) {
    OnyxTextNode node = new OnyxTextNode("onyx.ast.Variable", v.getName());
    parent_node().addChild(node);
  }
  
  public void visit(Constant c) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("datatype", c.getDatatype());
    OnyxTextNode node = 
        new OnyxTextNode("onyx.ast.Constant", attr, c.getValue());
    parent_node().addChild(node); 
  }
  
  public void visit(FunctionCall fc) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("name", fc.getName());
    OnyxElement node = new OnyxElement("onyx.ast.FunctionCall", attr);
    build_child_nodes(fc, node);
  }
  
  public void visit(Operator o) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("name", o.getName());
    OnyxElement node = new OnyxElement("onyx.ast.Operator", attr);
    build_child_nodes(o, node);
  }
  
  public void visit(Flwr f) {
    OnyxElement node = new OnyxElement("onyx.ast.Flwr");
    build_child_nodes(f, node);
  }
  
  public void visit(IfThenElseExpr iftee) {
    OnyxElement node = new OnyxElement("onyx.ast.IfThenElseExpr");
    build_child_nodes(iftee, node);
  }
  
  public void visit(ExprList el) {
    OnyxElement node = new OnyxElement("onyx.ast.ExprList");
    build_child_nodes(el, node);
  }

  public void visit(NonExpressionNode n) {
    visit_children(n);
  }


  private void set_parent_node(OnyxElement node) {
    st.push(node);
  }

  private void restore_parent_node() {
    st.pop();
  }

  private OnyxElement parent_node() {
    return st.peek();
  }

  private void visit_children(ASTNode n) {
    for (int i = 0; i < n.numChildren(); ++i)
      n.getChild(i).accept(this);
  }

  private void build_child_nodes(ASTNode ast_node, OnyxElement onyx_node) {
    set_parent_node(onyx_node);
    visit_children(ast_node);
    restore_parent_node();
    parent_node().addChild(onyx_node);
  }

  private void debug_children(ASTNode node) {
    System.err.println(node);
    for (int i = 0; i < node.numChildren(); ++i)
      System.err.println("--> " + node.getChild(i));
  }
}
