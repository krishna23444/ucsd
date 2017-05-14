public class Constant extends ExpressionNode implements Visitable {
  private String datatype, value;

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

  public String toString() {
    return "Constant: datatype = " + datatype + ", content = " + value;
  }
}
