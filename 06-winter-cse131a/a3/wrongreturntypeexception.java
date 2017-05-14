/*
 * Author: nitay
 * Date: Mar 10, 2006
 * Project: compilers_a3
 */

/**
 * 
 */
public class WrongReturnTypeException extends SemanticDynamicErrorException {
  /**
   * @param line
   * @param column
   * @param value
   */
  public WrongReturnTypeException(int line, int column, String value) {
    super(line, column, value);
  }

  /**
   * @param line
   * @param column
   */
  public WrongReturnTypeException(int line, int column) {
    super(line, column);
  }

  /**
   * @param value
   */
  public WrongReturnTypeException(String value) {
    super(value);
  }
}
