import onyx.xml.OnyxNode;

public abstract class NodeType extends XmlType {
  public boolean equals(Object o) {
    if (!(o instanceof NodeType))
      return false;

    return super.equals(o);
  }
  
  public int hashCode() {
    return super.hashCode();
  }
  
  public abstract AttrEnvType getAttr();

  public abstract void setAttr(AttrEnvType attr);
  
  public abstract StringType getTagName();
  
  public abstract OnyxNode getNode();
}
