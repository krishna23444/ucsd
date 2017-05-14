public class prog1
{
	public static void main(String[] args)
	{
		double sum = 0., sumsq = 0., product = 1., num = 0.;
		int i = 0, n = 0;

//		READ 2 NUMBERS
		n = 2;
		System.out.println("ENTER " + n + " NUMBERS, ONE PER LINE:");
		sum = 0.; sumsq = 0.; product = 1.;
		for(i = 0; i < n; i++)
		{
			num = SavitchIn.readLineDouble();
			sum += num; sumsq += num*num; product *= num;
		}

//		WRITE THE SUM
		System.out.println("THE SUM IS: " + sum);
	}
}
