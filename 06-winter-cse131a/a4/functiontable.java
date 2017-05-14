import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.NoSuchElementException;
import java.util.Set;
import java.util.SortedMap;
import java.util.SortedSet;
import java.util.TreeMap;
import java.util.TreeSet;

import onyx.xml.OnyxDocument;
import onyx.xml.error.OnyxXMLException;

public class FunctionTable {
  private HashMap<FunctionPrototype, ASTNode> table;
  private HashMap<String, ArrayList<FunctionPrototype>> func_names;
  
  public FunctionTable() {
    table = new HashMap<FunctionPrototype, ASTNode>();
    func_names = new HashMap<String, ArrayList<FunctionPrototype>>();
    addBuiltInFunctions();
  }

  public void addBuiltInFunctions() {
    // built-in functions
    for (FunctionPrototype fp : FuncPrototypes.NO_ARG_FUNCS)
      add(fp, new ExprList());
    for (FunctionPrototype fp : FuncPrototypes.UNARY_FUNCS)
      add(fp, new ExprList());
    for (FunctionPrototype fp : FuncPrototypes.BINARY_FUNCS)
      add(fp, new ExprList());
    for (FunctionPrototype fp : FuncPrototypes.TERTIARY_FUNCS)
      add(fp, new ExprList());
    
    // built-in operators
    for (FunctionPrototype fp : OpPrototypes.UNARY_OPS)
      add(fp, new ExprList());
    for (FunctionPrototype fp : OpPrototypes.BINARY_OPS)
      add(fp, new ExprList());
  }
  
  public void add(FunctionPrototype func, ASTNode code) {
    table.put(func, code);
    
    ArrayList<FunctionPrototype> protos = func_names.get(func.getName());
    if (protos == null)
      protos = new ArrayList<FunctionPrototype>();
    protos.add(func);
    func_names.put(func.getName(), protos);
  }
  
  public boolean contains(FunctionPrototype func) {
    return table.get(func) != null;
  }

  public ASTNode lookup(FunctionPrototype func) {
    return table.get(func);
  }
  
  public List<FunctionPrototype> getPrototypesByName(String name) {
    return func_names.get(name);
  }

  public FunctionPrototype getMatchingPrototype(FunctionPrototype func) {
    // get all functions that have this name
    List<FunctionPrototype> fps = getPrototypesByName(func.getName());
    
    // error if no function prototypes found
    if (fps == null || fps.isEmpty())
      throw new FunctionPrototypeNotFoundCandidatesException(func.getName());
    
    SortedMap<Integer, List<FunctionPrototype>> func_proms =
      new TreeMap<Integer, List<FunctionPrototype>>();
    
    List<FunctionPrototype> prot_list;
    int num_promotions;
  
    // find the function prototypes that requires the least type promotions.
    for (FunctionPrototype fp : fps) {
      // figure out how many promotions need to be done to convert this
      // FunctionPrototype to the current one we're working with
      num_promotions = func.numPromotionsTo(fp);
      
      // don't add FuntionPrototypes we can't promote to
      if (num_promotions == FunctionPrototype.CANNOT_PROMOTE)
        continue;
      
      // get list of FunctionPrototype
      prot_list = func_proms.get(num_promotions);
      
      // no FunctionPrototypes with this many promotions required yet, so we
      // make a new list
      if (prot_list == null)
        prot_list = new ArrayList<FunctionPrototype>();
      
      // add this function to the list
      prot_list.add(fp);
      
      // put the list back into the map
      func_proms.put(num_promotions, prot_list);
    }
    try {
      // get list of FunctionPrototypes for minimum number of conversions  
      int lowest_proms = func_proms.firstKey();
      prot_list = func_proms.get(lowest_proms);

      // no matching function, produce error
      if (prot_list.isEmpty()) {
        throw new FunctionPrototypeNotFoundCandidatesException(func.getName());
        // found our matching function
      } else if (prot_list.size() == 1) {
        return prot_list.get(0);
        // multiple functions found, produce error with prefix matching list
      } else {
    //  throw new FunctionPrototypeNotFoundCandidatesException(func.getName());
        throw new FunctionPrototypeNotFoundPrefixesException(
          ErrorMessages.functionPrototypeNotFound(
              func.toCandidateErrorString()), getAllPrefixPrototypes(func));
      }
      
      // no match found, produce error message with candidates list
    } catch (NoSuchElementException e) {
        throw new FunctionPrototypeNotFoundCandidatesException(func.getName());
    }
  }

