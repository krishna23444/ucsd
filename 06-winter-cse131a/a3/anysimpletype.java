public abstract class AnySimpleType extends AnyType {
  public abstract BooleanType equalTo(AnySimpleType a);
  
  public abstract BooleanType notEqualTo(AnySimpleType a);
  
  public boolean equals(Object o) {
    if (!(o instanceof AnySimpleType))
      return false;
    
    return super.equals(o);
  }
  
  public int hashCode() {
    return super.hashCode();
  }
}
