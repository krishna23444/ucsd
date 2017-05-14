/*
 * FiBackward.java
 *
 * Created on October 3, 2003, 12:59 AM
 */

//package P1;

/**
 *
 * @author  Nitay J
 */
public class FiBackward {
    
    /** Creates a new instance of FiBackward */
    public FiBackward() {
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        //declare variables and initialize them to 0
        double startAmnt = 0, intRate = 0, endAmnt = 0;
        int years = 0, cmpdsYear = 0;
        
        //collect input from user
        System.out.print("Enter desired goal in $: ");
        endAmnt = SavitchIn.readLineDouble();
        System.out.print("Enter annual interest rate in %: ");
        intRate = SavitchIn.readLineDouble();
        System.out.print("Enter investment duration in years: ");
        years = SavitchIn.readLineInt();
        System.out.print("Enter number of times per year interest is compounded: ");
        cmpdsYear = SavitchIn.readLineInt();
        
        //compute ending amount
        startAmnt=FiBackward.compoundAmount(endAmnt, intRate, cmpdsYear, years);
        
        //output the initial values given
        System.out.println("\n A goal of $" + endAmnt +" after " + years + " years");
        System.out.println(" with an APR of " + intRate + "% compounded " + cmpdsYear + " times per year");
        System.out.println(" can be achieved with an initial investment of:");
        
        //round off result and output it
        double roundedStartAmnt = Math.round(startAmnt*100) / 100.0;
        System.out.println(" $" + roundedStartAmnt);
    }
    
    public static double compoundAmount(double endAmnt,
                                        double annualRate,
                                        int cmpdsPerYear,
                                        int years) 
    {
        //set starting amount = ending amount
        double startAmnt=endAmnt;                         
        
        //calculate interest used in every compound period
        double interest=annualRate/(cmpdsPerYear*100);
        
        //calculate total number of compound periods
        long numCmpds = cmpdsPerYear * years;
        
        //apply interest (in reveres) N times, N = years * compounds per year
	for(int i=0; i < numCmpds; i++)
	  startAmnt /= 1 + interest;

        return startAmnt;
    }
}