  public boolean validArgs(AnyType [] args, Class ... types) {
    if (types.length != args.length)
      return false;
    
    for (int i = 0; i < types.length; ++i)
      if (!types[i].isInstance(args[i]))
        return false;
    
    return true;
  }

  public AnyType builtInAddAttr(AnyType ... args) {
    if (!validArgs(args, AttrEnvType.class, StringType.class, StringType.class))
      throw new RuntimeException("buildInAndAttr - wrong args");
    
    StringType key = (StringType)args[1];
    
    try {
      ((AttrEnvType)args[0]).add(key, (StringType)args[2]);
    } catch (OnyxXMLException e) {
      throw new InvalidOnyxAttributeException(
          ErrorMessages.invalidOnyxAttribute(key.getValue()));
    }
    
    return args[0];
  }
  
  public AnyType builtInAddChildNode(AnyType ... args) {
    if (!validArgs(args, ENodeType.class, NodeType.class))
      throw new RuntimeException("buildInAddChildNode - wrong args");
    
    ((ENodeType)args[0]).add((NodeType)args[1]);
    
    return args[0];
  }
  
  public AnyType builtInAttrEnv(AnyType [] args) {
    if (args.length != 0)
      throw new RuntimeException("buildInAttrEnv - wrong args");
    
    return new AttrEnvType();
  }

  public AnyType builtInChildren(AnyType ... args) {
    if (!validArgs(args, ENodeType.class))
      throw new RuntimeException("buildInChildren - wrong args");

    return ((ENodeType)args[0]).childrenSequence();
  }
  
  public AnyType builtInConcat(AnyType [] args) {
    if (!validArgs(args, StringType.class, StringType.class))
      throw new RuntimeException("buildInConcat - wrong args");
    
    return ((StringType)args[0]).concat((StringType)args[1]);
  }
  
  public AnyType builtInDecimal(AnyType ... args) {
    if (!validArgs(args, AnyType.class))
      // TODO: Throw error
      return null;
    
    try {
      if (args[0] instanceof StringType)
        return ((StringType)args[0]).toDecimalType();      
    } catch (NumberFormatException e) {
      throw new InvalidStringToNumberException(
            ErrorMessages.invalidStringToNum(((StringType)args[0]).getValue()));
    }
    
    if (args[0] instanceof IntegerType)
      return ((IntegerType)args[0]).toDecimalType();
    
    if (args[0] instanceof DecimalType)
      return new DecimalType((DecimalType)args[0]);
    
    return null;
  }
  
  public AnyType builtInDocument(AnyType ... args) {
    if (!validArgs(args, StringType.class))
      return null;

    StringType s = (StringType)args[0];
    try {
      return new ENodeType(OnyxDocument.docFromURI(s.getValue()));
    } catch (OnyxXMLException e) {
      throw new InvalidOnyxDocumentException(
          ErrorMessages.invalidOnyxDocument(s.getValue()));
    }
  }
  
  public AnyType builtInENode(AnyType ... args) {
    if (!validArgs(args, StringType.class, AttrEnvType.class))
      return null;
    
    StringType s = (StringType)args[0];
    
    try {
      return new ENodeType(s, (AttrEnvType)args[1]);
    } catch (OnyxXMLException e) {
      throw new InvalidOnyxTagnameException(
          ErrorMessages.invalidOnyxTagname(s.getValue()));
    }
  }

  public AnyType builtInFirst(AnyType ... args) {
    if (!validArgs(args, SequenceType.class))
      return null;
    
    return ((SequenceType)args[0]).first();
  }
  
