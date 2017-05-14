public abstract class XmlType extends AnyType {
  public boolean equals(Object o) {
    if (!(o instanceof XmlType))
      return false;
    
    return super.equals(o);
  }
  
  public int hashCode() {
    return super.hashCode();
  }
}
