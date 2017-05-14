import junit.framework.TestCase;

public class SequenceTest extends TestCase {
  private SequenceType seq;
 
  public void setUp() {
    seq = new SequenceType();
  }
//
//  public void testToSingleton() {
//    AnySimpleType a = new StringType("jojdfi");
//    seq.add(a);
//    try {
//      assertSame(a, seq.toSingleton());
//    } catch (NonSingletonSequenceException e) {
//      fail();
//    }
//  }
//
//  public void testBadToSingleton() {
//    try {
//      seq.toSingleton();
//      fail();
//    } catch (NonSingletonSequenceException e) {
//    }
//  }

  public void testAddToSequence() {
    AnySimpleType a = new StringType("sodfdoj");
    assertEquals(0, seq.size());
    seq.add(a);
    assertEquals(1, seq.size());
    seq.add(a);
    assertEquals(2, seq.size());
  }
  
  public void testToString() {
    BooleanType first_born = new BooleanType("true");
    DecimalType middle_child = new DecimalType("3.14");
    StringType baby = new StringType("fetus");
    IntegerType unborn = new IntegerType("-7");
    
    //  TODO: test all the rest of the onyx types
    
    seq.add(first_born, middle_child, baby, unborn);
    
    assertEquals("[true, 3.14, fetus, -7]", seq.toString());
  }

  public void testRange() {
    SequenceType s = SequenceType.range(new IntegerType(1), new IntegerType(3));
    SequenceType expected = new SequenceType(new IntegerType(1), 
                                             new IntegerType(2),
                                             new IntegerType(3));
    assertEquals(expected, s);
    assertFalse(s.equals(new IntegerType(5)));
  }
  
  public void tearDown() {
  }
}
