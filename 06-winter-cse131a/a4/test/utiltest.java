import junit.framework.TestCase;

public class UtilTest extends TestCase {
  protected void setUp() throws Exception {
    super.setUp();
  }

  public void testSameClass() {
    assertTrue(Util.sameClass(String.class, "Hi", "my", "name", "is"));
    assertFalse(Util.sameClass(String.class, "not", new Integer(45)));
  }

  protected void tearDown() throws Exception {
    super.tearDown();
  }
}
