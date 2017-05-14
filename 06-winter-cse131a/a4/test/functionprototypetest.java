import junit.framework.TestCase;

public class FunctionPrototypeTest extends TestCase {
  protected void setUp() throws Exception {
    super.setUp();
  }

  public void testIsPrefixOf() {
    ParamType[] args = { new ParamType("$a", Types.INTEGER),
                         new ParamType("$b", Types.BOOLEAN) };

    FunctionPrototype fp = new FunctionPrototype("abcd", args);
    
    // blank name -> prefix
    FunctionPrototype new_fp = new FunctionPrototype("", args);
    assertTrue(new_fp.isPrefixOf(fp));
    
    // same name -> prefix
    new_fp = new FunctionPrototype("abcd", args);
    assertTrue(new_fp.isPrefixOf(fp));
    
    // not prefix
    new_fp = new FunctionPrototype("abcde", args);
    assertFalse(new_fp.isPrefixOf(fp));
    
    // not prefix
    new_fp = new FunctionPrototype("oweujpdvlkj", args);
    assertFalse(new_fp.isPrefixOf(fp));
    
    // prefix
    new_fp = new FunctionPrototype("ab", args);
    assertTrue(new_fp.isPrefixOf(fp));
    
    // no args -> not prefix
    new_fp = new FunctionPrototype("ab");
    assertFalse(new_fp.isPrefixOf(fp));
    
    // wrong arg type -> not prefix
    ParamType[] new_args = { new ParamType("$a", Types.BOOLEAN),
                             new ParamType("$b", Types.BOOLEAN) };
    
    new_fp = new FunctionPrototype("ab", new_args);
    assertFalse(new_fp.isPrefixOf(fp));
    
    // wrong arg name -> prefix
    new_args[0] = new ParamType("$ac", Types.INTEGER);

    new_fp = new FunctionPrototype("ab", new_args);
    assertTrue(new_fp.isPrefixOf(fp));
    
    /// wrong return type -> prefix
    new_fp = new FunctionPrototype("abc", Types.STRING, args);
    assertTrue(new_fp.isPrefixOf(fp));
  }
  
  public void testCompareTo() {
    ParamType[] a = { new ParamType("$q", Types.DECIMAL) };
    ParamType[] na = { new ParamType("$p", Types.ATTR_ENV) };
    
    FunctionPrototype fp = new FunctionPrototype("abc", a);
    
    // shorter name
    FunctionPrototype nfp = new FunctionPrototype("abcd", na);
    assertTrue(fp.compareTo(nfp) < 0);
    
    // longer name
    nfp = new FunctionPrototype("dw", na);
    assertTrue(fp.compareTo(nfp) > 0);
    
    // same name
    nfp = new FunctionPrototype("abc", na);
    assertTrue(fp.compareTo(nfp) == 0);
    
    // same length, lexicographical ordering
    nfp = new FunctionPrototype("def", na);
    assertTrue(fp.compareTo(nfp) < 0);
  }
  
  public void testExactlyMatches() {
    ParamType[] a = { new ParamType("$p", Types.ATTR_ENV),
                      new ParamType("$f", Types.XML) };
    
    ParamType[] na = { new ParamType("$p", Types.ATTR_ENV),
                       new ParamType("$f", Types.XML) };
    
    FunctionPrototype fp = new FunctionPrototype("func", Types.NODE, a);
    FunctionPrototype nfp = new FunctionPrototype("func", Types.NODE, na);
    
    // same -> exactly equal
    assertTrue(fp.exactlyMatches(nfp));
    
    // different return type -> not exactly equal
    nfp = new FunctionPrototype("func", na);
    assertFalse(fp.exactlyMatches(nfp));
    
    // different param name -> not exactly equal
    na[0] = new ParamType("$a", Types.ATTR_ENV);
    nfp = new FunctionPrototype("func",Types.NODE, na);
    assertFalse(fp.exactlyMatches(nfp));
    
    // different param type -> not exactly equal
    na[0] = new ParamType("$p", Types.DECIMAL);
    nfp = new FunctionPrototype("func",Types.NODE, na);
    assertFalse(fp.exactlyMatches(nfp));
  }
  
  public void testStringForm() {
    FunctionPrototype fp =
      new FunctionPrototype(Ops.ADD, Types.DECIMAL,Types.BOOLEAN, Types.ANY);
    
    assertEquals(Types.DECIMAL + " " + Ops.ADD + "(" + Types.BOOLEAN + ","
                 + Types.ANY + ")", fp.toCandidateErrorString());
    
    fp = new FunctionPrototype(Ops.LESS_EQUALS, Types.ENODE, Types.TNODE);
    
    assertEquals(Types.ENODE + " " + Ops.LESS_EQUALS + "(" + Types.TNODE + ")",
                 fp.toCandidateErrorString());
  }

  public void testToOnyxNode() {
  }
  
