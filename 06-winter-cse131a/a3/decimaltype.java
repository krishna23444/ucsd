import java.math.BigDecimal;
import java.math.MathContext;

public class DecimalType extends NumberType {
  private BigDecimal value;

  public DecimalType(String s) {
    value = new BigDecimal(s, MathContext.UNLIMITED);
  }

  public DecimalType(double d) {
    this(String.valueOf(d));
  }
  
  public DecimalType(int i) {
    this(String.valueOf(i));
  }
  
  public DecimalType(BigDecimal value) {
    this(value.toString());
  }
  
  public DecimalType(DecimalType d) {
    this(d.getValue());
  }

  
  public BigDecimal getValue() {
    return value;
  }
  
  public boolean equals(Object o) {
    if (!(o instanceof DecimalType))
      return false;
    
    return super.equals(o) && value.equals(((DecimalType)o).value);
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

  public DecimalType add(DecimalType d) {
    return new DecimalType(this.value.add(d.value));
  }
  
  public DecimalType subtract(DecimalType d) {
    return new DecimalType(value.subtract(d.value));
  }
  
  public DecimalType multiply(DecimalType d) {
    return new DecimalType(value.multiply(d.value));
  }
  
  public DecimalType divide(DecimalType d) {
    if (d.getValue().equals(BigDecimal.ZERO))
      throw new SemanticDynamicErrorException(ErrorMessages.DIV_BY_ZERO);
      
    return new DecimalType(value.divide(d.value, value.scale(),
                                        BigDecimal.ROUND_HALF_EVEN));
    //return new DecimalType(value.divide(d.value));
  }
  
  public DecimalType negate() {
    return new DecimalType(value.negate());
  }
  
  public BooleanType equalTo(DecimalType d) {
    return new BooleanType(value.equals(d.value));
  }
  
  public BooleanType equalTo(AnySimpleType d) {
    if (getClass() == d.getClass())
      return equalTo((DecimalType)d);
    
    // TODO: handle wrong class type error
    return null;
  }
  
  public BooleanType notEqualTo(DecimalType d) {
    return new BooleanType(!value.equals(d.value));
  }
  
  public BooleanType notEqualTo(AnySimpleType d) {
    if (getClass() == d.getClass())
      return notEqualTo((DecimalType)d);
    
    // TODO: handle wrong class type error
    return null;
  }
  
  public BooleanType lessThan(DecimalType d) {
    return new BooleanType(value.compareTo(d.value) < 0);
  }
  
  public BooleanType greaterThan(DecimalType d) {
    return new BooleanType(value.compareTo(d.value) > 0);
  }
  
  public BooleanType lessEquals(DecimalType d) {
    return new BooleanType(value.compareTo(d.value) <= 0);
  }
  
  public BooleanType greaterEquals(DecimalType d) {
    return new BooleanType(value.compareTo(d.value) >= 0);
  }
  
  public IntegerType toIntegerType() {
    return new IntegerType(value.toBigInteger());
  }
}
