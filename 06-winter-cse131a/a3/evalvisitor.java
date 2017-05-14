import java.util.Stack;

public class EvalVisitor implements Visitor {
  private Stack<AnyType> stack = new Stack<AnyType>();
  private SymbolTable sym_tab = new SymbolTable();
  private FunctionTable func_tab = new FunctionTable();

  public void visit(ExpressionNode e) {
    visitChildren(e);
  }

  public void visit(Query q) {
    visitChildren(q);
  }

  public void visit(FunctionDeclaration fd) {
    ParamType [] args = new ParamType[fd.numChildren() - 1];
    for (int i = 0; i < fd.numChildren() - 1; ++i) {
      fd.getChild(i).accept(this);
      args[i] = (ParamType)pop();
    }
    
    ASTNode body = fd.getLastChild();
    FunctionPrototype fp =
      new FunctionPrototype(fd.getName(), fd.getDatatype(), args);
    
    // check if function definition already exists
    if (func_tab.contains(fp))
      throw new SemanticStaticErrorException(fd.getLine(), fd.getColumn(),
          ErrorMessages.functionAlreadyDefined(fp.getName(),
                                               fp.toCandidateErrorString()));
          
    func_tab.add(fp, body);
  }

  public void visit(VariableDeclaration vd) {
    // symbol table already contains this variable
    if (sym_tab.lookup(vd.getName()) != null)
      throw new SemanticStaticErrorException(vd.getLine(), vd.getColumn(),
          ErrorMessages.globalVariableAlreadyDefined(vd.getName()));
    
    vd.getFirstChild().accept(this);
    
    try {
      // symbol table handles type checking to ensure as datatype matches
      sym_tab.addVariable(vd.getName(), vd.getDatatype(), pop());
    } catch (LineColumnException e) {
      e.setLineAndColumn(vd.getLine(), vd.getColumn());
      throw e;
    }
    
  }

  public void visit(ArgumentDeclaration ad) {
    push(new ParamType(ad));
  }
  
  public void visit(ForAssignment fa) {
    // Has 2 children (the sequence and the next scope)
    
    SequenceType return_val = new SequenceType();
    
    fa.getFirstChild().accept(this);
    
    SequenceType range = pop().toSequenceType();
    
    for (int i = 0; i < range.size(); i++) {
      sym_tab.pushTraversableScope();
      
      sym_tab.add(fa.getName(), range.get(i));
      //sym_tab.add(fa.getName(), range.get(i));
      
      AnyType current = peek();
      
      fa.getLastChild().accept(this);
      
      if (current != peek())
        return_val.add(pop());
      
      sym_tab.popScope();
    }
    
    push(return_val);
  }
  
  public void visit(LetAssignment la) {
    // Has 2 children (the value of the variable and the next scope)
    // Has 1 attribute varName
    
    sym_tab.pushTraversableScope();
    
    la.getFirstChild().accept(this);
    
    sym_tab.add(la.getName(), pop());
    
    la.getLastChild().accept(this);
    
    sym_tab.popScope();
  }

  public void visit(Where w) {
    // Has 2 children (the condition and the return)
    
    // Eval the condition to determine if we visit the return
    w.getFirstChild().accept(this);
    AnyType retval = pop();

    if (!(retval instanceof BooleanType))
      throw new SemanticDynamicErrorException(w.getLine(), w.getColumn(),
                                            ErrorMessages.WHERE_COND_NOT_BOOL);

    
    // If condition was true, call the return which will push the value
    if (((BooleanType)retval).getValue())
      w.getLastChild().accept(this);
  }

  public void visit(Return r) {
    visitChildren(r);
  }

  public void visit(Variable v) {
    AnyType var = sym_tab.lookup(v.getName());
    
    if (var != null) {
      push(var);
    } else {
      throw new UnboundVariableException(v.getLine(), v.getColumn(),
          ErrorMessages.unboundVariable(v.getName()));
    }
  }

  public void visit(Constant c) {
    AnySimpleType constant;
    if (c.getDatatype().equals(Types.INTEGER)) {
      constant = new IntegerType(c.getValue());
    } else if (c.getDatatype().equals(Types.DECIMAL)) {
      constant = new DecimalType(c.getValue());
    } else if (c.getDatatype().equals(Types.STRING)) {
      constant = new StringType(c.getValue());
    } else if (c.getDatatype().equals(Types.BOOLEAN)) {
      constant = new BooleanType(c.getValue());
    } else {
      constant = null;
    }
    
    push(constant);
  }
  
