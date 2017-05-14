public class SyntaxError extends RuntimeException {
  private int line;
  private int column;

  public SyntaxError(int line, int column) {
    this.line = line;
    this.column = column;
  }

  public int getLine() {
    return line;
  }

  public int getColumn() {
    return column;
  }
}
