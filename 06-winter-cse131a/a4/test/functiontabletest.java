import onyx.xml.error.OnyxXMLException;
import junit.framework.TestCase;
import java.util.Arrays;

public class FunctionTableTest extends TestCase {
  private FunctionTable func_tab;
  
  protected void setUp() throws Exception {
    super.setUp();
    func_tab = new FunctionTable();
  }

  public void testValidArgs() {
    AnyType[] args = { new IntegerType(5), new StringType("hi") };
    
    assertTrue(func_tab.validArgs(args, IntegerType.class, StringType.class));
    assertFalse(func_tab.validArgs(args, BooleanType.class, StringType.class));
    assertFalse(func_tab.validArgs(args, IntegerType.class));
  }
  
  public void testAddFunction() { 
    FunctionPrototype func =
      new FunctionPrototype("lookup", Types.BOOLEAN,
                            new ParamType("$a", Types.ANY),
                            new ParamType("$b", Types.BOOLEAN));
    
    ExprList code = new ExprList(new Constant(Types.BOOLEAN, "true"));
    
    func_tab.add(func, code);
    assertSame(code, func_tab.lookup(func));
    
    FunctionPrototype same_func_prototype =
      new FunctionPrototype("lookup", Types.BOOLEAN,
                            new ParamType("$a", Types.ANY),
                            new ParamType("$b", Types.BOOLEAN));
    
    assertSame(code, func_tab.lookup(same_func_prototype));
  }
  
  public void testBuiltInAddAttr() {
    AnyType[] args = {
        new AttrEnvType(),
        new StringType("name"),
        new StringType("age")
    };
    
    func_tab.builtInAddAttr(args);
    
    // right key, right val
    AttrEnvType a = new AttrEnvType();
    a.add(new StringType("name"), new StringType("age"));
    assertEquals(a, args[0]);
    
    // wrong key, right val
    a = new AttrEnvType();
    a.add(new StringType("namelkofidsfoiu"), new StringType("age"));
    assertFalse(a.equals(args[0]));
    
    // right key, wrong val
    a = new AttrEnvType();
    a.add(new StringType("name"), new StringType("agesfdsfo"));
    assertFalse(a.equals(args[0]));
    
    // wrong key, wrong val
    a = new AttrEnvType();
    a.add(new StringType("nameoiuljodfdisf"), new StringType("agosdiuf"));
    assertFalse(a.equals(args[0]));
   
    // overwriting with new val
    a = new AttrEnvType();
    a.add(new StringType("name"), new StringType("joe"));
    assertFalse(a.equals(args[0]));
    
    args[2] = new StringType("joe");
    func_tab.builtInAddAttr(args);
    assertEquals(a, args[0]);
    
    // xml format error
    a = new AttrEnvType();
    try {
      a.add(new StringType("1"), new StringType("shoulnd't be valid"));
      fail();
    } catch (OnyxXMLException e) {
    }
  }

  public void testBuiltInAddChildNode() {
    ENodeType n = new ENodeType(new StringType("people"));
    TNodeType t = new TNodeType(new StringType("awesome"),
                                new StringType("kev"));
    AnyType[] args = {
      new ENodeType(new StringType("people")),
      new TNodeType(new StringType("awesome"), new StringType("kev"))
    };
    
    // TODO: FIX THIS TEST

    //System.err.println("Before args (in test): " + Arrays.toString(args));
    func_tab.builtInAddChildNode(args);
    //System.err.println("After args (in test): " + Arrays.toString(args));
    
    ENodeType expected = new ENodeType(new StringType("people"));
    expected.add(t);
    
    //System.err.println("expected: " + expected.stringOf());
    //System.err.println("actual: " + n.stringOf());
    //assertEquals(expected, n);
    
    expected = new ENodeType(new StringType("people"));
    
    //assertFalse(expected.equals(n));
  }

  public void testBuiltInAttrEnv() {
    AttrEnvType env = new AttrEnvType();
    env.add(new StringType("k"), new StringType("v"));
    
    assertEquals(new AttrEnvType(), func_tab.builtInAttrEnv(new AnyType[0]));
    assertFalse(env.equals(func_tab.builtInAttrEnv(new AnyType[0])));
  }
  
