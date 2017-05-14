0000 0000 1111 1111	= 255
0000 0001 1111 1100	= 510

1000 0000 0000 0000	= 32k



32 divided by 16	= 2
0000 0000 0010 0000 	= shift 4 right is the divide by 16
0000 0000 0000 0010	= 2


31 divided by 16
0000 0000 0001 1111	= 31
0000 0000 0000 0001 1111=decimal = 15 leftover

0000 0000 0000 1111	= 15 = remainder


>= 0.5 is 8 and obove remainder

plus 1 on the original, so 1 + 1 = 2





---------------------------------------------

k = array number of starting

for (i = 0; i < 16; i++) {
	sum = a[k + i];
}

avg = sum / 16

r = 16 - (sum % 16)

if (r <= 8) {

	avg++;	
}

array[2] = avg;

---------------------------------------------
*we are working with 8bit everything

$s1 = the array
$s2 = ongoing sum (lower)
$s3 = ongoing sum (upper)
$t1 = i


---------------------------------------------

		load	$t0, $s1		# store array[0] into $t0 for the start address
		add	$t1, $zero, $zero	# set $t1 to 0
		add	$s2, $zero, $zero	# set $s2 (lower) to 0

		add	$s3, $zero, $zero	# set $s3 (upper) to 0

						# time to start adding
inner:		load	$t2, $t1 ($t1)		# load array[$t1]

						# we're going to want to:
						# 1: set most sig bit to 0, then add to determine c_in [8th bit]

						# 2: add c_in with the 2 most sig bits to see if it is >= 2
						# 3: if so, increment the upper bit by 1

		and	$t3, $t2, 0x7F		# set array[$t1] most sig bit to 0, store in $t3
		and	$t4, $s2, 0x7F		# set sum(lower) most sig bit to 0, store in $t4

		add	$t3, $t4, $t3		# add them together, store in $t3
		srl	$t3, $t3, 7		# shift right 8 bits to get sig bit into 2^0 bit, $t3=c_in

		and	$t4, $t2, 0x80		# set $t4 to most sig bit of array[$t1]
		srl	$t4, $t4, 7		# shift it into the 2^0 bit, $t4 = a[most sig bit]


		and	$t5, $s2, 0x80		#
		srl	$t5, $t5, 7		# $t5 = $s2 [most sig bit]

		add	$t3, $t4, $t3		# $t3 = c_in + a[most sig bit]
		add	$t3, $t3, $t5		# $t3 = c_in + a[most sig bit] + sum [most sig bit]
		bl	$t3, 2, skipinc		# branch on less than 2, else increment upper


		addi	$s3, $s3, 1		# increment upper because of carryover from adding

skipinc:	addi	$t1, $t1, 1		# i++
		bl	$t1, 16, inner		# return to loop if < 16

						# now divide, takes some tricks with this 8bit stuff

		and	$t2, $s2, 0x0F		# put least 4 dig into $t2 of lower sum
		srl	$s2, $s2, 4		# shift sum (lower) 4 bits
		add	$t3, $zero, $zero	# zero out $t3
		and	$t3, $s3, 0x0F		# put least 4 dig into $t3 of upper sum

		sll	$t3, $t3, 4		# shift it 4 bits to the left

		and	$s2, $s2, 0x0F		# 0 out the 4 top bits of lower sum
		or	$s2, $s2, $t3		# put lower 4 bits of upper sum to upper 4 bits of lower sum
						# NOTE: max will be 12 bits, therefore all in lower sum is all bits

						# lower sum is now the XXX.decimal of the division
		
						# now deal with the remainder > 0.5 round up
		bge	$t3, 9, skipsuminc	# if remainder > 8, no need to increment

		addi	$s2, $s2, 1		# add 1 more to round up because remainder > 
0.5

skipsuminc:					# $s2 is now equal to the average!