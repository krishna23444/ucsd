public abstract class NumberType extends AnySimpleType {
  public boolean equals(Object o) {
    if (!(o instanceof NumberType))
      return false;
    
    return super.equals(o);
  }
  
  public int hashCode() {
    return super.hashCode();
  }
}
