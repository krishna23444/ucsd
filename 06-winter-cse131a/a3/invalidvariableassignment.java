/*
 * Author: nitay
 * Date: Mar 10, 2006
 * Project: compilers_a3
 */

/**
 * 
 */
public class InvalidVariableAssignmentException 
       extends SemanticDynamicErrorException {

  /**
   * @param line
   * @param column
   * @param value
   */
  public InvalidVariableAssignmentException(int line, int column,
                                            String value) {
    super(line, column, value);
  }

  /**
   * @param line
   * @param column
   */
  public InvalidVariableAssignmentException(int line, int column) {
    super(line, column);
  }

  /**
   * @param value
   */
  public InvalidVariableAssignmentException(String value) {
    super(value);
  }

}