  public void testNumPromotionsTo() {
    // One argument
    FunctionPrototype fp =
      new FunctionPrototype("doit", Types.ANY, Types.DECIMAL);
    FunctionPrototype fp_int =
      new FunctionPrototype("doit", Types.ANY, Types.INTEGER);
    FunctionPrototype fp_dec =
      new FunctionPrototype("doit", Types.ANY, Types.DECIMAL);
    FunctionPrototype fp_seq =
      new FunctionPrototype("doit", Types.ANY, Types.SEQUENCE);

    assertEquals(0, fp_dec.numPromotionsTo(fp));
    assertEquals(1, fp_int.numPromotionsTo(fp));
    assertEquals(FunctionPrototype.CANNOT_PROMOTE, fp_seq.numPromotionsTo(fp));

    FunctionPrototype fp_any =
      new FunctionPrototype("doit", Types.ANY, Types.ANY);

    assertEquals(0, fp.numPromotionsTo(fp_any));
    assertEquals(0, fp_int.numPromotionsTo(fp_any));
    assertEquals(0, fp_dec.numPromotionsTo(fp_any));
    assertEquals(0, fp_seq.numPromotionsTo(fp_any));

    // Two argument
    fp = new FunctionPrototype("doit", Types.ANY, Types.ANY, Types.ANY);
    FunctionPrototype fp_dec_dec =
      new FunctionPrototype("doit", Types.ANY, Types.DECIMAL, Types.DECIMAL);
    FunctionPrototype fp_int_dec =
      new FunctionPrototype("doit", Types.ANY, Types.INTEGER, Types.DECIMAL);
    FunctionPrototype fp_dec_int =
      new FunctionPrototype("doit", Types.ANY, Types.DECIMAL, Types.INTEGER);
    FunctionPrototype fp_int_int =
      new FunctionPrototype("doit", Types.ANY, Types.INTEGER, Types.INTEGER);
    FunctionPrototype fp_str_str =
      new FunctionPrototype("doit", Types.ANY, Types.STRING, Types.STRING);

    // Test against Any
    assertEquals(0, fp_dec_dec.numPromotionsTo(fp));
    assertEquals(0, fp_int_dec.numPromotionsTo(fp));
    assertEquals(0, fp_dec_int.numPromotionsTo(fp));
    assertEquals(0, fp_int_int.numPromotionsTo(fp));
    assertEquals(0, fp_str_str.numPromotionsTo(fp));

    // Test against dec_dec
    assertEquals(1, fp_int_dec.numPromotionsTo(fp_dec_dec));
    assertEquals(1, fp_dec_int.numPromotionsTo(fp_dec_dec));
    assertEquals(2, fp_int_int.numPromotionsTo(fp_dec_dec));
    assertEquals(FunctionPrototype.CANNOT_PROMOTE,
                 fp_str_str.numPromotionsTo(fp_dec_dec));
    
    // Test XML Generics
    FunctionPrototype fp_xml = 
      new FunctionPrototype("doit", Types.ANY, Types.XML);
    FunctionPrototype fp_attr =
      new FunctionPrototype("doit", Types.ANY, Types.ATTR_ENV);
    FunctionPrototype fp_node =
      new FunctionPrototype("doit", Types.ANY, Types.NODE);
    FunctionPrototype fp_enode =
      new FunctionPrototype("doit", Types.ANY, Types.ENODE);
    FunctionPrototype fp_tnode =
      new FunctionPrototype("doit", Types.ANY, Types.TNODE);

    assertEquals(0, fp_attr.numPromotionsTo(fp_xml));
    assertEquals(0, fp_enode.numPromotionsTo(fp_node));
    assertEquals(0, fp_enode.numPromotionsTo(fp_xml));
    assertEquals(0, fp_tnode.numPromotionsTo(fp_node));
    assertEquals(0, fp_tnode.numPromotionsTo(fp_xml));

    // Instances of anyclass
    assertEquals(0, fp_attr.numPromotionsTo(fp_any));
    assertEquals(0, fp_enode.numPromotionsTo(fp_any));
    assertEquals(0, fp_tnode.numPromotionsTo(fp_any));

    // Test Number Generics
    FunctionPrototype fp_num =
      new FunctionPrototype("doit", Types.ANY, Types.NUMBER);
    assertEquals(0, fp_int.numPromotionsTo(fp_num));
    assertEquals(0, fp_dec.numPromotionsTo(fp_num));
    assertEquals(FunctionPrototype.CANNOT_PROMOTE,
                 fp_dec.numPromotionsTo(fp_int));

    // Test AnySimpleType
    FunctionPrototype fp_simple =
      new FunctionPrototype("doit", Types.ANY, Types.ANY_SIMPLE);
    FunctionPrototype fp_bool =
      new FunctionPrototype("doit", Types.ANY, Types.BOOLEAN);
    FunctionPrototype fp_str =
      new FunctionPrototype("doit", Types.ANY, Types.STRING);
    
    assertEquals(0, fp_bool.numPromotionsTo(fp_simple));
    assertEquals(0, fp_int.numPromotionsTo(fp_simple));
    assertEquals(0, fp_dec.numPromotionsTo(fp_simple));
    assertEquals(0, fp_str.numPromotionsTo(fp_simple));
    assertEquals(FunctionPrototype.CANNOT_PROMOTE,
                 fp_seq.numPromotionsTo(fp_simple));
  }
  
  protected void tearDown() throws Exception {
    super.tearDown();
  }
}
