/*
 * FiForward.java
 *
 * Created on October 3, 2003, 12:59 AM
 */

//package P1;

/**
 *
 * @author  Nitay J
 */
public class FiForward {
    /** Creates a new instance of FiForward */
    public FiForward() {
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {      
        //declare variables and initialize them to 0
        double startAmnt=0, intRate=0, endAmnt=0;
        int yearsInv=0, cmpdsYear=0;
        
        //collect input from user
        System.out.print("Enter initial investment in $: ");
        startAmnt=SavitchIn.readLineDouble();
        System.out.print("Enter annual interest rate in %: ");
        intRate=SavitchIn.readLineDouble();
        System.out.print("Enter investment duration in years: ");
        yearsInv=SavitchIn.readLineInt();
        System.out.print("Enter number of times per year interest is compounded: ");
        cmpdsYear=SavitchIn.readLineInt();
        
        //compute ending amount
        endAmnt=FiForward.compoundAmount(startAmnt, intRate, cmpdsYear, yearsInv);
        
        //output the initial values given
        System.out.println("\n An initial investment of $" + startAmnt);
        System.out.println(" invested for " + yearsInv + " years at " + intRate + "%");
        System.out.println(" compounded " + cmpdsYear + " times per year becomes:");
        
        //round off result and output it
        double roundedEndAmnt = Math.round(endAmnt*100) / 100.0;
        System.out.println(" $" + roundedEndAmnt);
    }
    
    public static double compoundAmount(double startAmnt,
                                        double annualRate,
                                        int cmpdsPerYear,
                                        int years) 
    {
        //set ending amount = starting amount
        double endAmnt=startAmnt;                         
        
        //calculate interest used in every compound period
        double interest=annualRate/(cmpdsPerYear*100);
        
        //calculate total number of compound periods
        long numCmpds = cmpdsPerYear * years;
        
        //apply interest N times, N = years * compounds per year
        for(int i=0; i < numCmpds; i++)
            endAmnt *= 1 + interest;
        
        return endAmnt;
    }
}
