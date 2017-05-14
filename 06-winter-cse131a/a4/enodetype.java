import onyx.xml.OnyxElement;
import onyx.xml.OnyxNode;
import onyx.xml.OnyxTextNode;

public class ENodeType extends NodeType {
  private OnyxElement content;
  
  public ENodeType() {
    content = null;
  }
  
  public ENodeType(OnyxNode n) {
    content = new OnyxElement(n);
  }

  public ENodeType(ENodeType t) {
    this(t.content);
  }
  
  public ENodeType(StringType name) {
    content = new OnyxElement(name.getValue());
  }
  
  public ENodeType(StringType name, AttrEnvType attr_env) {
    content = new OnyxElement(name.getValue(), attr_env.getValue());
  }
  
  public void add(NodeType ... nodes) {
    for (NodeType n : nodes) {
      if (TNodeType.class == n.getClass())
        content.addChild(((TNodeType)n).getValue());
      else if (ENodeType.class == n.getClass())
        content.addChild(((ENodeType)n).getValue());
    }
  }

  public OnyxNode getNode() {
    return content;
  }
  
  public OnyxElement getValue() {
    return content;
  }

  public AttrEnvType getAttr() {
    return new AttrEnvType(content.getAttributes());
  }
  
  public void setAttr(AttrEnvType attr) {
    content.setAttributeEnvironment(attr.getValue());
  }
  
  public StringType getTagName() {
    return new StringType(content.getTagName());
  }
  
  public SequenceType childrenSequence() {
    SequenceType s = new SequenceType();
    for (OnyxNode n : content.getChildren()) {
      if (n instanceof OnyxTextNode)
        s.add(new TNodeType(n));
      else if (n instanceof OnyxElement)
        s.add(new ENodeType(n));
    }
    return s;
  }

  public String stringOf() {
    return content.toString();
  }
  
  public boolean equals(Object o) {
    if (!(o instanceof ENodeType))
      return false;
    
    ENodeType e = (ENodeType)o;
    
    return super.equals(o)
           && content.getChildren().equals(e.content.getChildren())
           && getAttr().equals(e.getAttr());
  }
  
  public String toString() {
    return content.toString();
  }
  
  public int hashCode() {
    return super.hashCode() + content.hashCode();
  }
}