  public void testBuiltInChildren() {
    ENodeType n = new ENodeType(new StringType("top"));
    AnyType[] args = { n };
    
    SequenceType s = new SequenceType();
    
    assertEquals(s, func_tab.builtInChildren(args));
    
    TNodeType a = new TNodeType(new StringType("awesome"), 
                                new StringType("kev"));
    n.add(a);
    
    args[0] = n;
    
    s = new SequenceType(new TNodeType(new StringType("awesome"),
                                       new StringType("kev")));
   
    assertEquals(s, func_tab.builtInChildren(args));
  }
  
  public void testBuiltInChildrenMore() {
    ENodeType n = new ENodeType(new StringType("top"));
    TNodeType a = new TNodeType(new StringType("my_name"),
                                new StringType("my_content"));
    ENodeType b = new ENodeType(new StringType("blah"));
    n.add(a, b);
    
    AnyType[] args = { n };
    
    SequenceType s =
      new SequenceType(new TNodeType(new StringType("my_name"),
                                     new StringType("my_content")),
                       new ENodeType(new StringType("blah")));
    

    SequenceType result = (SequenceType)func_tab.builtInChildren(args);

    assertEquals(s, func_tab.builtInChildren(args));
  }
  
  public void testBuiltInConcat() {
    StringType s1 = new StringType("jo");
    StringType s2 = new StringType("ab");
    
    StringType[] arr = { s1, s2 };
    
    assertEquals(new StringType("joab"), func_tab.builtInConcat(arr));
    assertFalse(new StringType("abjo").equals(func_tab.builtInConcat(arr)));
  }

  public void testBuiltInDecimal() {
    StringType s = new StringType("3.14");
    IntegerType i = new IntegerType(3);
    DecimalType d = new DecimalType(3.14);
    
    assertEquals(new DecimalType(3.14), func_tab.builtInDecimal(s));
    assertEquals(new DecimalType(3), func_tab.builtInDecimal(i));
    assertEquals(new DecimalType(3.14), func_tab.builtInDecimal(d));
    assertNotSame(d, func_tab.builtInDecimal(d));
    assertFalse((new DecimalType(5)).equals(func_tab.builtInDecimal(d)));
  }
  
  public void testBuiltInENode() {
    StringType s = new StringType("kevs_node");
    AttrEnvType attr = new AttrEnvType();
    attr.add(new StringType("key"), new StringType("value"));
    
    AttrEnvType attr2 = new AttrEnvType();
    attr2.add(new StringType("key"), new StringType("value"));
    
    ENodeType expected = new ENodeType(new StringType("kevs_node"), attr2);
    assertEquals(expected, func_tab.builtInENode(s, attr));
    
    attr2 = new AttrEnvType();
    attr2.add(new StringType("nonkey"), new StringType("nonvalue"));
    
    expected = new ENodeType(new StringType("kevs_node"), attr2);
    assertFalse(expected.equals(func_tab.builtInENode(s, attr)));
  }
  
  public void testBuiltInFirst() {
    SequenceType seq = new SequenceType();
    
    assertEquals(new SequenceType(), func_tab.builtInFirst(seq));
    
    seq.add(new IntegerType(7));
    assertEquals(new IntegerType(7),
                 func_tab.builtInFirst(seq));
    
    seq.add(new DecimalType(3.14));
    assertEquals(new IntegerType(7),
                 func_tab.builtInFirst(seq));
    assertFalse(new SequenceType().equals(func_tab.builtInFirst(seq)));
    assertFalse(new SequenceType(
                  new DecimalType(3.14)).equals(func_tab.builtInFirst(seq)));
    assertFalse(seq.equals(func_tab.builtInFirst(seq)));
    
    seq.add(new StringType("go"));
    assertEquals(new IntegerType(7),
                 func_tab.builtInFirst(seq));
    assertFalse(new SequenceType().equals(func_tab.builtInFirst(seq)));
    assertFalse(seq.equals(func_tab.builtInFirst(seq)));
  }
  
