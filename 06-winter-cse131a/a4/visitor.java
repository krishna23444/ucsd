public interface Visitor {
  void visit(ArgumentDeclaration ad);
  void visit(Constant c);
  void visit(ExpressionNode e);
  void visit(ExprList el);
  void visit(Flwr f);
  void visit(ForAssignment fa);
  void visit(FunctionCall fc);
  void visit(FunctionDeclaration fd);
  void visit(IfThenElseExpr i);
  void visit(LetAssignment la);
  void visit(NonExpressionNode n);
  void visit(Operator o);
  void visit(Query q);
  void visit(Return r);
  void visit(Variable v);
  void visit(VariableDeclaration vd);
  void visit(Where w);
}
