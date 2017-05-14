import onyx.xml.OnyxNode;
import onyx.xml.OnyxTextNode;

public class TNodeType extends NodeType {
  private OnyxTextNode content;
  
  public TNodeType() {
    content = null;
  }
  
  public TNodeType(OnyxNode n) {
    content = new OnyxTextNode(n);
  }

  public TNodeType(TNodeType t) {
    this(t.content);
  }

  public TNodeType(StringType name, AttrEnvType attrenv, StringType content) {
    this.content = new OnyxTextNode(name.getValue(), attrenv.getValue(),
                                    content.getValue());
  }
  
  public TNodeType(StringType name, StringType content) {
    this.content = new OnyxTextNode(name.getValue(), content.getValue());
  }

  public OnyxTextNode getValue() {
    return content;
  }

  public OnyxNode getNode() {
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
  
  public String getText() {
    return content.getText();
  }

  public boolean equals(Object o) {
    if (!(o instanceof TNodeType))
      return false;
    
    return super.equals(o) && getText().equals(((TNodeType)o).getText());
  }
  
  public int hashCode() {
    return super.hashCode() + content.hashCode();
  }
  
  public String toString() {
    return content.toString();
  }

  public String stringOf() {
    return content.toString();
  }
}