  public void testBuiltInGetAttrEnv() {
    AttrEnvType attr = new AttrEnvType();
    attr.add(new StringType("k"), new StringType("v"));
    
    NodeType e = new ENodeType(new StringType("kev"), attr);
    
    AttrEnvType attr2 = new AttrEnvType();
    attr2.add(new StringType("k"), new StringType("v"));
    assertEquals(attr2, func_tab.builtInGetAttrEnv(e));
    assertFalse(new AttrEnvType().equals(func_tab.builtInGetAttrEnv(e)));
    
    e = new TNodeType(new StringType("nitay"), attr, new StringType("meh"));
    
    assertEquals(attr2, func_tab.builtInGetAttrEnv(e));
    assertFalse(new AttrEnvType().equals(func_tab.builtInGetAttrEnv(e)));
  }
  
  
  public void testBuiltInGetAttrKeys() {
    AttrEnvType a = new AttrEnvType();
    
    assertEquals(new SequenceType(), func_tab.builtInGetAttrKeys(a));
    
    a.add(new StringType("a"), new StringType("b"));
    
    assertEquals(new SequenceType(new StringType("a")),
                 func_tab.builtInGetAttrKeys(a));
    assertFalse(new SequenceType().equals(func_tab.builtInGetAttrKeys(a)));
    assertFalse(new SequenceType(new StringType("b")).equals(
                  func_tab.builtInGetAttrKeys(a)));
   
    a.add(new StringType("c"), new StringType("a"));
    
    assertEquals(new SequenceType(new StringType("a"), new StringType("c")),
                func_tab.builtInGetAttrKeys(a));
    assertFalse(new SequenceType().equals(func_tab.builtInGetAttrKeys(a)));
    assertFalse(new SequenceType(new StringType("a")).equals(
                func_tab.builtInGetAttrKeys(a)));
    assertFalse(new SequenceType(new StringType("c")).equals(
                func_tab.builtInGetAttrKeys(a)));
  }
  
  public void testGetAttrValue() {
    AttrEnvType a = new AttrEnvType();
    assertEquals(new SequenceType(), 
                 func_tab.builtInGetAttrValue(a, new StringType("key")));
    a.add(new StringType("key"), new StringType("value"));
    assertEquals(new StringType("value"),
                 func_tab.builtInGetAttrValue(a, new StringType("key")));
    a.add(new StringType("another_key"), new StringType("another value"));
    assertEquals(new StringType("another value"),
                 func_tab.builtInGetAttrValue(a,new StringType("another_key")));
    assertFalse((new StringType("another value")).equals(
                 func_tab.builtInGetAttrValue(a, new StringType("key"))));
  }
  
  public void testBuiltInInteger() {
    StringType s = new StringType("3");
    IntegerType i = new IntegerType(3);
    DecimalType d = new DecimalType(3.14);
    
    assertEquals(new IntegerType(3), func_tab.builtInInteger(s));
    assertEquals(new IntegerType(3), func_tab.builtInInteger(i));
    assertNotSame(i, func_tab.builtInInteger(i));
    assertEquals(new IntegerType(3), func_tab.builtInInteger(d));
    assertFalse((new IntegerType(5)).equals(func_tab.builtInInteger(d)));
  }
  
  public void testIsENode() {
    assertTrue(((BooleanType)func_tab.builtInIsENode(
                                          new ENodeType())).getValue());
    assertFalse(((BooleanType)func_tab.builtInIsENode(
                                       new StringType("hi there"))).getValue());
    assertFalse(((BooleanType)func_tab.builtInIsENode(
                                       new TNodeType())).getValue());
    
  }
  
  public void testIsNode() {
    assertTrue(((BooleanType)func_tab.builtInIsNode(
                                          new ENodeType())).getValue());
    assertTrue(((BooleanType)func_tab.builtInIsNode(
                                          new TNodeType())).getValue());
    assertFalse(((BooleanType)func_tab.builtInIsNode(
                                       new StringType("hi there"))).getValue());
    
  }
  