  public AnyType builtInGetAttrEnv(AnyType ... args) {
    if (!validArgs(args, NodeType.class))
      return null;
    
     return ((NodeType)args[0]).getAttr();
  }
  
  public AnyType builtInGetAttrKeys(AnyType ... args) {
    if (!validArgs(args, AttrEnvType.class))
      return null;
    
    Set<String> set = ((AttrEnvType)args[0]).keySet();
    List<AnyType> list = new ArrayList<AnyType>();

    for (String s: set)
      list.add(new StringType(s));
      
    return new SequenceType(list);
  }
  
  public AnyType builtInGetAttrValue(AnyType ... args) {
    if (!validArgs(args, AttrEnvType.class, StringType.class))
      return null;
    
    StringType s = ((AttrEnvType)args[0]).lookup((StringType)args[1]);
    if (s == null)
      return new SequenceType();
    
    return s;
  }
  
  public AnyType builtInInteger(AnyType ... args) {
    if (!validArgs(args, AnySimpleType.class))
      return null;
    
    if (args[0] instanceof IntegerType)
      return new IntegerType((IntegerType)args[0]);
    
    if (args[0] instanceof DecimalType)
      return ((DecimalType)args[0]).toIntegerType();
    
    try {
      if (args[0] instanceof StringType)
        return ((StringType)args[0]).toIntegerType();      
    } catch (NumberFormatException e) {
      throw new InvalidStringToNumberException(
            ErrorMessages.invalidStringToNum(((StringType)args[0]).getValue()));
    }
      
    // TODO: If bool, throw err
    return null;
  }
  
  public AnyType builtInIsENode(AnyType ... args) {
    if (!validArgs(args, AnyType.class))
      // TODO: Err checking
      return null;
    
    if (args[0] instanceof ENodeType)
      return new BooleanType(true);
    
    return new BooleanType(false);
  }
  
  public AnyType builtInIsNode(AnyType ... args) {
    if (!validArgs(args, AnyType.class))
      // TODO: Err checking
      return null;
    
    if (args[0] instanceof NodeType)
      return new BooleanType(true);
    
    return new BooleanType(false);
  }
  
  public AnyType builtInIsTNode(AnyType ... args) {
    if (!validArgs(args, AnyType.class))
      // TODO: Err checking
      return null;
    
    if (args[0] instanceof TNodeType)
      return new BooleanType(true);
    
    return new BooleanType(false);
  }
  
  public AnyType builtInLength(AnyType ... args) {
    if (!validArgs(args, SequenceType.class))
      return null;
    
    // TODO: Type err
    return ((SequenceType)args[0]).length();
  }
  
  public AnyType builtInSetAttrEnv(AnyType ... args) {
    if (!validArgs(args, NodeType.class, AttrEnvType.class))
      return null;
    
    ((NodeType)args[0]).setAttr((AttrEnvType)args[1]);
    return args[0];
  }
  
  public AnyType builtInString(AnyType ... args) {
    if (!validArgs(args, AnyType.class))
      return null;
    
    if (args[0] instanceof TNodeType)
      return new StringType(((TNodeType)args[0]).getValue().getText());
    
    if (args[0] instanceof SequenceType)
      return new StringType("[" + ((SequenceType)args[0]).stringOf() + "]");
    
    if (args[0] instanceof AnySimpleType)
      return new StringType(((AnySimpleType)args[0]).stringOf());
    
    // TODO: err if wrong type
    return null;
  }
  
  public AnyType builtInTagname(AnyType ... args) {
    if (!validArgs(args, NodeType.class))
      return null;
    
    return ((NodeType)args[0]).getTagName();
  }
  
  public AnyType builtInTail(AnyType  ... args) {
    if (!validArgs(args, SequenceType.class))
      throw new RuntimeException("builtInTail - wrong args");
    // TODO: fix this throw to an error
    
    return ((SequenceType)args[0]).tail();
  }
  
