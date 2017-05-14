import java.math.BigInteger;

public class IntegerType extends NumberType {
  private BigInteger value;

  public IntegerType() {
    value = null;
  }

  public IntegerType(String s) {
    value = new BigInteger(s);
  }
  
  public IntegerType(int i) {
    this(String.valueOf(i));
  }

  public IntegerType(BigInteger i) {
    this(i.toString());
  }
  
  public IntegerType(IntegerType i) {
    this(i.value);
  }

  public BigInteger getValue() {
    return value;
  }

  public DecimalType toDecimalType() {
    return new DecimalType(value.toString());
  }

  public boolean equals(Object o) {
    if (!(o instanceof IntegerType))
      return false;
    
    return super.equals(o) && value.equals(((IntegerType)o).value);
  }
  
  public int hashCode() {
    return super.hashCode() + value.hashCode();
  }
  
  public String toString() {
    return value.toString();
  }
  
  public String stringOf() {
    return value.toString();
  }

  public IntegerType add(IntegerType i) {
    return new IntegerType(value.add(i.value));
  }
  
  public IntegerType subtract(IntegerType i) {
    return new IntegerType(value.subtract(i.value));
  }
  
  public IntegerType multiply(IntegerType i) {
    return new IntegerType(value.multiply(i.value));
  }
  
  public IntegerType divide(IntegerType i) {
    if (i.getValue().equals(BigInteger.ZERO))
      throw new SemanticDynamicErrorException(ErrorMessages.DIV_BY_ZERO);

    return new IntegerType(value.divide(i.value));
  }
  
  public IntegerType mod(IntegerType i) {
    if (i.getValue().compareTo(BigInteger.ZERO) <= 0)
      throw new SemanticDynamicErrorException(ErrorMessages.MOD_BY_NON_POS);

    return new IntegerType(value.mod(i.value));
  }
  
  public IntegerType negate() {
    return new IntegerType(value.negate());
  }
  
  public SequenceType to(IntegerType i) {
    return SequenceType.range(this, i);
  }
  
  public BooleanType equalTo(IntegerType i) {
    return new BooleanType(value.equals(i.value));
  }
  
  public BooleanType equalTo(AnySimpleType i) {
    if (getClass() == i.getClass())
      return equalTo((IntegerType)i);
    
    // TODO: handle wrong class type err
    return null;
  }
  
  public BooleanType notEqualTo(IntegerType i) {
    return new BooleanType(!value.equals(i.value));
  }
  
  public BooleanType notEqualTo(AnySimpleType i) {
    if (getClass() == i.getClass())
      return notEqualTo((IntegerType)i);
    
    // TODO: handle wrong class type err
    return null;
  }
  
  public BooleanType lessThan(IntegerType i) {
    return new BooleanType(value.compareTo(i.value) < 0);
  }
  
  public BooleanType greaterThan(IntegerType i) {
    return new BooleanType(value.compareTo(i.value) > 0);
  }
  
  public BooleanType lessEquals(IntegerType i) {
    return new BooleanType(value.compareTo(i.value) <= 0);
  }
  
  public BooleanType greaterEquals(IntegerType i) {
    return new BooleanType(value.compareTo(i.value) >= 0);
  }
  
}
