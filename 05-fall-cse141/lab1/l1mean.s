# r0 	<->	start address
# r1	<->	for loop count
# r2	<->	current value to add
# r3	<->	number of elements

		ld	$r0, $zero	# r0 = start address = d[0]
		ld	$r3, $one	# get length of set
					# assume variables are cleared

inner:		ld	$r2, [$r0]	# load in the new add value
		add	$dstl, $r2	# dstl += r2
		inch			# increment dsth iff overflow = 1

		add	$one, $r1	# ++i
		cmp	$r1, $end
		blt	inner

# r0	<->	start address
# r1	<->	0x0F (16-1 = 15)
# r2	<->
# r3	<->	number of elements
# note: we're assuming 16 registers anyway, so take advantage of it

		mov	$r2, $dstl	# r2 = dstl temporarily
		sub	$r3, $one	# r3 (16) - 1 = 0x0F

		mov	$r1, $dstl	# r1 = 0x0F
		mov	$dstl, $r2	# dstl = r2 (back to sum)

		and	$r2, $r1	# r2 is now remainder
		srl4	$dstl		# dstl / 4
		sll4	$dsth		# shift upper 4 bits left
		add	$dstl, $dsth	# add upper to lower

		srl	$r3		# r3 = 8 (16/2)
		add	$one, $r3	# r3 = 9

# r0	<->	start address
# r1	<->	0x0F (16-1 = 15)
# r2	<->	remainder
# r3	<->	9, to determine if you need to increment average

		cmp	$r2, $r3
		bge	skipsuminc

		add	$one, $dstl

skipsuminc:	add	$one, $r0	# d[1]
		add	$one, $r0	# d[2]
		st	[$r0], $dstl	# store averge in