  public AnyType builtInTNode(AnyType ... args) {
    if (!validArgs(args, StringType.class, AttrEnvType.class, StringType.class))
      throw new RuntimeException("builtInTNode - wrong args");
    // TODO: fix this throw to an error
    
    StringType s = (StringType)args[0];
    try {
      return new TNodeType(s, (AttrEnvType)args[1], (StringType)args[2]);
    } catch (OnyxXMLException e) {
      throw new InvalidOnyxTagnameException(
          ErrorMessages.invalidOnyxTagname(s.getValue()));
    }
  }

  public IntegerType opAddInteger(IntegerType i1, IntegerType i2) {
    return i1.add(i2);
  }

  public DecimalType opAddDecimal(DecimalType d1, DecimalType d2) {
    return d1.add(d2);
  }
  
  public StringType opAddString(StringType s1, StringType s2) {
    return s1.add(s2);
  }
  
  public IntegerType opSubtractInteger(IntegerType i1, IntegerType i2) {
    return i1.subtract(i2);
  }
  
  public DecimalType opSubtractDecimal(DecimalType d1, DecimalType d2) {
    return d1.subtract(d2);
  }

  public IntegerType opMultiplyInteger(IntegerType i1, IntegerType i2) {
    return i1.multiply(i2);
  }
  
  public DecimalType opMultiplyDecimal(DecimalType d1, DecimalType d2) {
    return d1.multiply(d2);
  }
  
  public DecimalType opDivide(DecimalType d1, DecimalType d2) {
    return d1.divide(d2);
  }
  
  public IntegerType opIntegerDivide(IntegerType i1, IntegerType i2) {
    return i1.divide(i2);
  }
  
  public IntegerType opMod(IntegerType i1, IntegerType i2) {
    return i1.mod(i2);
  }
  
  public IntegerType opUnaryPlus(IntegerType i1) {
    return i1;
  }
  
  public DecimalType opUnaryPlus(DecimalType d1) {
    return d1;
  }
  
  public IntegerType opUnaryMinus(IntegerType i1) {
    return i1.negate();
  }
  
  public DecimalType opUnaryMinus(DecimalType d1) {
    return d1.negate();
  }
  
  public SequenceType opTo(IntegerType i1, IntegerType i2) {
    return i1.to(i2);
  }
  
  public BooleanType opAnd(BooleanType b1, BooleanType b2) {
    return b1.and(b2);
  }
  
  public BooleanType opOr(BooleanType b1, BooleanType b2) {
    return b1.or(b2);
  }
  
  public BooleanType opEqualsBool(BooleanType b1, BooleanType b2) {
    return b1.equalTo(b2);
  }
  
  public BooleanType opEqualsDecimal(DecimalType d1, DecimalType d2) {
    return d1.equalTo(d2);
  }
  
  public BooleanType opEqualsInteger(IntegerType i1, IntegerType i2) {
    return i1.equalTo(i2);
  }
  
  public BooleanType opEqualsString(StringType s1, StringType s2) {
    return s1.equalTo(s2);
  }
  
  public AnyType opEquals(AnySimpleType a1, AnySimpleType a2) {
    if (Util.sameClass(BooleanType.class, a1, a2))
      return opEqualsBool((BooleanType)a1, (BooleanType)a2);
    
    if (Util.sameClass(IntegerType.class, a1, a2))
      return opEqualsInteger((IntegerType)a1, (IntegerType)a2);
    
    if (Util.sameClass(StringType.class, a1, a2))
      return opEqualsString((StringType)a1, (StringType)a2);
     
    a1 = Util.promoteInt(a1);
    a2 = Util.promoteInt(a2);

    if (Util.sameClass(DecimalType.class, a1, a2))
      return opEqualsDecimal((DecimalType)a1, (DecimalType)a2);
    
    return null;
  }
  
  public BooleanType opNotEqualsBool(BooleanType b1, BooleanType b2) {
    return b1.notEqualTo(b2);
  }
  
  public BooleanType opNotEqualsDecimal(DecimalType d1, DecimalType d2) {
    return d1.notEqualTo(d2);
  }
  
  public BooleanType opNotEqualsInteger(IntegerType i1, IntegerType i2) {
    return i1.notEqualTo(i2);
  }
  
