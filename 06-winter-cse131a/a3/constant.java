/**
 * Has 2 attributes: datatype and value.
 */
public class Constant extends ExpressionNode implements Visitable {
  private String datatype;
  private String value;

  public Constant(String datatype, String value) {
    this.datatype = datatype;
    this.value = value;
  }

  public void accept(Visitor visitor) {
    visitor.visit(this);
  }

  public String getValue() {
    return value;
  }

  public String getDatatype() {
    return datatype;
  }

  public boolean equals(Object o) {
    if (!(o instanceof Constant))
      return false;
    
    Constant c = (Constant)o;
    return super.equals(o) && datatype.equals(c.datatype)
                           && value.equals(c.value);
  }
  
  public int hashCode() {
    return super.hashCode() + datatype.hashCode() + value.hashCode();
  }

  public String toString() {
    return datatype + ":" + value;
  }
}
