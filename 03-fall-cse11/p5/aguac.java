/*
Project: P5
File: aguac.java
By: Nitay Joffe
Date: Nov 6, 2003
login: cs11fcy

Class Description:
 *aguac is a 'compiler' for the agua language. It takes as a parameter the name
 *of the file to read in from, checks that file for and syntax errors, and
 *compiles it into a java file with the same filename but a .java extension
 *instead of a .agua extension. If a syntax error is found, the method that
 *finds it throws a new instance of the SyntaxError class, and it is caught and
 *described in the main method.
*/

//BufferedReader, File, FileReader, FileWriter, PrintWriter
import java.io.*;

//StringTokenizer
import java.util.*;

public class aguac
{
  //input and output files
  private static BufferedReader fIn;
  private static PrintWriter fOut;
  
  //parses agua commands whose first token is equals to READ
  //uses StringTokenizer to split up the other tokens left in the line
  //and writes to fOut, assuming no syntax errors are found. Translates
  //commands in the form READ <n> NUMBERS into java.
  public static void aguaREADcmd(StringTokenizer st, int lineNum)
    throws SyntaxError
  {
    //test for correct number of tokens (2 because the first token was already
    //used and therefore the StringTokenizer's position in on the second token)
    if(st.countTokens() != 2)
      throw new SyntaxError("line " + lineNum + ": [number of tokens?]");

    //store second and third tokens inside String variables to work with later
    String secToken = st.nextToken(), thirdToken = st.nextToken();
    
    //use Integer.parseInt and Double.parseDouble to parse the second token
    //and check that it is a valid integer (by equality testing those values).
    //If the token is not syntaticly correct, the parse() methods will throw a
    //NumberFormatException, which is caught and translated into a SyntaxError
    try
    {
      int intValue = Integer.parseInt(secToken);
      double doubleValue = Double.parseDouble(secToken);

      //check for proper integer value token by comparing it to the double form
      if((double)intValue == doubleValue)
	//check for a possible negative sign
        if(intValue >= 0)
	  //make sure third token is syntaticly correct
          if(thirdToken.equals("NUMBERS"))
	  {
            fOut.println("\t\tn = " + intValue + ";");
            fOut.print("\t\tSystem.out.println(\"ENTER \" + n + \" ");
            fOut.println("NUMBERS, ONE PER LINE:\");");
            fOut.println("\t\tsum = 0.; sumsq = 0.; product = 1.;"); 
            fOut.println("\t\tfor(i = 0; i < n; i++)");
            fOut.println("\t\t{");
            fOut.println("\t\t\tnum = SavitchIn.readLineDouble();");
            fOut.print("\t\t\tsum += num; sumsq += num*num; product *= num;");
            fOut.println("\n\t\t}");
          }
          else throw new SyntaxError("line " + lineNum + "(token 3):");
        else throw new SyntaxError("line " + lineNum
                                   + " (token 2): [negative num?]");
      else throw new SyntaxError("line " + lineNum
                                 + " (token 2): [format of num?]");
    }

    //int and/or double conversion/parsing failed, throw a syntax error
    catch(NumberFormatException nfe)
    {
      throw new SyntaxError("line " + lineNum + " (token 2): [format of num?]");
    }
  }
  