  public BooleanType opNotEqualsString(StringType s1, StringType s2) {
    return s1.notEqualTo(s2);
  }
  
  public AnyType opNotEquals(AnySimpleType a1, AnySimpleType a2) {
    if (Util.sameClass(BooleanType.class, a1, a2))
      return opNotEqualsBool((BooleanType)a1, (BooleanType)a2);
    
    if (Util.sameClass(IntegerType.class, a1, a2))
      return opNotEqualsInteger((IntegerType)a1, (IntegerType)a2);
    
    if (Util.sameClass(StringType.class, a1, a2))
      return opNotEqualsString((StringType)a1, (StringType)a2);
    
    a1 = Util.promoteInt(a1);
    a2 = Util.promoteInt(a2);

    if (Util.sameClass(DecimalType.class, a1, a2))
      return opNotEqualsDecimal((DecimalType)a1, (DecimalType)a2);
    
    return null;
  }
  
  public BooleanType opLessThanDecimal(DecimalType d1, DecimalType d2) {
    return d1.lessThan(d2);
  }
  
  public BooleanType opLessThanInteger(IntegerType i1, IntegerType i2) {
    return i1.lessThan(i2);
  }
  
  public BooleanType opLessThanString(StringType s1, StringType s2) {
    return s1.lessThan(s2);
  }
  
  public AnyType opLessThan(AnySimpleType a1, AnySimpleType a2) {   
    if (Util.sameClass(IntegerType.class, a1, a2))
      return opLessThanInteger((IntegerType)a1, (IntegerType)a2);
    
    if (Util.sameClass(StringType.class, a1, a2))
      return opLessThanString((StringType)a1, (StringType)a2);
    
    a1 = Util.promoteInt(a1);
    a2 = Util.promoteInt(a2);
    
    if (Util.sameClass(DecimalType.class, a1, a2))
      return opLessThanDecimal((DecimalType)a1, (DecimalType)a2);
    
    return null;
  }
  
  public BooleanType opGreaterThanDecimal(DecimalType d1, DecimalType d2) {
    return d1.greaterThan(d2);
  }
  
  public BooleanType opGreaterThanInteger(IntegerType i1, IntegerType i2) {
    return i1.greaterThan(i2);
  }
  
  public BooleanType opGreaterThanString(StringType s1, StringType s2) {
    return s1.greaterThan(s2);
  }
  
  public AnyType opGreaterThan(AnySimpleType a1, AnySimpleType a2) {   
    if (Util.sameClass(IntegerType.class, a1, a2))
      return opGreaterThanInteger((IntegerType)a1, (IntegerType)a2);
    
    if (Util.sameClass(StringType.class, a1, a2))
      return opGreaterThanString((StringType)a1, (StringType)a2);
    
    Util.promoteInts(a1, a2);

    if (Util.sameClass(DecimalType.class, a1, a2))
      return opGreaterThanDecimal((DecimalType)a1, (DecimalType)a2);
    
    return null;
  }

  public BooleanType opLessEqualsDecimal(DecimalType d1, DecimalType d2) {
    return d1.lessEquals(d2);
  }
  
  public BooleanType opLessEqualsInteger(IntegerType i1, IntegerType i2) {
    return i1.lessEquals(i2);
  }
  
  public BooleanType opLessEqualsString(StringType s1, StringType s2) {
    return s1.lessEquals(s2);
  }
  
  public AnyType opLessEquals(AnySimpleType a1, AnySimpleType a2) {   
    if (Util.sameClass(IntegerType.class, a1, a2))
      return opLessEqualsInteger((IntegerType)a1, (IntegerType)a2);
    
    if (Util.sameClass(StringType.class, a1, a2))
      return opLessEqualsString((StringType)a1, (StringType)a2);
    
    Util.promoteInts(a1, a2);

    if (Util.sameClass(DecimalType.class, a1, a2))
      return opLessEqualsDecimal((DecimalType)a1, (DecimalType)a2);
    
    return null;
  }
  