  public void visit(FunctionCall fc) {
    AnyType[] args = visitAndGetChildren(fc);
    ParamType[] arg_classes = new ParamType[args.length];
    
    // get the types of our arguments
    for (int i = 0; i < args.length; ++i)
      arg_classes[i] = new ParamType(Types.nameOf(args[i]));
    
    // create function prototype for looking up in table
    FunctionPrototype fp = new FunctionPrototype(fc.getName(), arg_classes);
    
    FunctionPrototype new_fp;
    
    try {
      // get best matching function prototype
      new_fp = func_tab.getMatchingPrototype(fp);
    
    } catch (FunctionPrototypeNotFoundPrefixesException e) {
      //System.err.println("in prefixes");
      e.setLineAndColumn(fc.getLine(), fc.getColumn());
      throw e;
//      throw new FunctionPrototypeNotFoundPrefixesException(fc.getLine(),
//          fc.getColumn(), fp.toPrefixErrorString(),
//          func_tab.getAllPrefixPrototypes(fp));
    
    } catch (FunctionPrototypeNotFoundCandidatesException e) {
      //System.err.println("in candidates");
      throw new FunctionPrototypeNotFoundCandidatesException(fc.getLine(),
          fc.getColumn(), fp.toCandidateErrorString(),
          func_tab.getAllCandidatePrototypes(fp));
    
    } catch (InvalidStringToNumberException e) {
      //System.err.println("in invalid string to num");
      throw new InvalidStringToNumberException(fc.getLine(), fc.getColumn(),
          e.getMessage());
    }
    //System.err.println("try passed");
    
    args = new_fp.promoteTypesToMatch(args);
    
    // can't promote the params, throw an error for function match not found
    if (args == null)
      throw new FunctionPrototypeNotFoundCandidatesException(fc.getLine(),
          fc.getColumn(), fp.toCandidateErrorString(),
          func_tab.getAllCandidatePrototypes(fp));

    // handle built-in functions and operators
    if (func_tab.isBuiltIn(new_fp)) {
      AnyType result;
      try {
        result = func_tab.callBuiltIn(new_fp, args);
        // mod < 0 error, division by 0, 
      } catch (SemanticDynamicErrorException e) {
          e.setLineAndColumn(fc.getLine(), fc.getColumn());
          throw e;
      }
      
      if (result != null)
        push(result);
      else
         throw new RuntimeException("null result from builtin: " + fp);
    } else {
      ASTNode func_code = func_tab.getFunctionBody(new_fp);

      // no function match, throw an error with all the candidates
      if (func_code == null)
        throw new FunctionPrototypeNotFoundCandidatesException(fc.getLine(),
          fc.getColumn(), fp.toCandidateErrorString(),
          func_tab.getAllCandidatePrototypes(fp));

      // overwrite the FunctionPrototype to fully describe our function
      fp = func_tab.getClosestPrototype(new_fp);

      // create new scope
      sym_tab.pushFunctionScope();
      
      // add variables to the scope
      for (int i = 0; i < args.length; ++i)
        sym_tab.add(new_fp.getParam(i).getName(), args[i]);
      
      // call function
      func_code.accept(this);
      
      // remove scope
      sym_tab.popScope();
      
      try {
        if (!Types.classOf(fp.getReturnType()).isInstance(peek())
            && !(Types.classOf(fp.getReturnType()).equals(DecimalType.class)
            && peek().getClass().equals(IntegerType.class))
            && !(Types.classOf(fp.getReturnType()).equals(SequenceType.class)))
          throw new WrongReturnTypeException(
            fc.getLine(), fc.getColumn(),
            ErrorMessages.wrongReturnType(fc.getName(), fp.getReturnType(),
                Types.nameOf(peek())));
      } catch (ClassNotFoundException e) {
        e.printStackTrace();
      }
    }
  }
  
  public void visit(Operator o) {
    FunctionCall fc = new FunctionCall(o.getName());
    fc.setLineAndColumn(o.getLine(), o.getColumn());
    fc.addChildren(o.getChildren());
    visit(fc);
  }
  
  public void visit(Flwr f) {
    ASTNode child;
    
    f.makeChildNodes();
    
    if (f.hasChildren()) {
      child = f.getFirstChild();
      
      if (child.getClass() == NonExpressionNode.class)
        child.getFirstChild().accept(this);
      else 
        child.accept(this);
    }
  }
  
  public void visit(IfThenElseExpr iftee) {
    iftee.getCondition().accept(this);
    AnyType if_cond = pop();
    
    if (if_cond instanceof BooleanType) {
      if (((BooleanType)if_cond).getValue())
        iftee.getThen().accept(this);
      else
        iftee.getElse().accept(this);
    } else
      throw new SemanticDynamicErrorException(
          iftee.getLine(), iftee.getColumn(), ErrorMessages.IF_COND_NOT_BOOL);
  }

  public void visit(ExprList el) {
    AnyType [] children = visitAndGetChildren(el);

    if (children.length == 1)
      push(children[0]);
    else
      push(new SequenceType(children));
  }

  public void visit(NonExpressionNode n) {
    visitChildren(n);
  }
  
  
  
  public AnyType peek() {
    if (!stack.isEmpty())
      return stack.peek();
    return null;
  }

  public FunctionTable getFunctionTable() {
    return func_tab;
  }

  public StackFrame currentScope() {
    return sym_tab.currentScope();
  }

  private void push(AnyType item) {
    stack.push(item);
  }

  private AnyType pop() {
    return stack.pop();
  }
 
  
  private void visitChildren(ASTNode n) {
    for (int i = 0; i < n.numChildren(); i++)
      n.getChild(i).accept(this);
  }
  
  private AnyType[] visitAndGetChildren(ASTNode n) {
    int pre_visit_num_children = stack.size();
    visitChildren(n);
    AnyType[] children = new AnyType[stack.size() - pre_visit_num_children];
    for (int i = children.length - 1; i >= 0; --i)
      children[i] = pop();
    return children;
  }

  private boolean instanceOf(Class type, Object ... classes) {
    for (Object o : classes)
      if (!type.isInstance(o))
        return false;
    return true;
  }

  private boolean hasDecimalType(AnyType ... numbers) {
    for (AnyType n : numbers)
      if (Util.sameClass(DecimalType.class, n.getClass()))
        return true;
    return false;
  }

  private void checkNumArgs(AnyType [] args, int num) {
    if (args.length != num) {
      throw new RuntimeException("Wrong number of arguments: " + args.length
                                 + " for " + num);
      // TODO: raise an error
    }
  }  
}
