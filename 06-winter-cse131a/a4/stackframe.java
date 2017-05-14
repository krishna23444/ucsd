import java.util.HashMap;

public class StackFrame {
  private HashMap<String, AnyType> data;
  private boolean traversable; 
    
  public StackFrame(boolean traversable) {
    data = new HashMap<String, AnyType>();
    this.traversable = traversable;
  }
  
  public boolean isTraversable() {
    return traversable;
  }
  
  public boolean isNotTraversable() {
    return !traversable;
  }

  public AnyType add(String name, AnyType value) {
    return data.put(name, value);
  }
  
  public boolean update(String name, AnyType value) {
    if (!data.containsKey(name))
      return false;
    
    return (add(name, value) != null);
  }
  
  public AnyType lookup(String name) { 
    return data.get(name);
  }
  
  public AnyType remove(String name) {
    return data.remove(name);
  }
}
