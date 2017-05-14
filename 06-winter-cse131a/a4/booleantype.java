public class BooleanType extends AnySimpleType {
  private Boolean value;
  
  public BooleanType(String s) {
    value = new Boolean(s);
  }

  public BooleanType(boolean b) {
    value = new Boolean(b);
  }

  public BooleanType(BooleanType b) {
    this(b.getValue());
  }

  public Boolean getValue() {
    return value;
  }

  public void setValue(Boolean value) {
    this.value = value;
  }
  
  public BooleanType and(BooleanType b) {
    return new BooleanType(value.booleanValue() && b.value.booleanValue());
  }
  
  public BooleanType or(BooleanType b) {
    return new BooleanType(value.booleanValue() || b.value.booleanValue());
  }

  public BooleanType equalTo(BooleanType b) {
    return new BooleanType(value.equals(b.value));
  }

  public BooleanType equalTo(AnySimpleType b) {
    if (getClass() == b.getClass())
      return equalTo((BooleanType)b);
    
    // TODO: handle cast error
    return null;
  }
  
  public BooleanType notEqualTo(BooleanType b) {
    return new BooleanType(!value.equals(b.value));
  }
  
  public BooleanType notEqualTo(AnySimpleType b) {
    if (getClass() == b.getClass())
      return notEqualTo((BooleanType)b);
    
    // TOOD: handle cast error
    return null;
  }
  
  public String stringOf() {
    return value.toString();
  }

  public boolean equals(Object o) {
    if (!(o instanceof BooleanType))
      return false;
    
    return super.equals(o) && value.equals(((BooleanType)o).value);
  }
  
  public int hashCode() {
    return super.hashCode() + value.hashCode();
  }

  public String toString() {
    return value.toString();
  }
}
