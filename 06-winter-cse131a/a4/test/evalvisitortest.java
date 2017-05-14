import java.math.BigDecimal;
import java.math.BigInteger;

import junit.framework.TestCase;

public class EvalVisitorTest extends TestCase {
  private EvalVisitor visitor;

  public void setUp() {
    visitor = new EvalVisitor();
  }

  // Testing Variables
  public void testVariableDeclarationTypes() {
    Constant a = new Constant(Types.INTEGER, "4");
    VariableDeclaration vd = new VariableDeclaration("$aa");
    vd.addChild(a);

    // TODO: implement
    
    // visitor.visit(new VariableDeclaration("aa"));
    // String [] varTypes = {
    // OnyxTypes.TNODE,
    // OnyxTypes.ENODE,
    // OnyxTypes.ATTR_ENV,
    // OnyxTypes.INTEGER,
    // OnyxTypes.DECIMAL,
    // OnyxTypes.BOOLEAN,
    // OnyxTypes.STRING,
    // OnyxTypes.SEQUENCE,
    // };
    //    
    // for (String var : varTypes) {
    // visitor.visit(new VariableDeclaration(var, var));
    //      
    // SymbolType st = visitor.currentScope().lookup(var);
    // assertNotNull(st);
    // assertEquals(TypeFactory.onyxTypeByName(var).getClass(), st.getClass());
    // }

    // TODO: test multiple insertions of variables with same name
  }

  // Testing Constant evals
  public void testStringConstant() {
    Constant c = new Constant(Types.STRING, "hello");
    visitor.visit(c);
    try {
      StringType root = (StringType) visitor.peek();
      assertEquals(c.getValue(), root.getValue());
    } catch (ClassCastException e) {
      fail();
    }
  }

  public void testDecimalConstant() {
    Constant c = new Constant(Types.DECIMAL, "3.14159");
    visitor.visit(c);
    try {
      DecimalType root = (DecimalType) visitor.peek();
      assertEquals(new BigDecimal(c.getValue()), root.getValue());
    } catch (ClassCastException e) {
      fail();
    }
  }

  public void testBooleanConstant() {
    Constant c = new Constant(Types.BOOLEAN, "true");
    visitor.visit(c);
    try {
      BooleanType root = (BooleanType) visitor.peek();
      assertEquals(new Boolean(c.getValue()), root.getValue());
    } catch (ClassCastException e) {
      fail();
    }
  }

  public void testIntegerConstant() {
    Constant c = new Constant(Types.INTEGER, "5");
    visitor.visit(c);
    try {
      IntegerType root = (IntegerType) visitor.peek();
      assertEquals(new BigInteger(c.getValue()), root.getValue());
    } catch (ClassCastException e) {
      fail();
    }
  }

  // Test IfThenElseExpr
  public void testIfThenElseExpr() {
    Constant ifs = new Constant(Types.BOOLEAN, "true");
    Constant then = new Constant(Types.DECIMAL, "3.14");
    Constant els = new Constant(Types.DECIMAL, "2.71");

    IfThenElseExpr iftee = new IfThenElseExpr(ifs, then, els);
    iftee.accept(visitor);
    DecimalType root = (DecimalType) visitor.peek();
    assertEquals(new BigDecimal("3.14"), root.getValue());

    ifs = new Constant(Types.BOOLEAN, "false");
    iftee = new IfThenElseExpr(ifs, then, els);
    iftee.accept(visitor);
    root = (DecimalType) visitor.peek();
    assertEquals(new BigDecimal("2.71"), root.getValue());
  }

  public void testEmptyExprList() {
    visitor.visit(new ExprList());
    try {
      assertTrue(((SequenceType) visitor.peek()).isEmpty());
    } catch (ClassCastException e) {
      fail();
    }
  }

  public void testExprList() {
    Constant first_born = new Constant(Types.INTEGER, "3");
    Constant middle_child = new Constant(Types.BOOLEAN, "false");
    Constant baby = new Constant(Types.STRING, "fetus");
    Constant unborn = new Constant(Types.DECIMAL, "-2.71");

    visitor.visit(new ExprList(first_born, middle_child, baby, unborn));

    try {
      SequenceType seq = (SequenceType) visitor.peek();
      assertEquals("[3, false, fetus, -2.71]", seq.toString());
    } catch (ClassCastException e) {
      fail();
    }
  }