  //parses agua commands whose first token is equals to WRITE
  //uses StringTokenizer to split up the other tokens left in the line
  //and writes to fOut, assuming no syntax errors are found. Translates
  //commands in the form WRITE THE <op>, where op is one of SUM, AVERAGE,
  //PRODUCT or STANDARD DEVIATION
  public static void aguaWRITEcmd(StringTokenizer st, int lineNum)
    throws SyntaxError
  {

    //line must be THE <op> where op is either AVERAGE, SUM, or PRODUCT since
    //there are only 3 tokens total (2 in this case b/c first one was already
    //used and therefore the StringTokenizer's position is on the second token)
    if(st.countTokens() == 2)
    {
      //store second and third tokens inside String variables to work with later
      String secToken = st.nextToken(), thirdToken = st.nextToken();
      
      //check syntax of second token
      if(secToken.equals("THE"))
      {
	//check third token for "AVERAGE" "SUM" or "PRODUCT", and output the
	//appropriate code depending on which one it is... or throw a new syntax
	//error if none of those fit since those are the only choices
        if(thirdToken.equals("AVERAGE"))
        {
          fOut.print("\t\tSystem.out.println(\"THE AVERAGE IS: \" + sum/n);");
        }
        else if(thirdToken.equals("SUM"))
        {
          fOut.println("\t\tSystem.out.println(\"THE SUM IS: \" + sum);");
        }
        else if(thirdToken.equals("PRODUCT"))
        {
          fOut.print("\t\tSystem.out.println(\"THE PRODUCT IS: \" +");
          fOut.println(" product);");
        }
        else throw new SyntaxError("line " + lineNum + " (token 3):");
      }
      else throw new SyntaxError("line " + lineNum + " (token 2):");
    }

    //line must be THE STANDARD DEVIATION, since that's the only statement
    //that has 4 tokens total (3 in this case b/c the first one has already been
    //used and therefore the StringTokenizer's position is on the second token)
    else if(st.countTokens() == 3)
    {
      //store tokens inside String variables to work with later
      String secToken = st.nextToken(), thirdToken = st.nextToken();
      String fourthToken = st.nextToken();
      
      //check syntax of second token
      if(secToken.equals("THE"))
	//check syntax of third token
        if(thirdToken.equals("STANDARD"))
	  //check syntax of fourth token
          if(fourthToken.equals("DEVIATION"))
          {
            fOut.print("\t\tSystem.out.println(\"THE STANDARD DEVIATION IS: ");
            fOut.println("\" + Math.sqrt(sumsq/n - (sum/n)*(sum/n)));");
          }
          else throw new SyntaxError("line " + lineNum + " (token 4):");
        else throw new SyntaxError("line " + lineNum + " (token 3):");
      else throw new SyntaxError("line " + lineNum + " (token 2):");
    }

    //line has the wrong number of tokens, too many or too few
    else
      throw new SyntaxError("line " + lineNum + ": [number of tokens?]");
  }
  
  //checks that the filename has the correct extension (.agua), and that the
  //filename is not equal to aguac.agua (that would cuase some problems...)
  public static String checkFilename(String file)
  {
    //location of last period in the string, if there are none then equal to -1
    int dotAt = file.lastIndexOf('.');
    
    //test that there is a period in the string
    if(dotAt > 0)
      //test string after period, should be equal to agua
      if(file.substring(dotAt + 1, file.length()).equals("agua"))
	//test that the filename is not equal to aguac.agua
        if(! (file.substring(0, dotAt).equals("aguac")) )
          //passed all the tests, return the classname, which = filename - .agua
	  return file.substring(0, dotAt);

    //if program gets here then filename must be wrong for one reason or another
    exit("Invalid filename format");
    return null;
  }
  
  //closes the input and output files, if they exist. Catches an IOException if the
  //input file failes to close, and prints out the exception to the screen
  public static void closeFiles()
  {
    if(fOut != null) fOut.close();
    if(fIn != null) 
    {
      try
      {
        fIn.close();
      }
      catch(IOException ioe)
      {
        System.out.println(ioe.getMessage());
      }
    }
  }
  
  //terminate the program. Calls closeFiles to make sure the input/output files
  //are closed, and prints the error string passed to the method (in case there
  //are any last comments/errors user should know of)
  public static void exit(String error)
  {
    closeFiles();
    if(error != null && !error.equals("")) System.out.println(error);
    System.exit(1);
  }
  
