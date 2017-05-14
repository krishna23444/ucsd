public abstract class AnyType {
  public boolean equals(Object o) {
    if (!(o instanceof AnyType))
      return false;

    return true;
  }

  public int hashCode() {
    return super.hashCode();
  }

  public abstract String stringOf();

  public SequenceType toSequenceType() {
    return new SequenceType(this);
  }
}
