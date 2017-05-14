import junit.framework.TestCase;

public class SymbolTableTest extends TestCase {
  private SymbolTable st;
  
  public SymbolTableTest(String arg0) {
    super(arg0);
  }

  protected void setUp() throws Exception {
    super.setUp();
    
    st = new SymbolTable();
  }
  
  public void testSimpleAddLookup() {
    AnyType sym = new StringType("doesn't matter");
    
    st.add("sym", sym);
    AnyType var = st.lookup("sym");

    assertSame(sym, var);
    assertNull(st.lookup("should fail"));
  }
  
  public void testFrameTraversal() {
    AnyType sym = new StringType("doesn't matter");
    
    st.add("sym", sym);
    st.pushTraversableScope();
    st.pushTraversableScope();
    assertSame(sym, st.lookup("sym"));
    
    st.pushFunctionScope();
    assertNull(st.lookup("sym"));
    
    st.popScope();
    
    assertSame(sym, st.lookup("sym"));
    
    AnyType new_sym = new StringType("something else");

    st.add("sym", new_sym);
    assertSame(new_sym, st.lookup("sym"));
    assertNotSame(sym, st.lookup("sym"));
    
    st.popScope();
    assertSame(sym, st.lookup("sym"));
    assertNotSame(new_sym, st.lookup("sym"));
  }
  
  protected void tearDown() throws Exception {
    super.tearDown();
  }
}
