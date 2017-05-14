public class LexicalErrorException extends LineColumnException {
  public LexicalErrorException(int line, int column, String name) {
    super(line, column, name);
  }
  
  public LexicalErrorException(int line, int column) {
    super(line, column);
  }
}
