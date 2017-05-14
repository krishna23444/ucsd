import java.util.Stack;
import onyx.xml.OnyxAttributeEnvironment;
import onyx.xml.OnyxElement;
import onyx.xml.OnyxNode;
import onyx.xml.OnyxTextNode;

public class EmitXMLVisitor implements Visitor {
  private Stack<OnyxElement> st;

  public EmitXMLVisitor() {
    st = new Stack<OnyxElement>();
  }

  public OnyxNode getRoot() {
    return st.pop();
  }

  public void visit(ExpressionNode e) {
    visitChildren(e);
  }

  public void visit(Query q) {
    OnyxElement root = new OnyxElement("onyx.ast.Query");
    setParentNode(root);
    visitChildren(q);
  }

  public void visit(FunctionDeclaration fd) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("funcName", fd.getName());
    attr.add("datatype", fd.getDatatype());

    OnyxElement node = new OnyxElement("onyx.ast.FunctionDeclaration", attr);

    buildChildNodes(fd, node);
  }
  
  public void visit(VariableDeclaration vd) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("varName", vd.getName());
    attr.add("datatype", vd.getDatatype());

    OnyxElement node = new OnyxElement("onyx.ast.VariableDeclaration", attr);

    buildChildNodes(vd, node);
  }
  
  public void visit(ArgumentDeclaration ad) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("varName", ad.getName());
    attr.add("datatype", ad.getDatatype());

    OnyxElement node = new OnyxElement("onyx.ast.ArgumentDeclaration", attr);

    parentNode().addChild(node);
  }
  
  public void visit(ForAssignment fa) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("varName", fa.getName());

    OnyxElement node = new OnyxElement("onyx.ast.ForAssignment", attr);

    buildChildNodes(fa, node);
  }
  
  public void visit(LetAssignment la) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("varName", la.getName());

    OnyxElement node = new OnyxElement("onyx.ast.LetAssignment", attr);

    buildChildNodes(la, node);
  }

  public void visit(Where w) {
    OnyxElement node = new OnyxElement("onyx.ast.Where");
    buildChildNodes(w, node);
  }

  public void visit(Return r) {
    OnyxElement node = new OnyxElement("onyx.ast.Return");
    buildChildNodes(r, node);
  }

  public void visit(Variable v) {
    OnyxTextNode node = new OnyxTextNode("onyx.ast.Variable", v.getName());
    parentNode().addChild(node);
  }
  
  public void visit(Constant c) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("datatype", c.getDatatype());
    OnyxTextNode node = 
        new OnyxTextNode("onyx.ast.Constant", attr, c.getValue());
    parentNode().addChild(node); 
  }
  
  public void visit(FunctionCall fc) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("name", fc.getName());
    OnyxElement node = new OnyxElement("onyx.ast.FunctionCall", attr);
    buildChildNodes(fc, node);
  }
  
  public void visit(Operator o) {
    OnyxAttributeEnvironment attr = new OnyxAttributeEnvironment();
    attr.add("name", o.getName());
    OnyxElement node = new OnyxElement("onyx.ast.Operator", attr);
    buildChildNodes(o, node);
  }
  
  public void visit(Flwr f) {
    OnyxElement node = new OnyxElement("onyx.ast.Flwr");
    buildChildNodes(f, node);
  }
  
  public void visit(IfThenElseExpr iftee) {
    OnyxElement node = new OnyxElement("onyx.ast.IfThenElseExpr");
    buildChildNodes(iftee, node);
  }
  
  public void visit(ExprList el) {
    OnyxElement node = new OnyxElement("onyx.ast.ExprList");
    buildChildNodes(el, node);
  }

  public void visit(NonExpressionNode n) {
    visitChildren(n);
  }


  private void setParentNode(OnyxElement node) {
    st.push(node);
  }

  private void restoreParentNode() {
    st.pop();
  }

  private OnyxElement parentNode() {
    return st.peek();
  }

  private void visitChildren(ASTNode n) {
    for (int i = 0; i < n.numChildren(); ++i)
      n.getChild(i).accept(this);
  }

  private void buildChildNodes(ASTNode ast_node, OnyxElement onyx_node) {
    setParentNode(onyx_node);
    visitChildren(ast_node);
    restoreParentNode();
    parentNode().addChild(onyx_node);
  }
}
