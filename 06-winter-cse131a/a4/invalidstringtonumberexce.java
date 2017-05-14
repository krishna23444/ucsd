/*
 * Author: nitay
 * Date: Mar 10, 2006
 * Project: compilers_a3
 */

/**
 * 
 */
public class InvalidStringToNumberException extends
    SemanticDynamicErrorException {

  /**
   * @param line
   * @param column
   * @param value
   */
  public InvalidStringToNumberException(int line, int column, String value) {
    super(line, column, value);
  }

  /**
   * @param line
   * @param column
   */
  public InvalidStringToNumberException(int line, int column) {
    super(line, column);
  }

  /**
   * @param value
   */
  public InvalidStringToNumberException(String value) {
    super(value);
  }

}
