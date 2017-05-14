#!/usr/bin/perl -w

use strict;

use lib $ENV{HOME} . '/mace/perl5';

use Carp;
use Mace::Util qw(:all);
use Mail::Sendmail;
use Getopt::Long;
use Sys::Hostname;
use File::Basename;
use Cwd;

my $email = "";
Getopt::Long::Configure("pass_through");
GetOptions("email=s" => \$email);

my $config = shift(@ARGV) || printUsage();

my %initargs = @ARGV;

# use Data::Dumper;
# print Dumper(%initargs);
# exit(0);

my %p = ();
my @order = ();
Mace::Util::loadConfigHash($config, \%p, \@order);

# use Data::Dumper;
# print Dumper(%p);

my @vals = ();
for my $k (@order) {
    my $v = $p{$k};
    my @t = split(/ /, $v);
    my @a = ();
    for my $el (@t) {
	if ($el =~ m|^(\d+)\.\.(\d+)$|) {
	    my $start = $1;
	    my $end = $2;
	    for my $i ($start..$end) {
		push(@a, $i);
	    }
	}
	else {
	    push(@a, $el);
	}
    }
    push(@vals, [$k, \@a]);
}

# print Dumper(@vals);

my @arglist = ();
genArgs(\@vals);

my $path = dirname($0);
# if ($0 =~ m|(.*)/check-multi.pl|) {
#     $path = $1;
# }
my $mc = "$path/run-modelchecker.pl";

for my $el (@arglist) {
    my $cmd = "$mc $el";
    print "$cmd\n";
    if (my $pid = fork()) {
	waitpid($pid, 0);
    }
    else {
	exec($cmd);
    }
    my $returnStatus = $? >> 8;
    if ($returnStatus != 0) {
	print "exiting on args $el\n";

	if ($email) {
	    my $hostname = hostname();
	    my $cwd = getcwd();
	    my $body =<<EOF;
modelchecker failed on $el
$hostname:$cwd
EOF
	    my %mail = ( To => $email,
			 From => $email,
			 Subject => "modelchecker failed",
			 Message => $body,
			 );
	    sendmail(%mail) or die $Mail::Sendmail::error;
	}
	
	exit(0);
    }
}


sub genArgs {
    my $va = shift;
    my $s = shift || "";
    if (scalar(@$va)) {
	my $l = shift(@$va);
	my ($k, $a) = @$l;
	my $useInit = 0;
	my $test = 0;
	if (defined($initargs{"-$k"})) {
	    $useInit = 1;
	    $test = 1;
	}
	for my $el (@$a) {
	    if ($useInit && $test) {
		if ($el eq $initargs{"-$k"}) {
		    $test = 0;
		}
		else {
# 		    print "skipping $k $el\n";
		    next;
		}
	    }
	    delete($initargs{"-$k"});
	    genArgs($va, "$s -$k $el");
	}
	unshift(@$va, $l);
    }
    else {
	push(@arglist, $s);
    }
}

sub printUsage {
    my $usage =<<EOF;
usage: $0 config [-e email] [-initial_param val [...]]
EOF

    die $usage;
}
