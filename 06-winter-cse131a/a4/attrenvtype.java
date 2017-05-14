import java.util.Set;
import onyx.xml.OnyxAttributeEnvironment;

public class AttrEnvType extends XmlType {
  private OnyxAttributeEnvironment attr_env;
  
  public AttrEnvType() {
    attr_env = new OnyxAttributeEnvironment();
  }

  public AttrEnvType(OnyxAttributeEnvironment e) {
    this();
    for (String s: e.keySet())
      attr_env.add(s, e.get(s));
  }
  
  public AttrEnvType(AttrEnvType attr) {
    this(attr.getValue());
  }
  
  public void add(StringType key, StringType value) {
    attr_env.add(key.getValue(), value.getValue());
  }
  
  public boolean equals(Object o) {
    if (!(o instanceof AttrEnvType))
      return false;
    
    // OnyxAttributeEnvironment doesn't define equals, so we do it for them
    Set<String> keys = keySet();
    AttrEnvType a = (AttrEnvType)o;
    Set<String> others_keys = a.attr_env.keySet();
    
    if (!keys.equals(others_keys))
      return false;
    
    for (String k : keySet())
      if (!attr_env.get(k).equals(a.attr_env.get(k)))
        return false;
    
    return true;
  }
  
  public int hashCode() {
    return super.hashCode() + attr_env.hashCode();
  }

  public String toString() {
    return attr_env.toString();
  }
  
  public String stringOf() {
    return attr_env.toString();
  }
  
  public OnyxAttributeEnvironment getValue() {
    return attr_env;
  }
  
  public Set<String> keySet() {
    return attr_env.keySet();
  }
  
  public StringType lookup(StringType s) {
    if (!attr_env.contains(s.getValue()))
      return null;
    
    return new StringType(attr_env.get(s.getValue()));
  }
}