  public void testNestedExprList() {
    Constant first_born = new Constant(Types.INTEGER, "27");
    Constant age = new Constant(Types.INTEGER, "99");
    Constant num_feet = new Constant(Types.DECIMAL, "2.5");
    ExprList messed_up_child = new ExprList(age, num_feet);

    Constant baby = new Constant(Types.BOOLEAN, "false");

    visitor.visit(new ExprList(first_born, messed_up_child, baby));

    try {
      SequenceType seq = (SequenceType) visitor.peek();
      assertEquals(4, seq.size());
      assertEquals("[27, 99, 2.5, false]", seq.toString());
    } catch (ClassCastException e) {
      fail();
    }
  }

  public void testOpNumericAdd() {
    // Test simple integer addition
    Constant left = new Constant(Types.INTEGER, "42");
    Constant right = new Constant(Types.INTEGER, "2");

    visitor.visit(new Operator(Ops.ADD, left, right));
    assertFalse((new IntegerType("6").equals(visitor.peek())));
    assertEquals(new IntegerType("44"), visitor.peek());

    // Test type promotion for left
    right = new Constant(Types.DECIMAL, "3.14");

    visitor.visit(new Operator(Ops.ADD, left, right));
    assertEquals(new DecimalType("45.14"), visitor.peek());

    // Test type promotion for right
    left = new Constant(Types.DECIMAL, "2.71");
    right = new Constant(Types.INTEGER, "3");

    visitor.visit(new Operator(Ops.ADD, left, right));
    assertEquals(new DecimalType("5.71"), visitor.peek());

    // Test decimal addition
    right = new Constant(Types.DECIMAL, "3.1");

    visitor.visit(new Operator(Ops.ADD, left, right));
    assertFalse((new DecimalType("5.85")).equals(visitor.peek()));
    assertEquals(new DecimalType("5.81"), visitor.peek());

    // Test string addition
    left = new Constant(Types.STRING, "test");
    right = new Constant(Types.STRING, " always");

    visitor.visit(new Operator(Ops.ADD, left, right));
    assertEquals(new StringType("test always"), visitor.peek());
    assertFalse((new StringType("noooooooooo").equals(visitor.peek())));
  }

  public void testOpNumericSubtract() {
    // Test simple integer addition
    Constant left = new Constant(Types.INTEGER, "42");
    Constant right = new Constant(Types.INTEGER, "2");

    visitor.visit(new Operator(Ops.SUBTRACT, left, right));
    assertFalse((new IntegerType(6).equals(visitor.peek())));
    assertEquals(new IntegerType(40), visitor.peek());

    // Test type promotion for left
    right = new Constant(Types.DECIMAL, "3.14");

    visitor.visit(new Operator(Ops.SUBTRACT, left, right));
    assertEquals(new DecimalType(38.86), visitor.peek());
    assertFalse((new DecimalType(45.00)).equals(visitor.peek()));

    // Test type promotion for right
    left = new Constant(Types.DECIMAL, "2.71");
    right = new Constant(Types.INTEGER, "3");

    visitor.visit(new Operator(Ops.SUBTRACT, left, right));
    assertEquals(new DecimalType(-0.29), visitor.peek());
    assertFalse((new DecimalType(5)).equals(visitor.peek()));

    // Test decimal addition
    right = new Constant(Types.DECIMAL, "3.1");

    visitor.visit(new Operator(Ops.SUBTRACT, left, right));
    assertFalse((new DecimalType(55)).equals(visitor.peek()));
    assertEquals(new DecimalType(-0.39), visitor.peek());
  }

  public void testOpumericMultiply() {
    // Test simple integer multiplication
    Constant left = new Constant(Types.INTEGER, "42");
    Constant right = new Constant(Types.INTEGER, "2");

    visitor.visit(new Operator(Ops.MULTIPLY, left, right));
    assertFalse((new IntegerType(6).equals(visitor.peek())));
    assertEquals(new IntegerType(84), visitor.peek());

    // Test type promotion for left
    right = new Constant(Types.DECIMAL, "3.14");

    visitor.visit(new Operator(Ops.MULTIPLY, left, right));
    assertEquals(new DecimalType(131.88), visitor.peek());
    assertFalse((new DecimalType(10.00)).equals(visitor.peek()));

    // Test type promotion for right
    left = new Constant(Types.DECIMAL, "2.71");
    right = new Constant(Types.INTEGER, "3");

    visitor.visit(new Operator(Ops.MULTIPLY, left, right));
    assertEquals(new DecimalType(8.13), visitor.peek());
    assertFalse((new DecimalType(5)).equals(visitor.peek()));

    // Test decimal multiplication
    right = new Constant(Types.DECIMAL, "3.1");

    visitor.visit(new Operator(Ops.MULTIPLY, left, right));
    assertFalse((new DecimalType(55)).equals(visitor.peek()));
    assertEquals(new DecimalType(8.401), visitor.peek());
  }

