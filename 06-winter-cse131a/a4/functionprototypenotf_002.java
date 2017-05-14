/*
 * Author: nitay
 * Date: Mar 9, 2006
 * Project: compilers_a3
 */

import java.util.List;

/**
 * 
 */
public class FunctionPrototypeNotFoundPrefixesException extends
    SemanticDynamicErrorException {
  
  private List<FunctionPrototype> possible_matches;
  
  /**
   * @param line
   * @param column
   * @param value
   */
  public FunctionPrototypeNotFoundPrefixesException(int line,
                                                    int column,
                                                    String value) {
    super(line, column, value);
  }
  
  /**
   * @param line
   * @param column
   * @param matches
  */
  public FunctionPrototypeNotFoundPrefixesException(
     int line, int column, String value, List<FunctionPrototype> matches) {
    this(line, column, value);
    setPossibleMatches(matches);
  }

  /**
   * @param value
   */
  public FunctionPrototypeNotFoundPrefixesException(String value) {
    super(value);
  }
  
  /**
   * @param value
   */
  public FunctionPrototypeNotFoundPrefixesException(String value,
      List<FunctionPrototype> matches) {
    super(value);
    setPossibleMatches(matches);
  }
  
  public List<FunctionPrototype> getPossibleMatches() {
    return possible_matches;
  }
  
  public void setPossibleMatches(List<FunctionPrototype> matches) {
    this.possible_matches = matches; 
  }

}
