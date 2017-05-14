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
public class FuncPrototypes {
  public static final FunctionPrototype ATTR_ENV =
    new FunctionPrototype(Funcs.ATTR_ENV, Types.ATTR_ENV);
  
  public static final FunctionPrototype FIRST =
    new FunctionPrototype(Funcs.FIRST, Types.ANY, Types.SEQUENCE);
  public static final FunctionPrototype CHILDREN =
    new FunctionPrototype(Funcs.CHILDREN, Types.SEQUENCE, Types.ENODE);
  public static final FunctionPrototype GET_ATTR_ENV =
    new FunctionPrototype(Funcs.GET_ATTR_ENV, Types.ATTR_ENV, Types.NODE);
  public static final FunctionPrototype IS_TNODE =
    new FunctionPrototype(Funcs.IS_TNODE, Types.BOOLEAN, Types.ANY);
  public static final FunctionPrototype TAIL =
    new FunctionPrototype(Funcs.TAIL, Types.ANY, Types.SEQUENCE);
  public static final FunctionPrototype TAGNAME =
    new FunctionPrototype(Funcs.TAGNAME, Types.STRING, Types.NODE);
  public static final FunctionPrototype IS_NODE =
    new FunctionPrototype(Funcs.IS_NODE, Types.BOOLEAN, Types.ANY);
  public static final FunctionPrototype INTEGER =
    new FunctionPrototype(Funcs.INTEGER, Types.INTEGER, Types.ANY);
  public static final FunctionPrototype IS_ENODE =
    new FunctionPrototype(Funcs.IS_ENODE, Types.BOOLEAN, Types.ANY);
  public static final FunctionPrototype LENGTH =
    new FunctionPrototype(Funcs.LENGTH, Types.INTEGER, Types.SEQUENCE);
  public static final FunctionPrototype DOCUMENT =
    new FunctionPrototype(Funcs.DOCUMENT, Types.NODE, Types.STRING);
  public static final FunctionPrototype DECIMAL =
    new FunctionPrototype(Funcs.DECIMAL, Types.DECIMAL, Types.ANY);
  public static final FunctionPrototype GET_ATTR_KEYS =
    new FunctionPrototype(Funcs.GET_ATTR_KEYS, Types.SEQUENCE, Types.ATTR_ENV);
  public static final FunctionPrototype STRING =
    new FunctionPrototype(Funcs.STRING, Types.STRING, Types.ANY);
  
  public static final FunctionPrototype CONCAT =
    new FunctionPrototype(Funcs.CONCAT, Types.STRING,
                          Types.STRING, Types.STRING);
  public static final FunctionPrototype ENODE =                        
    new FunctionPrototype(Funcs.ENODE, Types.ENODE,
                          Types.STRING, Types.ATTR_ENV);
  public static final FunctionPrototype GET_ATTR_VALUE =
    new FunctionPrototype(Funcs.GET_ATTR_VALUE, Types.ANY,
                          Types.ATTR_ENV, Types.STRING);
  public static final FunctionPrototype SET_ATTR_ENV =
    new FunctionPrototype(Funcs.SET_ATTR_ENV, Types.NODE,
                          Types.NODE, Types.ATTR_ENV);
  public static final FunctionPrototype ADD_CHILD_NODE =
    new FunctionPrototype(Funcs.ADD_CHILD_NODE, Types.ENODE,
                          Types.ENODE, Types.NODE);
  
  public static final FunctionPrototype ADD_ATTR =
    new FunctionPrototype(Funcs.ADD_ATTR, Types.ATTR_ENV,
                          Types.ATTR_ENV, Types.STRING, Types.STRING);
  public static final FunctionPrototype TNODE =
    new FunctionPrototype(Funcs.TNODE, Types.TNODE,
                          Types.STRING, Types.ATTR_ENV, Types.STRING);
  

  public static final FunctionPrototype [] NO_ARG_FUNCS = {
    ATTR_ENV
  };

  public static final FunctionPrototype [] UNARY_FUNCS = {
    FIRST, CHILDREN, GET_ATTR_ENV, IS_TNODE, TAIL, TAGNAME, IS_NODE, INTEGER,
    IS_ENODE, LENGTH, DOCUMENT, DECIMAL, GET_ATTR_KEYS, STRING
  };

  public static final FunctionPrototype [] BINARY_FUNCS = {
    CONCAT, ENODE, GET_ATTR_VALUE, SET_ATTR_ENV, ADD_CHILD_NODE
  };

  public static final FunctionPrototype[] TERTIARY_FUNCS = {
    ADD_ATTR, TNODE
  };
  
  
  private static boolean func_map_built = false;
  private static Map<FunctionPrototype, Integer> func_args_map =
      new HashMap<FunctionPrototype, Integer>();
  
  public static Integer numArgs(FunctionPrototype func) {
    if (!func_map_built) {
      for (FunctionPrototype fp : NO_ARG_FUNCS)
        func_args_map.put(fp, 0);
      
      for (FunctionPrototype fp : UNARY_FUNCS)
        func_args_map.put(fp, 1);
      
      for (FunctionPrototype fp : BINARY_FUNCS)
        func_args_map.put(fp, 2);
      
      for (FunctionPrototype fp : TERTIARY_FUNCS)
        func_args_map.put(fp, 3);

      func_map_built = true;
    }
    return func_args_map.get(func);
  }
  
  public static boolean contains(FunctionPrototype func) {
    return numArgs(func) != null;
  }
}