  public void testOpNumericDivide() {
    // Test simple integer multiplication
    Constant left = new Constant(Types.INTEGER, "42");
    Constant right = new Constant(Types.INTEGER, "5");

    visitor.visit(new Operator(Ops.DIVIDE, left, right));
    assertFalse((new DecimalType(6).equals(visitor.peek())));

    // Test type promotion for left
    right = new Constant(Types.DECIMAL, "0.8");

    visitor.visit(new Operator(Ops.DIVIDE, left, right));
    // division result is 52.5, but we should get 52 because of scaling
    assertEquals(new DecimalType("52"), visitor.peek());
    //assertEquals(new DecimalType(52.5), visitor.peek());
    assertFalse((new DecimalType(10.00)).equals(visitor.peek()));

    // Test type promotion for right
    left = new Constant(Types.DECIMAL, "4.5");
    right = new Constant(Types.INTEGER, "2");

    visitor.visit(new Operator(Ops.DIVIDE, left, right));
    // division result is 2.25, but we should get 2.2 because of scaling
    assertEquals(new DecimalType("2.2"), visitor.peek());
    //assertEquals(new DecimalType(2.25), visitor.peek());
    assertFalse((new DecimalType(5)).equals(visitor.peek()));

    // Test decimal multiplication
    right = new Constant(Types.DECIMAL, "1.0");

    visitor.visit(new Operator(Ops.DIVIDE, left, right));
    assertFalse((new DecimalType(55)).equals(visitor.peek()));
    assertEquals(new DecimalType(4.5), visitor.peek());
  }

  public void testOpNumericIntegerDivide() {
    // Positive test
    Constant left = new Constant(Types.INTEGER, "2");
    Constant right = new Constant(Types.INTEGER, "4");

    visitor.visit(new Operator(Ops.INTEGER_DIVIDE, left, right));
    assertEquals(new IntegerType(0), visitor.peek());
    assertFalse((new DecimalType(0.00)).equals(visitor.peek()));
    assertFalse((new IntegerType(5)).equals(visitor.peek()));

    // Test with decimal args
    right = new Constant(Types.DECIMAL, "4.5");

    try {
      visitor.visit(new Operator(Ops.INTEGER_DIVIDE, left, right));
      fail();
    } catch (RuntimeException e) {
      assertEquals("Function with prototype " + Ops.INTEGER_DIVIDE + "("
                   + Types.INTEGER + "," + Types.DECIMAL + ") not found.",
                   e.getMessage());
    }
  }

  public void testOpNumericMod() {
    // Positive test
    Constant left = new Constant(Types.INTEGER, "2");
    Constant right = new Constant(Types.INTEGER, "4");

    visitor.visit(new Operator(Ops.MOD, left, right));
    assertEquals(new IntegerType(2), visitor.peek());
    assertFalse((new DecimalType(2.00)).equals(visitor.peek()));
    assertFalse((new IntegerType(5)).equals(visitor.peek()));

    // Test with decimal args
    right = new Constant(Types.DECIMAL, "4.5");

    try {
      visitor.visit(new Operator(Ops.INTEGER_DIVIDE, left, right));
      fail();
    } catch (RuntimeException e) {
    }
  }

  public void testOpUnaryPlus() {
    // Function just pushes arg back on stack
    // TODO: test type checking
    Constant one = new Constant(Types.INTEGER, "1");

    visitor.visit(new Operator(Ops.UNARY_PLUS, one));
    assertEquals(new IntegerType(1), visitor.peek());
    assertFalse((new IntegerType(5)).equals(visitor.peek()));

    one = new Constant(Types.DECIMAL, "1.1");

    visitor.visit(new Operator(Ops.UNARY_PLUS, one));
    assertEquals(new DecimalType(1.1), visitor.peek());
    assertFalse((new DecimalType(5.0)).equals(visitor.peek()));
  }

