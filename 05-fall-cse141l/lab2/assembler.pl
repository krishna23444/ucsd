#!/usr/bin/perl --

my %reg3 = (
	'r0' => '000',
	'r1' => '001',
	'r2' => '010',
	'r3' => '011',
	'zero' => '100',
	'one' => '101',
	'three' => '110',
	'res' => '111' );

my %reg2 = (
	'r0' => '00',
	'r1' => '01',
	'r2' => '10',
	'r3' => '11' );

my $input_file = $ARGV[0];
$input_file =~ m/^(.*)\.s$/;
my $combined_file = $ARGV[1];
my $machine_file = $1 . ".imi";
my $pc = 0; #program counter

#read in the assembly code
open(INPUT, $input_file);
@code = <INPUT>;
close(INPUT);

#open output files
open(CMOUT, ">".$combined_file);
open(MCOUT, ">".$machine_file);

#find all branch labels
my %labels;
foreach(@code){
	$line = $_;
	if(!line || $line =~ m/^\#.*/ || $line =~ m/^\s*$/){
		next;
	}
	if($line =~ m/^(.*):(.*)$/){
		$labels{$1} = $pc;
	}
	$pc ++;
}
    
$pc = 0;
foreach(@code){
	$line = $_;
	if(!line || $line =~ m/^\#.*/ || $line =~ m/^\s*$/){
		print CMOUT $line;
		next;
	}
	my $mcode = "00000000"; #machine code
	my $hcode; #hex code
	#halt
	if($line =~ m/^\s*(\S+:)*\s*hlt.*/){
		$mcode = "01111101";
	}
	#one shot in
	else {if($line =~ m/^\s*(\S+:)*\s*inch.*/){
		$mcode = "01111100";
	}
	#branch to 17 if less than
	else {if($line =~ m/^\s*(\S+:)*\s*blt17.*/){
		$mcode = "01111101";
	}
	else {if($line =~ m/^\s*(\S+:)*\s*clrr.*/){
		$mcode = "01111110";
	}
	#2 register operand instructions
	else {if($line =~ m/^\s*(\S+:)*\s*(\S*)\s*\$(\S*)\s*,\s*\$(\S*)\s*.*$/){
		if($2 eq 'add'){
			$mcode = '001' . $reg3{$3} . $reg2{$4};
		}
		if($2 eq 'sub'){
			$mcode = '010' . $reg3{$3} . $reg2{$4};
		}
		if($2 eq 'ld'){
			$mcode = '1100'.$reg2{$3} . $reg2{$4};
		}
		if($2 eq 'ldn'){
			$mcode = '1101'.$reg2{$3} . $reg2{$4};
		}
		if($2 eq 'st'){
			$mcode = '1110'.$reg2{$3} . $reg2{$4};
		}
		if($2 eq 'stn'){
			$mcode = '1111'.$reg2{$3} . $reg2{$4};
		}
        if($2 eq 'mov'){
            $mcode = '101'.$reg2{$3} . $reg3{$4};
        }
	}
	#1 register operand instuctions
	else {if($line =~ m/^\s*(\S+:)*\s*(\S*)\s*\$(\S*)\s*.*$/){
		if($2 eq 'srl4'){
			$mcode = '01100' . $reg3{$3};
		}
		if($2 eq 'srl'){
			$mcode = '01101' . $reg3{$3};
		}
		if($2 eq 'sll4'){
			$mcode = '01110' . $reg3{$3};
		}
		if($2 eq 'srl3'){
			$mcode = '011110' . $reg2{$3};
		}
    if($2 eq 'and1'){
      $mcode = '10010' . $reg3{$3};
    }
	}
	#branches
	else {if($line =~ m/^\s*(\S+:)*\s*(\S*)\s*(\S*)\s*.*$/){
		if($2 eq 'blt'){
			my $offset = $pc - $labels{$3};
			$mcode = '000' . DecToBin5($offset);
		}
		if($2 eq 'bge'){
			my $offset = $labels{$3} - $pc;
			$mcode = '1000'. DecToBin($offset);
		}
	}}}}}}}
	
	#form the line
	$hcode = BinaryToHex($mcode);
	$out = "$hcode  $pc  $line";
	print MCOUT "$hcode\n";
	print CMOUT $out;
	$pc ++;
}

close(CMOUT);
close(MCOUT);

#Takes an 8bit binary word string and turns it into a hexadecimal string
sub BinaryToHex{
	my %hex = (
		'0000' => '0',
		'0001' => '1',
		'0010' => '2',
		'0011' => '3',
		'0100' => '4',
		'0101' => '5',
		'0110' => '6',
		'0111' => '7',
		'1000' => '8',
		'1001' => '9',
		'1010' => 'A',
		'1011' => 'B',
		'1100' => 'C',
		'1101' => 'D',
		'1110' => 'E',
		'1111' => 'F' );

	my ($bin) = @_;
	$bin =~ m/^(....)(....)$/;
	$hex = '0x' . $hex{$1} . $hex{$2};
	
	return $hex;
}

#Takes a decimal integer and returns a 4 bit binary string
sub DecToBin {
    ($num) = @_;
    my $str = sprintf("%b", $num);
    $str =~ s/^0+(?=\d)//;
	$padded = sprintf("%0*d", 4, $str);
    return $padded;
}

#As above but returns 5 bit bin string
sub DecToBin5 {
    ($num) = @_;
    my $str = sprintf("%b", $num);
    $str =~ s/^0+(?=\d)//;
	$padded = sprintf("%0*d", 5, $str);
    return $padded;
}

