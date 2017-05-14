import java.util.ArrayList;
import java.util.List;

public class SymbolTable {
  private List<StackFrame> st;

  public SymbolTable() {
    st = new ArrayList<StackFrame>();
    pushFunctionScope();
  }

  public void add(String name, AnyType v) {
    currentScope().add(name, v);
  }
  
  public AnyType lookup(String name) {
    StackFrame scope;
    AnyType var;
    
    for (int index = currentIndex(); index >= 0; --index) {
      scope = st.get(index);
      
      var = scope.lookup(name);
      if (var != null)
        return var;

      if (scope.isNotTraversable())
        break;
    }

    return null;
  }

  private void pushScope(boolean isTraversable) {
    st.add(new StackFrame(isTraversable));
  }
  
  public void pushTraversableScope() {
    pushScope(true);
  }
  
  public void pushFunctionScope() {
    pushScope(false);
  }

  public StackFrame popScope() {
    return st.remove(currentIndex());
  }
  
  public StackFrame currentScope() {
    return st.get(currentIndex());
  }
  
  private int currentIndex() {
    return st.size() - 1;
  }
  
  public void addVariable(String name, String datatype, AnyType value) {
    try {
      // check for valid datatype
      if (Types.classOf(datatype).isInstance(value)) {
        add(name, value);
        // check for sequence, create one if needed
      } else if (Types.classOf(datatype).equals(SequenceType.class)) {
        add(name, value.toSequenceType());
      } else {
        throw new InvalidVariableAssignmentException(
             ErrorMessages.invalidVariableAssignment(name));
      }
    } catch (ClassNotFoundException e) {
      throw new ParserStaticErrorException(
          ErrorMessages.invalidType(datatype));
    }
  }
}
