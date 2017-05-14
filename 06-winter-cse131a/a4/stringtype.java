public class StringType extends AnySimpleType {
  private String value;

  public StringType(String s) {
    value = new String(s);
  }
  
  public StringType(StringType s) {
    this(s.getValue());
  }

  public String getValue() {
    return value;
  }
  
  public StringType concat(StringType s) {
    return new StringType(value.concat(s.value));
  }
  
  public StringType add(StringType s) {
    return concat(s);
  }

  public BooleanType equalTo(StringType i) {
    return new BooleanType(value.equals(i.value));
  }
  
  public BooleanType equalTo(AnySimpleType i) {
    if (getClass() == i.getClass())
      return equalTo((StringType)i);
    
    // TODO: handle wrong class type err
    return null;
  }
  
  public BooleanType notEqualTo(StringType i) {
    return new BooleanType(!value.equals(i.value));
  }
  
  public BooleanType notEqualTo(AnySimpleType i) {
    if (getClass() == i.getClass())
      return notEqualTo((StringType)i);
    
    // TODO: handle wrong class type err
    return null;
  }

  public BooleanType lessThan(StringType s) {
    return new BooleanType(value.compareTo(s.value) < 0);
  }
  
  public BooleanType greaterThan(StringType s) {
    return new BooleanType(value.compareTo(s.value) > 0);
  }
  
  public BooleanType lessEquals(StringType s) {
    return new BooleanType(value.compareTo(s.value) <= 0);
  }
  
  public BooleanType greaterEquals(StringType s) {
    return new BooleanType(value.compareTo(s.value) >= 0);
  }
  
  public IntegerType toIntegerType() {
    return new IntegerType(value);
  }

  public DecimalType toDecimalType() {
    return new DecimalType(value);
  }
  
  public boolean equals(Object o) {
    if (!(o instanceof StringType))
      return false;
    
    return super.equals(o) && value.equals(((StringType)o).value);
  }
  
  public int hashCode() {
    return super.hashCode() + value.hashCode();
  }

  public String toString() {
    return value;
  }
  
  public String stringOf() {
    return value;
  }
}