  public void testIsTNode() {
    assertTrue(((BooleanType)func_tab.builtInIsTNode(
                                       new TNodeType())).getValue());
    assertFalse(((BooleanType)func_tab.builtInIsTNode(
                                       new StringType("hi there"))).getValue());
    assertFalse(((BooleanType)func_tab.builtInIsTNode(
                                       new ENodeType())).getValue());
    
  }
  
  public void testLength() {
    SequenceType short_seq = new SequenceType();
    SequenceType middle_seq = new SequenceType();
    middle_seq.add(new IntegerType(), new IntegerType());
    SequenceType large_seq = new SequenceType();
    large_seq.add(new IntegerType(), new IntegerType(), new IntegerType());
    
    assertEquals(new IntegerType(0), func_tab.builtInLength(short_seq));
    assertEquals(new IntegerType(2), func_tab.builtInLength(middle_seq));
    assertEquals(new IntegerType(3), func_tab.builtInLength(large_seq));
    assertFalse((new IntegerType(1)).equals(func_tab.builtInLength(short_seq)));
  }
  
  public void testSetAttr() {
    AttrEnvType a = new AttrEnvType();
    ENodeType e = new ENodeType(new StringType("kev"), a);
    AttrEnvType b = new AttrEnvType();
    b.add(new StringType("name"), new StringType("nitay"));
    assertEquals(a, e.getAttr());
    
    func_tab.builtInSetAttrEnv(e, b);
    
    AttrEnvType b_clone = new AttrEnvType();
    b_clone.add(new StringType("name"), new StringType("nitay"));
    assertEquals(b_clone, e.getAttr());
    assertFalse(a.equals(e.getAttr()));
    
    func_tab.builtInSetAttrEnv(e, new AttrEnvType());
    assertEquals(new AttrEnvType(), e.getAttr());
  }
  
  public void testBuiltInString() {
    AnySimpleType a = new IntegerType(4);
    assertEquals(new StringType("4"), func_tab.builtInString(a));
    
    a = new DecimalType(3.14);
    assertEquals(new StringType("3.14"), func_tab.builtInString(a));
    assertFalse(new StringType("4").equals(func_tab.builtInString(a)));
    
    a = new BooleanType(false);
    assertEquals(new StringType("false"), func_tab.builtInString(a));
    assertFalse(new StringType("3.14").equals(func_tab.builtInString(a)));
    
    a = new StringType("yo joe");
    assertEquals(new StringType("yo joe"), func_tab.builtInString(a));
    assertFalse(new StringType("boo").equals(func_tab.builtInString(a)));
    
    SequenceType seq =
      new SequenceType(new IntegerType(4), new DecimalType(3.14));
    
    assertEquals(new StringType("[4 3.14]"), func_tab.builtInString(seq));
    
    seq =
      new SequenceType(new DecimalType(3.14), new IntegerType(4),
                       new StringType("blah"), new BooleanType(true));
    
    assertEquals(new StringType("[3.14 4 blah true]"),
                 func_tab.builtInString(seq));
    
    AttrEnvType attr = new AttrEnvType();
    attr.add(new StringType("key"), new StringType("value"));
    TNodeType t = new TNodeType(new StringType("my_name"), attr,
                                new StringType("contento"));
    assertEquals(new StringType("contento"), func_tab.builtInString(t));
  }

  public void testBuiltInTagName() {
    NodeType n = new ENodeType(new StringType("joe")); 
    assertEquals(new StringType("joe"), func_tab.builtInTagname(n));
    assertFalse(new StringType("bob").equals(func_tab.builtInTagname(n)));
    
    n = new TNodeType(new StringType("bob"), new StringType("bob's stuff"));
    assertEquals(new StringType("bob"), func_tab.builtInTagname(n));
    assertFalse(new StringType("joe").equals(func_tab.builtInTagname(n)));
  }
  
