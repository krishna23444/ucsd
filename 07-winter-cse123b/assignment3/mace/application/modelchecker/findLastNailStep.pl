#!/usr/bin/perl -w

use strict;


my $lastStep = 0;
my $lastNail = shift;

my $startLog = 0;
while (my $l = <>) {
    chomp($l);
    if ($l =~ m|\[main\] Starting simulation|) {
	$startLog = 1;
    }
    if ($startLog) {
# 	if ($l =~ m|^(\S+) .* \[SimulateEventBegin\] (\d+) (\d+) (\w+)|) {
	if ($l =~ m|\[main\] Now on simulator step (\d+)|) {
	    $lastStep = $1;
	}
	elsif ($l =~ m|ReplayRandomUtil.*currentPos (\d+)|) {
	    if ($1 == $lastNail) {
		print "step $lastStep is last nail for position $lastNail\n";
		exit(0);
	    }
	}
    }
}
