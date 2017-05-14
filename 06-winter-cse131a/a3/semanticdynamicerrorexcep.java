/*
 * Author: nitay
 * Date: Mar 8, 2006
 * Project: compilers_a3
 */

/**
 * 
 */
public class SemanticDynamicErrorException extends LineColumnException {
  /**
   * @param line
   * @param column
   * @param value
   */
  public SemanticDynamicErrorException(int line, int column, String value) {
    super(line, column, value);
  }

  /**
   * @param line
   * @param column
   */
  public SemanticDynamicErrorException(int line, int column) {
    super(line, column);
  }
  
  public SemanticDynamicErrorException(String value) {
    super(value);
  }
}
