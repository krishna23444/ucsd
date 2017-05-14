/*
 * Author: nitay, kevin
 * Date: Mar 7, 2006
 * Project: compilers_a3
 */

import java.util.HashMap;
import java.util.Map;

/**
 * 
 */
public class OpPrototypes {     
  public static final FunctionPrototype ADD_D =
    new FunctionPrototype(Ops.ADD, Types.DECIMAL, Types.DECIMAL, Types.DECIMAL);
  public static final FunctionPrototype ADD_I =
    new FunctionPrototype(Ops.ADD, Types.INTEGER, Types.INTEGER, Types.INTEGER);
  public static final FunctionPrototype ADD_S =
    new FunctionPrototype(Ops.ADD, Types.STRING, Types.STRING, Types.STRING);
  
  public static final FunctionPrototype SUBTRACT_D =
    new FunctionPrototype(Ops.SUBTRACT, Types.DECIMAL,
                          Types.DECIMAL, Types.DECIMAL);
  public static final FunctionPrototype SUBTRACT_I =
    new FunctionPrototype(Ops.SUBTRACT, Types.INTEGER,
                          Types.INTEGER, Types.INTEGER);
  
  public static final FunctionPrototype MULTIPLY_D =
    new FunctionPrototype(Ops.MULTIPLY, Types.DECIMAL,
                          Types.DECIMAL, Types.DECIMAL);
  public static final FunctionPrototype MULTIPLY_I =
    new FunctionPrototype(Ops.MULTIPLY, Types.INTEGER,
                          Types.INTEGER, Types.INTEGER);
  
  public static final FunctionPrototype MOD =
    new FunctionPrototype(Ops.MOD, Types.INTEGER, Types.INTEGER, Types.INTEGER);
  
  public static final FunctionPrototype DIVIDE =
    new FunctionPrototype(Ops.DIVIDE, Types.DECIMAL,
                          Types.DECIMAL, Types.DECIMAL);
  
  public static final FunctionPrototype INTEGER_DIVIDE  =
    new FunctionPrototype(Ops.INTEGER_DIVIDE, Types.INTEGER,
                          Types.INTEGER, Types.INTEGER);
 
  public static final FunctionPrototype EQUALS =
    new FunctionPrototype(Ops.EQUALS, Types.BOOLEAN,
                          Types.ANY_SIMPLE, Types.ANY_SIMPLE);
//  public static final FunctionPrototype EQUALS_B =
//    new FunctionPrototype(Ops.EQUALS, Types.BOOLEAN,
//                          Types.BOOLEAN, Types.BOOLEAN);
//  public static final FunctionPrototype EQUALS_D =
//    new FunctionPrototype(Ops.EQUALS, Types.BOOLEAN,
//                          Types.DECIMAL, Types.DECIMAL);
//  public static final FunctionPrototype EQUALS_I =
//    new FunctionPrototype(Ops.EQUALS, Types.BOOLEAN,
//                          Types.INTEGER, Types.INTEGER);
//  public static final FunctionPrototype EQUALS_S =
//    new FunctionPrototype(Ops.EQUALS, Types.BOOLEAN,
//                          Types.STRING, Types.STRING);
  
  public static final FunctionPrototype AND =
    new FunctionPrototype(Ops.AND, Types.BOOLEAN, Types.BOOLEAN, Types.BOOLEAN);
  
  public static final FunctionPrototype OR =
    new FunctionPrototype(Ops.OR, Types.BOOLEAN, Types.BOOLEAN, Types.BOOLEAN);
  
  public static final FunctionPrototype GREATER_THAN = 
    new FunctionPrototype(Ops.GREATER_THAN, Types.BOOLEAN,
                          Types.ANY_SIMPLE, Types.ANY_SIMPLE);
  
//  public static final FunctionPrototype GREATER_THAN_D =
//    new FunctionPrototype(Ops.GREATER_THAN, Types.BOOLEAN,
//                          Types.DECIMAL, Types.DECIMAL);
//  public static final FunctionPrototype GREATER_THAN_I =
//    new FunctionPrototype(Ops.GREATER_THAN, Types.BOOLEAN,
//                          Types.INTEGER, Types.INTEGER);
//  public static final FunctionPrototype GREATER_THAN_S =
//    new FunctionPrototype(Ops.GREATER_THAN, Types.BOOLEAN,
//                          Types.STRING, Types.STRING);
  
  public static final FunctionPrototype LESS_THAN = 
    new FunctionPrototype(Ops.LESS_THAN, Types.BOOLEAN,
                          Types.ANY_SIMPLE, Types.ANY_SIMPLE);

  public static final FunctionPrototype GREATER_EQUALS =
    new FunctionPrototype(Ops.GREATER_EQUALS, Types.BOOLEAN,
                          Types.ANY_SIMPLE, Types.ANY_SIMPLE);

  public static final FunctionPrototype LESS_EQUALS =
    new FunctionPrototype(Ops.LESS_EQUALS, Types.BOOLEAN,
                          Types.ANY_SIMPLE, Types.ANY_SIMPLE);
  
  public static final FunctionPrototype TO =
    new FunctionPrototype(Ops.TO, Types.SEQUENCE, Types.INTEGER, Types.INTEGER);
  
  public static final FunctionPrototype NOT_EQUALS =
    new FunctionPrototype(Ops.NOT_EQUALS, Types.BOOLEAN,
                          Types.ANY_SIMPLE, Types.ANY_SIMPLE);


  public static final FunctionPrototype UNARY_PLUS_D =
    new FunctionPrototype(Ops.UNARY_PLUS, Types.DECIMAL, Types.DECIMAL);
  public static final FunctionPrototype UNARY_PLUS_I =
    new FunctionPrototype(Ops.UNARY_PLUS, Types.INTEGER, Types.INTEGER);
  
  public static final FunctionPrototype UNARY_MINUS_D =
    new FunctionPrototype(Ops.UNARY_MINUS, Types.DECIMAL, Types.DECIMAL);
  public static final FunctionPrototype UNARY_MINUS_I =
    new FunctionPrototype(Ops.UNARY_MINUS, Types.INTEGER, Types.INTEGER);
  

  public static final FunctionPrototype[] UNARY_OPS = {
    UNARY_PLUS_D, UNARY_PLUS_I, UNARY_MINUS_D, UNARY_MINUS_I
  };
  
  public static final FunctionPrototype[] BINARY_OPS = {
    ADD_D, SUBTRACT_I, MULTIPLY_I, MOD, INTEGER_DIVIDE, ADD_S, EQUALS,
    SUBTRACT_D, MULTIPLY_D, AND, DIVIDE, OR, 
    GREATER_THAN, LESS_THAN, TO, NOT_EQUALS, 
    ADD_I, LESS_EQUALS, GREATER_EQUALS
  };

  
  private static boolean op_map_built = false;
  private static Map<FunctionPrototype, Integer> op_args_map =
      new HashMap<FunctionPrototype, Integer>();
  
  public static Integer numArgs(FunctionPrototype func) {
    if (!op_map_built) {
      for (FunctionPrototype fp : UNARY_OPS)
        op_args_map.put(fp, 1);
      
      for (FunctionPrototype fp : BINARY_OPS)
        op_args_map.put(fp, 2);

      op_map_built = true;
    }
    return op_args_map.get(func);
  }
  
  public static boolean contains(FunctionPrototype func) {
    return numArgs(func) != null;
  }
}
