/*
 * Author: nitay
 * Date: Mar 10, 2006
 * Project: compilers_a3
 */

/**
 * 
 */
public class InvalidOnyxDocumentException
      extends SemanticDynamicErrorException {
  /**
   * @param line
   * @param column
   * @param value
   */
  public InvalidOnyxDocumentException(int line, int column, String value) {
    super(line, column, value);
  }

  /**
   * @param line
   * @param column
   */
  public InvalidOnyxDocumentException(int line, int column) {
    super(line, column);
  }

  /**
   * @param value
   */
  public InvalidOnyxDocumentException(String value) {
    super(value);
  }

}
