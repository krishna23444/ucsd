import junit.framework.Test;
import junit.framework.TestSuite;

public class AllTests {
  public static Test suite() {
    TestSuite suite = new TestSuite();
    suite.addTestSuite(EvalVisitorTest.class);
    suite.addTestSuite(FunctionPrototypeTest.class);
    suite.addTestSuite(FunctionTableTest.class);
    suite.addTestSuite(ParamTypeTest.class);
    suite.addTestSuite(SequenceTest.class);
    suite.addTestSuite(SymbolTableTest.class);
    suite.addTestSuite(UtilTest.class);
    return suite;
  }

  public static void main(String [] args) {
    junit.textui.TestRunner.run(suite());
  }
}
