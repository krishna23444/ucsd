/*
 * Author: nitay, kevin
 * Date: Mar 7, 2006
 * Project: compilers_a3
 */

import junit.framework.TestCase;

/**
 * 
 */
public class ParamTypeTest extends TestCase {
  /*
   * @see TestCase#setUp()
   */
  protected void setUp() throws Exception {
    super.setUp();
  }

  public void testNumPromotionsTo() {
  }
  
  public void testPromoteTo() {
    // anything to AnyType
    ParamType pt = new ParamType("a", Types.ANY);
    AnyType at = new DecimalType(3.14);
    assertEquals(new DecimalType(3.14), pt.promoteTo(at));
    at = new StringType("joe");
    assertEquals(new StringType("joe"), pt.promoteTo(at));
    
    // integer to decimal
    pt = new ParamType("a", Types.DECIMAL);
    at = new IntegerType(-4);
    assertEquals(new DecimalType(-4), pt.promoteTo(at));
    
    // single to singleton sequence
    pt = new ParamType("a", Types.SEQUENCE);
    at = new IntegerType(-15);
    assertEquals(new SequenceType(new IntegerType(-15)), pt.promoteTo(at));
    
    // shouldn't be able to promote
    pt = new ParamType("a", Types.ENODE);
    at = new BooleanType(true);
    assertNull(pt.promoteTo(at));
  }
  
  /*
   * @see TestCase#tearDown()
   */
  protected void tearDown() throws Exception {
    super.tearDown();
  }
}
