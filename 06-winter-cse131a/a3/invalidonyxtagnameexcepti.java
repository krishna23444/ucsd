/*
 * Author: nitay
 * Date: Mar 10, 2006
 * Project: compilers_a3
 */

/**
 * 
 */
public class InvalidOnyxTagnameException extends SemanticDynamicErrorException {
  /**
   * @param line
   * @param column
   * @param value
   */
  public InvalidOnyxTagnameException(int line, int column, String value) {
    super(line, column, value);
  }

  /**
   * @param line
   * @param column
   */
  public InvalidOnyxTagnameException(int line, int column) {
    super(line, column);
  }

  /**
   * @param value
   */
  public InvalidOnyxTagnameException(String value) {
    super(value);
  }
}
