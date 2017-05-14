import java.util.SortedSet;

/*
 * Author: nitay, kevin
 * Date: Mar 9, 2006
 * Project: compilers_a3
 */

/**
 * 
 */
public class FunctionPrototypeNotFoundCandidatesException extends
    SemanticDynamicErrorException {

  private SortedSet<String> possible_matches;
  
  public FunctionPrototypeNotFoundCandidatesException(int line, int column,
                                                      String value) {
    super(line, column, ErrorMessages.functionPrototypeNotFound(value));
  }

  /**
   * @param line
   * @param column
   * @param matches
   */
  public FunctionPrototypeNotFoundCandidatesException(
      int line, int column, String value, SortedSet<String> matches) {
    this(line, column, value);
    setPossibleMatches(matches);
  }

  /**
   * @param value
   */
  public FunctionPrototypeNotFoundCandidatesException(String value) {
    super(ErrorMessages.functionPrototypeNotFound(value));
  }

  public SortedSet<String> getPossibleMatches() {
    return possible_matches;
  }
  
  public void setPossibleMatches(SortedSet<String> matches) {
    this.possible_matches = matches;
  }
}