  public BooleanType opGreaterEqualsDecimal(DecimalType d1, DecimalType d2) {
    return d1.greaterEquals(d2);
  }
  
  public BooleanType opGreaterEqualsInteger(IntegerType i1, IntegerType i2) {
    return i1.greaterEquals(i2);
  }
  
  public BooleanType opGreaterEqualsString(StringType s1, StringType s2) {
    return s1.greaterEquals(s2);
  }

  public AnyType opGreaterEquals(AnySimpleType a1, AnySimpleType a2) {   
    if (Util.sameClass(IntegerType.class, a1, a2))
      return opGreaterEqualsInteger((IntegerType)a1, (IntegerType)a2);
    
    if (Util.sameClass(StringType.class, a1, a2))
      return opGreaterEqualsString((StringType)a1, (StringType)a2);
    
    Util.promoteInts(a1, a2);

    if (Util.sameClass(DecimalType.class, a1, a2))
      return opGreaterEqualsDecimal((DecimalType)a1, (DecimalType)a2);
    
    return null;
  }
  
  public boolean isBuiltIn(FunctionPrototype func) {
    return OpPrototypes.contains(func) || FuncPrototypes.contains(func);
  }

  public AnyType callBuiltIn(FunctionPrototype func, AnyType[] args) {
    // built-in operators
    if (func.equals(OpPrototypes.ADD_D))
      return opAddDecimal((DecimalType)args[0], (DecimalType)args[1]);
    else if (func.equals(OpPrototypes.ADD_I))
      return opAddInteger((IntegerType)args[0], (IntegerType)args[1]);
    else if (func.equals(OpPrototypes.ADD_S))
      return opAddString((StringType)args[0], (StringType)args[1]);
    else if (func.equals(OpPrototypes.SUBTRACT_I))
      return opSubtractInteger((IntegerType)args[0], (IntegerType)args[1]);
    else if (func.equals(OpPrototypes.SUBTRACT_D))
      return opSubtractDecimal((DecimalType)args[0], (DecimalType)args[1]);
    else if (func.equals(OpPrototypes.MULTIPLY_I))
      return opMultiplyInteger((IntegerType)args[0], (IntegerType)args[1]);
    else if (func.equals(OpPrototypes.MULTIPLY_D))
      return opMultiplyDecimal((DecimalType)args[0], (DecimalType)args[1]);
    else if (func.equals(OpPrototypes.DIVIDE))
      return opDivide((DecimalType)args[0], (DecimalType)args[1]);
    else if (func.equals(OpPrototypes.INTEGER_DIVIDE))
      return opIntegerDivide((IntegerType)args[0], (IntegerType)args[1]);
    else if (func.equals(OpPrototypes.MOD))
      return opMod((IntegerType)args[0], (IntegerType)args[1]);
    else if (func.equals(OpPrototypes.UNARY_PLUS_D))
      return opUnaryPlus((DecimalType)args[0]);
    else if (func.equals(OpPrototypes.UNARY_PLUS_I))
      return opUnaryPlus((IntegerType)args[0]);
    else if (func.equals(OpPrototypes.UNARY_MINUS_D))
      return opUnaryMinus((DecimalType)args[0]);
    else if (func.equals(OpPrototypes.UNARY_MINUS_I))
      return opUnaryMinus((IntegerType)args[0]);
    else if (func.equals(OpPrototypes.TO))
      return opTo((IntegerType)args[0], (IntegerType)args[1]);
    else if (func.equals(OpPrototypes.AND))
      return opAnd((BooleanType)args[0], (BooleanType)args[1]);
    else if (func.equals(OpPrototypes.OR))
      return opOr((BooleanType)args[0], (BooleanType)args[1]);
    else if (func.equals(OpPrototypes.EQUALS))
      return opEquals((AnySimpleType)args[0], (AnySimpleType)args[1]);
    else if (func.equals(OpPrototypes.NOT_EQUALS))
      return opNotEquals((AnySimpleType)args[0], (AnySimpleType)args[1]);
    else if (func.equals(OpPrototypes.LESS_THAN))
      return opLessThan((AnySimpleType)args[0], (AnySimpleType)args[1]);
    else if (func.equals(OpPrototypes.GREATER_THAN))
      return opGreaterThan((AnySimpleType)args[0], (AnySimpleType)args[1]);
    else if (func.equals(OpPrototypes.LESS_EQUALS))
      return opLessEquals((AnySimpleType)args[0], (AnySimpleType)args[1]);
    else if (func.equals(OpPrototypes.GREATER_EQUALS))
      return opGreaterEquals((AnySimpleType)args[0], (AnySimpleType)args[1]);
    
    // built-in functions
    else if (func.getName().equals(Funcs.ADD_ATTR))
      return builtInAddAttr(args);
    else if (func.getName().equals(Funcs.ADD_CHILD_NODE))
      return builtInAddChildNode(args);
    else if (func.getName().equals(Funcs.ATTR_ENV))
      return builtInAttrEnv(args);
    else if (func.getName().equals(Funcs.CHILDREN))
      return builtInChildren(args);
    else if (func.getName().equals(Funcs.CONCAT))
      return builtInConcat(args);
    else if (func.getName().equals(Funcs.DECIMAL))
      return builtInDecimal(args);
    else if (func.getName().equals(Funcs.DOCUMENT))
      return builtInDocument(args);
    else if (func.getName().equals(Funcs.ENODE))
      return builtInENode(args);
    else if (func.getName().equals(Funcs.TNODE))
      return builtInTNode(args);
    else if (func.getName().equals(Funcs.FIRST))
      return builtInFirst(args);
    else if (func.getName().equals(Funcs.GET_ATTR_ENV))
      return builtInGetAttrEnv(args);
    else if (func.getName().equals(Funcs.GET_ATTR_KEYS))
      return builtInGetAttrKeys(args);
    else if (func.getName().equals(Funcs.GET_ATTR_VALUE))
      return builtInGetAttrValue(args);
    else if (func.getName().equals(Funcs.INTEGER))
      return builtInInteger(args);
    else if (func.getName().equals(Funcs.IS_ENODE))
      return builtInIsENode(args);
    else if (func.getName().equals(Funcs.IS_NODE))
      return builtInIsNode(args);
    else if (func.getName().equals(Funcs.IS_TNODE))
      return builtInIsTNode(args);
    else if (func.getName().equals(Funcs.LENGTH))
      return builtInLength(args);
    else if (func.getName().equals(Funcs.SET_ATTR_ENV))
      return builtInSetAttrEnv(args);
    else if (func.getName().equals(Funcs.STRING))
      return builtInString(args);
    else if (func.getName().equals(Funcs.TAGNAME))
      return builtInTagname(args);
    else if (func.getName().equals(Funcs.TAIL))
      return builtInTail(args);
    else
      throw new RuntimeException("callBuiltIn - func name not match: "+ func);
  }

  public SortedSet<String> getAllCandidatePrototypes(FunctionPrototype func) {
    SortedSet<String> set = new TreeSet<String>();
    for (FunctionPrototype fp : table.keySet())
      if (func.isNamePrefixOf(fp))
        set.add(fp.toCandidateErrorString());
    return set;
  }

  public List<FunctionPrototype>
  getAllPrefixPrototypes(FunctionPrototype func) {
    List<FunctionPrototype> set = new ArrayList<FunctionPrototype>();
    for (FunctionPrototype fp : table.keySet()) {
      // System.err.println("evaluating func: " + fp);
      if (func.isPrefixOf(fp)) {
        //System.err.println("added to list: " + fp);
        set.add(fp);
      }
    }
    Collections.sort(set);
    //System.err.println("set: " + set);
    return set;
  }
  
  public FunctionPrototype getClosestPrototype(FunctionPrototype func) {
    List<FunctionPrototype> matches = getAllPrefixPrototypes(func);

    if (matches.isEmpty())
      return null;
    
    return matches.get(0);
  }

  public ASTNode getFunctionBody(FunctionPrototype func) {
    return table.get(func);

    // TODO: error checking?
  }
}
