/*
 * Author: nitay
 * Date: Mar 9, 2006
 * Project: compilers_a3
 */

/**
 * 
 */
public class ErrorMessages {
  public static final String DIV_BY_ZERO =
    "Division by zero is invalid";

  public static final String MOD_BY_NON_POS =
    "Modulus value must be greater than zero";
  
  public static final String IF_COND_NOT_BOOL =
    "If condition is not of type onyx.types.Boolean";
    
  public static final String WHERE_COND_NOT_BOOL =
    "Where condition is not of type onyx.types.Boolean";
  
  public static String functionPrototypeNotFound(String s) {
    return "Function with prototype " + s + " not found";
  }

  public static String functionAlreadyDefined(String name, String fp) {
    return "Function " + name + " with prototype " + fp + " already defined";
  }

  public static String globalVariableAlreadyDefined(String s) {
    return "Global variable " + s + " already defined";
  }

  public static String invalidOnyxAttribute(String s) {
    return "Attribute name " + s + " is an invalid OXML attribute name";
  }
  
  public static String invalidOnyxDocument(String s) {
    return "Error loading OXML data from file named " + s;
  }
  
  public static String invalidOnyxTagname(String s) {
    return "Tagname " + s + " is an invalid OXML tagname";
  }
  
  public static String invalidStringToNum(String s) {
    return '"' + s + "\" is not a valid number";
  }
  
  public static String invalidType(String s) {
    return s + " is not a valid Onyx type";
  }
  
  public static String invalidVariableAssignment(String s) {
    return "Type not assignable to " + s;
  }
  
  public static String unboundVariable(String s) {
    return "Variable " + s + " not bound";
  }
  
  public static String wrongReturnType(String name, String exp, String act) {
    return "Function " + name + " declared to return type " + exp
           + ", but found " + act;
  }
}