  //the main method, runs when 'java aguac <filename>' is called on the command
  //line. This method calls the appropriate public static helper methods to open
  //the files, translate if line-by-line, and close the files once finished. It
  //also catches any SyntaxErrors that might be thrown by the methods it uses,
  //and prints them out to screen along with closing the files, deleting the
  //output file (because it contains invalid/incomplete java code), and exiting
  public static void main(String[] args)
  {
    String className, lineInput = null;
    int lineNum = 0;
    
    //wrong number of parameters passed to aguac, either no filename, or too
    //many files were passed on the command line to the aguac program
    if(args.length != 1) exit("Incorrect number of arguements passed to aguac");

    //check syntax of args[0] (agua file), and extract java classname from it
    className = checkFilename(args[0]);

    //open the input (agua file), and output (java file) for reading/writing
    openFiles(args[0], className + ".java");

    //store the location of the output file in a File object so that we may
    //delete it later in the program if any syntax errors occur
    File outputFile = new File(className + ".java");

    //write the beginning part of the java file common to all agua programs
    writeHeader(className);
    
    //loop while there are any lines left to be read in the input agua file
    do
    {
      //read in the next line of agua file. Throws an IOException if an error
      //occurs during the read process, in which case we output the error
      //message and quit, because the file may be corrupt or whatnot
      try
      {
        lineInput = fIn.readLine();
      }
      catch(IOException ioe)
      {
        System.out.println(ioe.getMessage());
        exit("cannot read file = input/output exception thrown, exiting...");
      }

      //translate the line read in as long as it's not null ( = EOF)
      if(lineInput != null)
      {
	//increment the line number (used for outputting useful syntax errors)
        lineNum++;

	//translate the line. Throws a SyntaxError exception if there is a
	//syntax error in the agua file, in which case we print out the error,
	//along with useful information like line/token number and possible
	//reason for error, and try to delete the java file (since it is
	//incomplete and therefore contains bad code which just takes up space)
        try
        {
          translateLine(lineInput, lineNum);
        }
        catch(SyntaxError se)
        {
          //output syntax error message with the line/token number, possible
	  //reason for the error, and a copy of the line on which it occured
	  System.out.println("Syntax error: " + args[0] + ", "
                             + se.getMessage() + "\n" + lineInput);

	  //try to delete the output java file since it contains bad code which
	  //is just taking up space on the machine
	  try
	  {
	    outputFile.delete();
	  }
	  catch(SecurityException secE)
	  {
	    System.out.println(secE.getMessage());
	  }	  
	  
	  exit("");
        }
      }
    } while(lineInput != null);
    
    //write the ending part of the java file common to all agua programs
    writeFooter();
  
    //Compilation completed successfully, exit with a happy message
    exit(args[0] + " -- Successfully Compiled Into -- " + className + ".java");
  }
  
  //open the input and output files. Catches an IOException if the
  //output file failes to close, and prints out the exception to the screen
  //Also catches a FileNotFoundException if the input file cannot be opened
  //for whatever reason, and prints out the exception to the screen...
  //along with exiting of course since no compiling can be done
  public static void openFiles(String inputFile, String outputFile)
  {
    try
    {
      fIn = new BufferedReader(new FileReader(inputFile));
      fOut = new PrintWriter(new FileWriter(outputFile));
    }
    catch(FileNotFoundException fnfe)
    {
      System.out.println(fnfe.getMessage());
      exit("file not found exception thrown, exiting...");
    }
    catch(IOException ioe)
    {
      System.out.println(ioe.getMessage());
      exit("input/output exception thrown, exiting...");
    }
  }
    
  //takes a line input (from the .agua) file, and calls the appropriate
  //parse method (aguaWRITEcmd or aguaREADcmd), depending on the first token
  //in the line. Also checks for any syntax errors along the way, and throws
  //an instance of the SyntaxError class if one is found
  public static void translateLine(String line, int lineNum) throws SyntaxError
  {
    //tokenize the line passed to this method
    StringTokenizer st = new StringTokenizer(line);

    //line is white space... return from method/skip this line
    if(st.countTokens() == 0) return;
    
    //print a comment in the java file showing which line we're working on
    fOut.println("\n//\t\t" + line);

    //test for correct number of tokens in the line (must be 3 or 4)
    if(st.countTokens() < 3 || st.countTokens() > 4)
      throw new SyntaxError("line " + lineNum + ": [number of tokens?]");

    //store first token in the StringTokenizer (should be = WRITE or READ)
    String firstToken = st.nextToken();

    //test if the token is equal to "WRITE" or "READ", calling each method
    //appropriately. If the token is not equal to either, then there is a syntax
    //error... so throw a new instance of the SyntaxError class describing error
    if(firstToken.equals("WRITE")) aguaWRITEcmd(st, lineNum);
    else if(firstToken.equals("READ")) aguaREADcmd(st, lineNum);
    else throw new SyntaxError("line " + lineNum + " (token 1):");
  }
  
  //writes the ending part of the program which is common to all agua -> java
  //files. This basically prints the brace to end the main method, and the
  //brace to signify the end of the class
  public static void writeFooter()
  {
    fOut.println("\t}");
    fOut.println("}");
  }
  
  //writes the beginning part of the program which is common to all agua -> java
  //files. This basically prints out the class and main method headers, along
  //with any braces they require, and the variable declarations for all the
  //variables that might need to be used in possible READ/WRITE statements
  public static void writeHeader(String className)
  {
    fOut.println("public class " + className);
    fOut.println("{");
    fOut.println("\tpublic static void main(String[] args)");
    fOut.println("\t{");
    fOut.println("\t\tdouble sum = 0., sumsq = 0., product = 1., num = 0.;");
    fOut.println("\t\tint i = 0, n = 0;");
  }
}
