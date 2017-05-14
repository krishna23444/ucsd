/*
 * Author: nitay, kevin
 * Date: Mar 8, 2006
 * Project: compilers_a3
 */

/**
 * An exception that contains line and column information.
 */
public class LineColumnException extends RuntimeException {
  private int line;
  private int column;

  public LineColumnException(int line, int column, String value) {
    super(value);
    setLineAndColumn(line, column);
  }
  
  public LineColumnException(int line, int column) {
    this(line, column, "YO, YOU CALLED AN EMPTY CONSTRUCTOR MAN"); 
  }
  
  public LineColumnException(String value) {
    this(-1, -1, value);
  }

  public int getLine() {
    return line;
  }
  
  public int getColumn() {
    return column;
  }
  
  public void setLineAndColumn(int line, int column) {
    this.line = line;
    this.column = column;
  }
}
