import java.io.*;
import java_cup.runtime.*;

/**
 * Lexical scanner for the Onyx xml language.
 */
%%

%class Scanner
%cupsym onyx_sym
%unicode
%cup
%line
%column

%state STRING

%{
  int line, col;
  String str;

  private Symbol symbol(int type) {
    return new Symbol(type, yyline, yycolumn);
  }
  private Symbol symbol(int type, Object value) {
    return new Symbol(type, yyline, yycolumn, value);
  }
  private Symbol symbol(int type, int line, int column, Object value) {
    return new Symbol(type, line, column, value);
  }
%}

Boolean    = "true" | "false"
Letter     = [a-zA-Z]
Digit      = [0-9]
HexDigit   = [0-9a-fA-F]
QName      = ({Letter} | "_") ({Letter} | {Digit} | "." | "-" | "_")*
Whitespace = [ \t\r\n]+

%%

<YYINITIAL> {
  /* keywords */
"declare"       { return symbol(onyx_sym.DECLARE); }
"function"      { return symbol(onyx_sym.FUNCTION); }
"variable"      { return symbol(onyx_sym.VARIABLE); }
"as"            { return symbol(onyx_sym.AS); }
"if"            { return symbol(onyx_sym.IF); }
"then"          { return symbol(onyx_sym.THEN); }
"else"          { return symbol(onyx_sym.ELSE); }
"for"           { return symbol(onyx_sym.FOR); }
"let"           { return symbol(onyx_sym.LET); }
"where"         { return symbol(onyx_sym.WHERE); }
"return"        { return symbol(onyx_sym.RETURN); }
"in"            { return symbol(onyx_sym.IN); }
"to"            { return symbol(onyx_sym.TO); }
"except"        { return symbol(onyx_sym.EXCEPT); }
"before"        { return symbol(onyx_sym.BEFORE); }
"after"         { return symbol(onyx_sym.AFTER); }
"ordered-by"    { return symbol(onyx_sym.ORDEREDBY); }
"descending"    { return symbol(onyx_sym.DESCENDING); }
"ascending"     { return symbol(onyx_sym.ASCENDING); }
"intersect"     { return symbol(onyx_sym.INTERSECT); }
"union"         { return symbol(onyx_sym.UNION); }
"le"            { return symbol(onyx_sym.LE); }
"lt"            { return symbol(onyx_sym.LT); }
"ge"            { return symbol(onyx_sym.GE); }
"gt"            { return symbol(onyx_sym.GT); }
"eq"            { return symbol(onyx_sym.EQ); }
"ne"            { return symbol(onyx_sym.NE); }
"and"           { return symbol(onyx_sym.AND); }
"or"            { return symbol(onyx_sym.OR); }
"div"           { return symbol(onyx_sym.DIV); }
"idiv"          { return symbol(onyx_sym.IDIV); }
"mod"           { return symbol(onyx_sym.MOD); }

  /* Punctuation and operator symbols */
"/"             { return symbol(onyx_sym.SLASH); }
"//"            { return symbol(onyx_sym.SLASHSLASH); }
"="             { return symbol(onyx_sym.EQUALS); }
"!="            { return symbol(onyx_sym.NOTEQUALS); }
"<="            { return symbol(onyx_sym.LESSEQUALS); }
">="            { return symbol(onyx_sym.GREATEREQUALS); }
"<"             { return symbol(onyx_sym.LESS); }
">"             { return symbol(onyx_sym.GREATER); }
":="            { return symbol(onyx_sym.COLONEQUALS); }
"*"             { return symbol(onyx_sym.MULT); }
"-"             { return symbol(onyx_sym.MINUS); }
"+"             { return symbol(onyx_sym.PLUS); }
"|"             { return symbol(onyx_sym.VERTICALBAR); }
","             { return symbol(onyx_sym.COMMA); }
"("             { return symbol(onyx_sym.LPAREN); }
")"             { return symbol(onyx_sym.RPAREN); }
"["             { return symbol(onyx_sym.LSQUARE); }
"]"             { return symbol(onyx_sym.RSQUARE); }
"{"             { return symbol(onyx_sym.LCURLY); }
"}"             { return symbol(onyx_sym.RCURLY); }
";"             { return symbol(onyx_sym.SEMICOLON); }

  /* QNames, Variables, and Literals */
{Boolean}             { return symbol(onyx_sym.BOOLEANLITERAL, yytext()); }

{QName}               { return symbol(onyx_sym.QNAME, yytext()); }

"$" {QName}           { return symbol(onyx_sym.VARNAME, yytext()); }

{Digit}+              { return symbol(onyx_sym.INTEGERLITERAL, yytext()); }

("." ({Digit})+) | (({Digit})+ "." ({Digit})*)
                      { return symbol(onyx_sym.DECIMALLITERAL, yytext()); }

  /* Strings */
\"                    { str = new String(); yybegin(STRING);
                        line = yyline; col = yycolumn; }

  /* Comments */
\{-- .* ~--\}         { /* Not Tokenized */ }

  /* Whitespace */
{Whitespace}          { /* Not Tokenized */ }

  /* Lexical Errors */
.                     { return symbol(onyx_sym.LEXICALERROR, yytext()); }
}

<STRING> {
  \"                  { yybegin(YYINITIAL);
                        return symbol(onyx_sym.STRINGLITERAL, line, col, str); }
  \"\"                { str += "\""; }
  "&lt;"              { str += "<"; }
  "&gt;"              { str += ">"; }
  "&amp;"             { str += "&"; }
  "&quot;"            { str += "\""; }
  "&apos;"            { str += "'"; }
  "&#" {Digit}+ ";"   { str += String.valueOf(Character.toChars(
                                Integer.parseInt(yytext().substring(2,
                                                  yytext().length() - 1))));
                      }
  "&#x" {HexDigit}+ ";" { str += String.valueOf(Character.toChars(
                                  Integer.parseInt(yytext().substring(3,
                                                  yytext().length() - 1), 16)));
                        }
  .                   { str += yytext(); }
}
