import java_cup.runtime.*;

/**
 * Lexical scanner for the Onyx xml language.
 */
%%

%class Scanner
%cupsym sym
%unicode
%cup
%line
%column

%state STRING

%{
  int line, col;
  String str;

  private Symbol symbol(int type) {
    return new Symbol(type, yyline+1, yycolumn+1);
  }
  private Symbol symbol(int type, Object value) {
    return new Symbol(type, yyline+1, yycolumn+1, value);
  }
  private Symbol symbol(int type, int line, int column, Object value) {
    return new Symbol(type, line+1, column+1, value);
  }
%}

Boolean    = "true" | "false"
Letter     = [:letter:]
Digit      = [0-9]
HexDigit   = [0-9a-fA-F]
QName      = ({Letter} | "_") ({Letter} | {Digit} | "." | "-" | "_")*
Whitespace = [ \t\r\n]+

%%

<YYINITIAL> {
  /* keywords */
"declare"       { return symbol(sym.DECLARE); }
"function"      { return symbol(sym.FUNCTION); }
"variable"      { return symbol(sym.VARIABLE); }
"as"            { return symbol(sym.AS); }
"if"            { return symbol(sym.IF); }
"then"          { return symbol(sym.THEN); }
"else"          { return symbol(sym.ELSE); }
"for"           { return symbol(sym.FOR); }
"let"           { return symbol(sym.LET); }
"where"         { return symbol(sym.WHERE); }
"return"        { return symbol(sym.RETURN); }
"in"            { return symbol(sym.IN); }
"to"            { return symbol(sym.TO); }
"except"        { return symbol(sym.EXCEPT); }
"before"        { return symbol(sym.BEFORE); }
"after"         { return symbol(sym.AFTER); }
"ordered-by"    { return symbol(sym.ORDEREDBY); }
"descending"    { return symbol(sym.DESCENDING); }
"ascending"     { return symbol(sym.ASCENDING); }
"intersect"     { return symbol(sym.INTERSECT); }
"union"         { return symbol(sym.UNION); }
"le"            { return symbol(sym.LE); }
"lt"            { return symbol(sym.LT); }
"ge"            { return symbol(sym.GE); }
"gt"            { return symbol(sym.GT); }
"eq"            { return symbol(sym.EQ); }
"ne"            { return symbol(sym.NE); }
"and"           { return symbol(sym.AND); }
"or"            { return symbol(sym.OR); }
"div"           { return symbol(sym.DIV); }
"idiv"          { return symbol(sym.IDIV); }
"mod"           { return symbol(sym.MOD); }

  /* Punctuation and operator symbols */
"/"             { return symbol(sym.SLASH); }
"//"            { return symbol(sym.SLASHSLASH); }
"="             { return symbol(sym.EQUALS); }
"!="            { return symbol(sym.NOTEQUALS); }
"<="            { return symbol(sym.LESSEQUALS); }
">="            { return symbol(sym.GREATEREQUALS); }
"<"             { return symbol(sym.LESS); }
">"             { return symbol(sym.GREATER); }
":="            { return symbol(sym.COLONEQUALS); }
"*"             { return symbol(sym.MULT); }
"-"             { return symbol(sym.MINUS); }
"+"             { return symbol(sym.PLUS); }
"|"             { return symbol(sym.VERTICALBAR); }
","             { return symbol(sym.COMMA); }
"("             { return symbol(sym.LPAREN); }
")"             { return symbol(sym.RPAREN); }
"["             { return symbol(sym.LSQUARE); }
"]"             { return symbol(sym.RSQUARE); }
"{"             { return symbol(sym.LCURLY); }
"}"             { return symbol(sym.RCURLY); }
";"             { return symbol(sym.SEMICOLON); }

  /* QNames, Variables, and Literals */
{Boolean}             { return symbol(sym.BOOLEANLITERAL, yytext()); }

{QName}               { return symbol(sym.QNAME, yytext()); }

"$" {QName}           { return symbol(sym.TOKENVARIABLE, yytext()); }

{Digit}+              { return symbol(sym.INTEGERLITERAL, yytext()); }

("." ({Digit})+) | (({Digit})+ "." ({Digit})*)
                      { return symbol(sym.DECIMALLITERAL, yytext()); }

  /* Strings */
\"                    { str = new String(); yybegin(STRING);
                        line = yyline; col = yycolumn; }

  /* Comments */
\{-- [^]* ~--\}       { /* Not Tokenized */ }

  /* Whitespace */
{Whitespace}          { /* Not Tokenized */ }

  /* Lexical Errors */
[^]                   { return symbol(sym.LEXICALERROR, yytext()); }
}

<STRING> {
  \"                    { yybegin(YYINITIAL);
                          return symbol(sym.STRINGLITERAL, line, col, str); }
  \"\"                  { str += "\""; }
  "&lt;"                { str += "<"; }
  "&gt;"                { str += ">"; }
  "&amp;"               { str += "&"; }
  "&quot;"              { str += "\""; }
  "&apos;"              { str += "'"; }
  "&#" {Digit}+ ";"     { str += String.valueOf(Character.toChars(
                                 Integer.parseInt(yytext().substring(2,
                                                  yytext().length() - 1))));
                        }
  "&#x" {HexDigit}+ ";" { str += String.valueOf(Character.toChars(
                                  Integer.parseInt(yytext().substring(3,
                                                  yytext().length() - 1), 16)));
                        }
  [^]                   { str += yytext(); }
}