  public void testOpUnaryMinus() {
    Constant a = new Constant(Types.INTEGER, "1");

    visitor.visit(new Operator(Ops.UNARY_MINUS, a));
    assertEquals(new IntegerType(-1), visitor.peek());
    assertFalse((new IntegerType(1)).equals(visitor.peek()));

    a = new Constant(Types.DECIMAL, "1.5");

    visitor.visit(new Operator(Ops.UNARY_MINUS, a));
    assertEquals(new DecimalType(-1.5), visitor.peek());
    assertFalse((new DecimalType(1.5)).equals(visitor.peek()));
  }

  public void testOpTo() {
    Constant one = new Constant(Types.INTEGER, "1");
    Constant ten = new Constant(Types.INTEGER, "10");

    visitor.visit(new Operator(Ops.TO, one, ten));
    SequenceType s = SequenceType
        .range(new IntegerType(1), new IntegerType(10));
    assertEquals(s, visitor.peek());
    assertFalse((new DecimalType("1.5")).equals(visitor.peek()));
  }

  public void testOpAnd() {
    // a is true, b is false
    Constant a = new Constant(Types.BOOLEAN, "true");
    Constant b = new Constant(Types.BOOLEAN, "false");

    visitor.visit(new Operator(Ops.AND, a, b));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // a is true, b is true
    b = new Constant(Types.BOOLEAN, "true");

    visitor.visit(new Operator(Ops.AND, a, b));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // a is false, b is true
    a = new Constant(Types.BOOLEAN, "false");

    visitor.visit(new Operator(Ops.AND, a, b));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // a is false, b is false
    b = new Constant(Types.BOOLEAN, "false");

    visitor.visit(new Operator(Ops.AND, a, b));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));
  }

  public void testOpOr() {
    // a is true, b is false
    Constant a = new Constant(Types.BOOLEAN, "true");
    Constant b = new Constant(Types.BOOLEAN, "false");

    visitor.visit(new Operator(Ops.OR, a, b));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // a is true, b is true
    b = new Constant(Types.BOOLEAN, "true");

    visitor.visit(new Operator(Ops.OR, a, b));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // a is false, b is true
    a = new Constant(Types.BOOLEAN, "false");

    visitor.visit(new Operator(Ops.OR, a, b));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // a is false, b is false
    b = new Constant(Types.BOOLEAN, "false");

    visitor.visit(new Operator(Ops.OR, a, b));

    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));
  }

  public void testOpEquals() {
    Constant b1 = new Constant(Types.BOOLEAN, "true");
    Constant b2 = new Constant(Types.BOOLEAN, "false");
    Constant b3 = new Constant(Types.BOOLEAN, "true");

    // different booleans
    visitor.visit(new Operator(Ops.EQUALS, b1, b2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // same booleans
    visitor.visit(new Operator(Ops.EQUALS, b1, b3));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    Constant i1 = new Constant(Types.INTEGER, "14");
    Constant i2 = new Constant(Types.INTEGER, "-25");
    Constant i3 = new Constant(Types.INTEGER, "14");

    // different integers
    visitor.visit(new Operator(Ops.EQUALS, i1, i2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // same integers
    visitor.visit(new Operator(Ops.EQUALS, i1, i3));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    Constant d1 = new Constant(Types.DECIMAL, "2.71");
    Constant d2 = new Constant(Types.DECIMAL, "3.14");
    Constant d3 = new Constant(Types.DECIMAL, "2.71");

    // different decimals
    visitor.visit(new Operator(Ops.EQUALS, d1, d2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // same decimals
    visitor.visit(new Operator(Ops.EQUALS, d1, d3));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    Constant s1 = new Constant(Types.STRING, "your mom");
    Constant s2 = new Constant(Types.STRING, "is not tested");
    Constant s3 = new Constant(Types.STRING, "your mom");

    // different strings
    visitor.visit(new Operator(Ops.EQUALS, s1, s2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // same strings
    visitor.visit(new Operator(Ops.EQUALS, s1, s3));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // boolean and integer
    try {
      visitor.visit(new Operator(Ops.EQUALS, b1, i1));
      fail();
    } catch (RuntimeException e) {
    }

    // decimal and string
    try {
      visitor.visit(new Operator(Ops.EQUALS, d1, s1));
      fail();
    } catch (RuntimeException e) {
    }
  }

  public void testOpNotEquals() {
    Constant b1 = new Constant(Types.BOOLEAN, "true");
    Constant b2 = new Constant(Types.BOOLEAN, "false");
    Constant b3 = new Constant(Types.BOOLEAN, "true");

    // different booleans
    visitor.visit(new Operator(Ops.NOT_EQUALS, b1, b2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // same booleans
    visitor.visit(new Operator(Ops.NOT_EQUALS, b1, b3));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    Constant i1 = new Constant(Types.INTEGER, "14");
    Constant i2 = new Constant(Types.INTEGER, "-25");
    Constant i3 = new Constant(Types.INTEGER, "14");

    // different integers
    visitor.visit(new Operator(Ops.NOT_EQUALS, i1, i2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // same integers
    visitor.visit(new Operator(Ops.NOT_EQUALS, i1, i3));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    Constant d1 = new Constant(Types.DECIMAL, "2.71");
    Constant d2 = new Constant(Types.DECIMAL, "3.14");
    Constant d3 = new Constant(Types.DECIMAL, "2.71");

    // different decimals
    visitor.visit(new Operator(Ops.NOT_EQUALS, d1, d2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // same decimals
    visitor.visit(new Operator(Ops.NOT_EQUALS, d1, d3));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    Constant s1 = new Constant(Types.STRING, "your mom");
    Constant s2 = new Constant(Types.STRING, "is not tested");
    Constant s3 = new Constant(Types.STRING, "your mom");

    // different strings
    visitor.visit(new Operator(Ops.NOT_EQUALS, s1, s2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // same strings
    visitor.visit(new Operator(Ops.NOT_EQUALS, s1, s3));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // boolean and integer
    try {
      visitor.visit(new Operator(Ops.NOT_EQUALS, b1, i1));
      fail();
    } catch (RuntimeException e) {
    }

    // decimal and string
    try {
      visitor.visit(new Operator(Ops.NOT_EQUALS, d1, s1));
      fail();
    } catch (RuntimeException e) {
    }
  }

  public void testOpLessThan() {
    Constant i1 = new Constant(Types.INTEGER, "14");
    Constant i2 = new Constant(Types.INTEGER, "38");

    // i1 < i2
    visitor.visit(new Operator(Ops.LESS_THAN, i1, i2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // i1 == i2
    i1 = new Constant(Types.INTEGER, "38");

    visitor.visit(new Operator(Ops.LESS_THAN, i1, i2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // i1 > i2
    i1 = new Constant(Types.INTEGER, "45");

    visitor.visit(new Operator(Ops.LESS_THAN, i1, i2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // d1 < d2
    Constant d1 = new Constant(Types.DECIMAL, "2.71");
    Constant d2 = new Constant(Types.DECIMAL, "3.14");

    visitor.visit(new Operator(Ops.LESS_THAN, d1, d2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // d1 == d2
    d1 = new Constant(Types.DECIMAL, "3.14");

    visitor.visit(new Operator(Ops.LESS_THAN, d1, d2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // d1 > d2
    d1 = new Constant(Types.DECIMAL, "5.34");

    visitor.visit(new Operator(Ops.LESS_THAN, d1, d2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // d1 < i1
    visitor.visit(new Operator(Ops.LESS_THAN, d1, i1));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // i2 > d2
    visitor.visit(new Operator(Ops.LESS_THAN, i2, d2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // s1 < s2
    Constant s1 = new Constant(Types.STRING, "abc");
    Constant s2 = new Constant(Types.STRING, "def");

    visitor.visit(new Operator(Ops.LESS_THAN, s1, s2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // s1 == s2
    s1 = new Constant(Types.STRING, "def");

    visitor.visit(new Operator(Ops.LESS_THAN, s1, s2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // s1 > s2
    s1 = new Constant(Types.STRING, "ghi");

    visitor.visit(new Operator(Ops.LESS_THAN, s1, s2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // boolean and boolean
    Constant b1 = new Constant(Types.BOOLEAN, "false");
    try {
      visitor.visit(new Operator(Ops.LESS_THAN, b1, b1));
      fail();
    } catch (RuntimeException e) {
    }

    // boolean and integer
    try {
      visitor.visit(new Operator(Ops.LESS_THAN, b1, i1));
      fail();
    } catch (RuntimeException e) {
    }

    // decimal and string
    try {
      visitor.visit(new Operator(Ops.LESS_THAN, d1, s1));
      fail();
    } catch (RuntimeException e) {
    }
  }

  public void testOpGreaterThan() {
    Constant i1 = new Constant(Types.INTEGER, "14");
    Constant i2 = new Constant(Types.INTEGER, "38");

    // i1 < i2
    visitor.visit(new Operator(Ops.GREATER_THAN, i1, i2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // i1 == i2
    i1 = new Constant(Types.INTEGER, "38");

    visitor.visit(new Operator(Ops.GREATER_THAN, i1, i2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // i1 > i2
    i1 = new Constant(Types.INTEGER, "45");

    visitor.visit(new Operator(Ops.GREATER_THAN, i1, i2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // d1 < d2
    Constant d1 = new Constant(Types.DECIMAL, "2.71");
    Constant d2 = new Constant(Types.DECIMAL, "3.14");

    visitor.visit(new Operator(Ops.GREATER_THAN, d1, d2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // d1 == d2
    d1 = new Constant(Types.DECIMAL, "3.14");

    visitor.visit(new Operator(Ops.GREATER_THAN, d1, d2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // d1 > d2
    d1 = new Constant(Types.DECIMAL, "5.34");

    visitor.visit(new Operator(Ops.GREATER_THAN, d1, d2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // d1 < i1
    visitor.visit(new Operator(Ops.GREATER_THAN, d1, i1));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // i2 > d2
    visitor.visit(new Operator(Ops.GREATER_THAN, i2, d2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // s1 < s2
    Constant s1 = new Constant(Types.STRING, "abc");
    Constant s2 = new Constant(Types.STRING, "def");

    visitor.visit(new Operator(Ops.GREATER_THAN, s1, s2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // s1 == s2
    s1 = new Constant(Types.STRING, "def");

    visitor.visit(new Operator(Ops.GREATER_THAN, s1, s2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // s1 > s2
    s1 = new Constant(Types.STRING, "ghi");

    visitor.visit(new Operator(Ops.GREATER_THAN, s1, s2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // boolean and boolean
    Constant b1 = new Constant(Types.BOOLEAN, "false");
    try {
      visitor.visit(new Operator(Ops.GREATER_THAN, b1, b1));
      fail();
    } catch (RuntimeException e) {
    }

    // boolean and integer
    try {
      visitor.visit(new Operator(Ops.GREATER_THAN, b1, i1));
      fail();
    } catch (RuntimeException e) {
    }

    // decimal and string
    try {
      visitor.visit(new Operator(Ops.GREATER_THAN, d1, s1));
      fail();
    } catch (RuntimeException e) {
    }
  }

  public void testOpLessEquals() {
    Constant i1 = new Constant(Types.INTEGER, "14");
    Constant i2 = new Constant(Types.INTEGER, "38");

    // i1 < i2
    visitor.visit(new Operator(Ops.LESS_EQUALS, i1, i2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // i1 == i2
    i1 = new Constant(Types.INTEGER, "38");

    visitor.visit(new Operator(Ops.LESS_EQUALS, i1, i2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // i1 > i2
    i1 = new Constant(Types.INTEGER, "45");

    visitor.visit(new Operator(Ops.LESS_EQUALS, i1, i2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // d1 < d2
    Constant d1 = new Constant(Types.DECIMAL, "2.71");
    Constant d2 = new Constant(Types.DECIMAL, "3.14");

    visitor.visit(new Operator(Ops.LESS_EQUALS, d1, d2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // d1 == d2
    d1 = new Constant(Types.DECIMAL, "3.14");

    visitor.visit(new Operator(Ops.LESS_EQUALS, d1, d2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // d1 > d2
    d1 = new Constant(Types.DECIMAL, "5.34");

    visitor.visit(new Operator(Ops.LESS_EQUALS, d1, d2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // d1 < i1
    visitor.visit(new Operator(Ops.LESS_EQUALS, d1, i1));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // i2 > d2
    visitor.visit(new Operator(Ops.LESS_EQUALS, i2, d2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // s1 < s2
    Constant s1 = new Constant(Types.STRING, "abc");
    Constant s2 = new Constant(Types.STRING, "def");

    visitor.visit(new Operator(Ops.LESS_EQUALS, s1, s2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // s1 == s2
    s1 = new Constant(Types.STRING, "def");

    visitor.visit(new Operator(Ops.LESS_EQUALS, s1, s2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // s1 > s2
    s1 = new Constant(Types.STRING, "ghi");

    visitor.visit(new Operator(Ops.LESS_EQUALS, s1, s2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // boolean and boolean
    Constant b1 = new Constant(Types.BOOLEAN, "false");
    try {
      visitor.visit(new Operator(Ops.LESS_EQUALS, b1, b1));
      fail();
    } catch (RuntimeException e) {
    }

    // boolean and integer
    try {
      visitor.visit(new Operator(Ops.LESS_EQUALS, b1, i1));
      fail();
    } catch (RuntimeException e) {
    }

    // decimal and string
    try {
      visitor.visit(new Operator(Ops.LESS_EQUALS, d1, s1));
      fail();
    } catch (RuntimeException e) {
    }
  }

  public void testOpGreaterEquals() {
    Constant i1 = new Constant(Types.INTEGER, "14");
    Constant i2 = new Constant(Types.INTEGER, "38");

    // i1 < i2
    visitor.visit(new Operator(Ops.GREATER_EQUALS, i1, i2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // i1 == i2
    i1 = new Constant(Types.INTEGER, "38");

    visitor.visit(new Operator(Ops.GREATER_EQUALS, i1, i2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // i1 > i2
    i1 = new Constant(Types.INTEGER, "45");

    visitor.visit(new Operator(Ops.GREATER_EQUALS, i1, i2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // d1 < d2
    Constant d1 = new Constant(Types.DECIMAL, "2.71");
    Constant d2 = new Constant(Types.DECIMAL, "3.14");

    visitor.visit(new Operator(Ops.GREATER_EQUALS, d1, d2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // d1 == d2
    d1 = new Constant(Types.DECIMAL, "3.14");

    visitor.visit(new Operator(Ops.GREATER_EQUALS, d1, d2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // d1 > d2
    d1 = new Constant(Types.DECIMAL, "5.34");

    visitor.visit(new Operator(Ops.GREATER_EQUALS, d1, d2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // d1 < i1
    visitor.visit(new Operator(Ops.GREATER_EQUALS, d1, i1));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // i2 > d2
    visitor.visit(new Operator(Ops.GREATER_EQUALS, i2, d2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // s1 < s2
    Constant s1 = new Constant(Types.STRING, "abc");
    Constant s2 = new Constant(Types.STRING, "def");

    visitor.visit(new Operator(Ops.GREATER_EQUALS, s1, s2));
    assertEquals(new BooleanType(false), visitor.peek());
    assertFalse(new BooleanType(true).equals(visitor.peek()));

    // s1 == s2
    s1 = new Constant(Types.STRING, "def");

    visitor.visit(new Operator(Ops.GREATER_EQUALS, s1, s2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // s1 > s2
    s1 = new Constant(Types.STRING, "ghi");

    visitor.visit(new Operator(Ops.GREATER_EQUALS, s1, s2));
    assertEquals(new BooleanType(true), visitor.peek());
    assertFalse(new BooleanType(false).equals(visitor.peek()));

    // boolean and boolean
    Constant b1 = new Constant(Types.BOOLEAN, "false");
    try {
      visitor.visit(new Operator(Ops.GREATER_EQUALS, b1, b1));
      fail();
    } catch (RuntimeException e) {
    }

    // boolean and integer
    try {
      visitor.visit(new Operator(Ops.GREATER_EQUALS, b1, i1));
      fail();
    } catch (RuntimeException e) {
    }

    // decimal and string
    try {
      visitor.visit(new Operator(Ops.GREATER_EQUALS, d1, s1));
      fail();
    } catch (RuntimeException e) {
    }
  }

  public void testArgumentDeclaration() {
    ArgumentDeclaration ad = new ArgumentDeclaration("name", Types.BOOLEAN);
    visitor.visit(ad);
    assertEquals(new ParamType("name", Types.BOOLEAN), visitor.peek());
    assertFalse(new ParamType("name", Types.ANY).equals(visitor.peek()));

    ad = new ArgumentDeclaration("new_name");
    visitor.visit(ad);
    assertEquals(new ParamType("new_name", Types.ANY), visitor.peek());
    assertFalse(new ParamType("name", Types.BOOLEAN).equals(visitor
        .peek()));
  }

  public void testBuiltInFunctionCall() {
    FunctionCall fc = new FunctionCall(Funcs.FIRST);

    Constant a = new Constant(Types.INTEGER, "5");
    Constant b = new Constant(Types.DECIMAL, "1.1");
    Constant c = new Constant(Types.STRING, "hi");
    ExprList l = new ExprList(a, b, c);
    fc.addChild(l);

    visitor.visit(fc);

    IntegerType expected = new IntegerType(5);
    assertEquals(expected, visitor.peek());
    assertFalse((new StringType("hi")).equals(visitor.peek()));

    // test with no elements
    fc = new FunctionCall(Funcs.FIRST);
    fc.addChild(new ExprList());

    visitor.visit(fc);

    assertEquals(new SequenceType(), visitor.peek());
    assertNotNull(visitor.peek());

    FunctionCall attr_call = new FunctionCall(Funcs.ATTR_ENV);
    FunctionCall add_attr = new FunctionCall(Funcs.ADD_ATTR);
    Constant key = new Constant(Types.STRING, "key");
    Constant value = new Constant(Types.STRING, "value");

    add_attr.addChildren(attr_call, key, value);

    visitor.visit(add_attr);

    AttrEnvType exp_attr = new AttrEnvType();
    exp_attr.add(new StringType("key"), new StringType("value"));
    assertEquals(exp_attr, visitor.peek());

    fc = new FunctionCall("NOT IN TABLE");
    try {
      visitor.visit(fc);
      fail();
    } catch (RuntimeException e) {
    }
  }

  public void testFunctionDeclaration() {
    ArgumentDeclaration a1 = new ArgumentDeclaration("$a", Types.INTEGER);
    ArgumentDeclaration a2 = new ArgumentDeclaration("$b", Types.INTEGER);
    ExprList args = new ExprList(a1, a2);

    Variable v1 = new Variable("$a");
    Variable v2 = new Variable("$b");

    Operator op = new Operator(Ops.ADD, v1, v2);

    FunctionDeclaration fd = new FunctionDeclaration("jo", args, new ExprList(
        op));

    FunctionPrototype fp = new FunctionPrototype("jo", new ParamType("$a",
        Types.INTEGER), new ParamType("$b", Types.INTEGER));

    assertNull(visitor.getFunctionTable().lookup(fp));
    visitor.visit(fd);
    assertNotNull(visitor.getFunctionTable().lookup(fp));
  }

  public void testNonBuiltinFunctionCall() {
    ArgumentDeclaration a1 = new ArgumentDeclaration("$a", Types.INTEGER);
    ArgumentDeclaration a2 = new ArgumentDeclaration("$b", Types.INTEGER);
    ExprList args = new ExprList(a1, a2);

    Variable v1 = new Variable("$a");
    Variable v2 = new Variable("$b");

    Operator op = new Operator(Ops.ADD, v1, v2);

    FunctionDeclaration fd = new FunctionDeclaration("jo", args, new ExprList(
        op));

    FunctionPrototype fp = new FunctionPrototype("jo", new ParamType("$a",
        Types.INTEGER), new ParamType("$b", Types.INTEGER));

    assertNull(visitor.getFunctionTable().lookup(fp));
    visitor.visit(fd);
    assertNotNull(visitor.getFunctionTable().lookup(fp));

    Constant c1 = new Constant(Types.INTEGER, "14");
    Constant c2 = new Constant(Types.INTEGER, "27");
    ExprList el = new ExprList(c1, c2);

    FunctionCall fc = new FunctionCall("jo");
    fc.addChildren(c1, c2);

    visitor.visit(fc);
    assertEquals(new IntegerType(41), visitor.peek());
  }

  public void testPromoteInts() {
    NumberType[] nums = { new IntegerType("4"), new DecimalType("3.14") };
    assertEquals(nums[0].getClass(), IntegerType.class);

    nums[0] = Util.promoteInt(nums[0]);
    nums[1] = Util.promoteInt(nums[1]);
    assertEquals(nums[0].getClass(), DecimalType.class);
  }

  public void testCheckArgsWithUnaryOps() {
    for (String func : Ops.UNARY_OPS) {
      try {
        visitor.visit(new Operator(func));
        fail();
      } catch (RuntimeException e) {
        assertEquals("Function with prototype " + func + "() not found.",
                     e.getMessage());
      }
    }
  }

  public void testCheckArgsWithBinaryOps() {
    for (String func : Ops.BINARY_OPS) {
      try {
        visitor.visit(new Operator(func));
        fail();
      } catch (RuntimeException e) {
        assertEquals("Function with prototype " + func + "() not found.",
                     e.getMessage());
      }
    }
  }

  public void tearDown() {
  }
}
