/*
 * Author: nitay, kevin
 * Date: Mar 8, 2006
 * Project: compilers_a3
 */

/**
 * 
 */
public class ParserStaticErrorException extends LineColumnException {
  /**
   * @param line
   * @param column
   * @param value
   */
  public ParserStaticErrorException(int line, int column, String value) {
    super(line, column, value);
  }
  
  /**
   * @param line
   * @param column
   */
  public ParserStaticErrorException(int line, int column) {
    super(line, column);
  }
  
  public ParserStaticErrorException(String s) {
    super(s);
  }
}