  public void testBuiltInTail() {
    SequenceType seq = new SequenceType();
    assertEquals(new SequenceType(), func_tab.builtInTail(seq));
    
    seq.add(new IntegerType(4));
    assertEquals(new SequenceType(), func_tab.builtInTail(seq));
    assertFalse(new SequenceType(new IntegerType(4)).equals(
                          func_tab.builtInTail(seq)));
    
    seq.add(new DecimalType(3.14));
    assertEquals(new SequenceType(new DecimalType(3.14)),
                 func_tab.builtInTail(seq));
    assertFalse(new SequenceType().equals(func_tab.builtInTail(seq)));
    assertFalse(new SequenceType(new IntegerType(4)).equals(
                          func_tab.builtInTail(seq)));
    
    seq.add(new StringType("q"));
    assertEquals(new SequenceType(new DecimalType(3.14), new StringType("q")),
                 func_tab.builtInTail(seq));
    assertFalse(new SequenceType().equals(func_tab.builtInTail(seq)));
    assertFalse(new SequenceType(new IntegerType(4)).equals(
                func_tab.builtInTail(seq)));
  }
  
  public void testGetBodyAndArgs() {
    ParamType[] args = { new ParamType("$a", Types.BOOLEAN),
                         new ParamType("$b", Types.BOOLEAN) };
    
    FunctionPrototype fp = new FunctionPrototype("tf", args);
    
    Variable v1 = new Variable("$a");
    Variable v2 = new Variable("$b");
    Operator op = new Operator(Ops.AND, v1, v2);
    
    ExprList code = new ExprList(op);
    
    assertNull(func_tab.lookup(fp));
    func_tab.add(fp, code);
    assertEquals(new ExprList(op), func_tab.lookup(fp));
  }
  
  public void testBuiltInPrefixPrototypes() {
    // functions matching is* taking one AnyType arg -> isNode, isENode, isTNode
    ParamType[] args = { new ParamType("fsdfs", Types.ANY) };
    FunctionPrototype fp = new FunctionPrototype("is", args);
    FunctionPrototype[] res =
      func_tab.getAllPrefixPrototypes(fp).toArray(new FunctionPrototype[0]);
    
    FunctionPrototype[] exp_fp = {
        new FunctionPrototype(Funcs.IS_NODE, args),
        new FunctionPrototype(Funcs.IS_ENODE, args),
        new FunctionPrototype(Funcs.IS_TNODE, args),
    };
    
    assertTrue(Arrays.equals(exp_fp, res));
    
    // matches for get* taking one AttrEnvType arg -> getAttrKeys
    args[0] = new ParamType("sldfkjds", Types.ATTR_ENV);
    
    fp = new FunctionPrototype("get", args);
    
    res = func_tab.getAllPrefixPrototypes(fp).toArray(new FunctionPrototype[0]);
    
    FunctionPrototype[] exp = {
        new FunctionPrototype(Funcs.GET_ATTR_KEYS, args),
    };
    
    assertTrue(Arrays.equals(exp, res));
  }

  public void testGetMatchingPrototype() {
    
    // TODO: create our own functions... test prefix matching stuff
  }

  public void testGetClosestPrototype() {    
    ParamType[] args = { new ParamType("blsdlfk", Types.ATTR_ENV),
                         new ParamType("sdofids", Types.STRING),
                         new ParamType("oicjvxz", Types.STRING)     };
    FunctionPrototype old_fp =
        new FunctionPrototype(Funcs.ADD_ATTR, args);

    // AddAttribute actual function prototype
    ParamType[] a = { new ParamType("$arg0", Types.ATTR_ENV),
                      new ParamType("$arg1", Types.STRING),
                      new ParamType("$arg2", Types.STRING)     };    
    FunctionPrototype exp_fp =
        new FunctionPrototype(Funcs.ADD_ATTR, Types.ATTR_ENV, a);

    FunctionPrototype new_fp = func_tab.getClosestPrototype(old_fp);

    assertFalse(exp_fp.exactlyMatches(old_fp));
    assertTrue(exp_fp.exactlyMatches(new_fp));
  }

  protected void tearDown() throws Exception {
    super.tearDown();
  }
}
