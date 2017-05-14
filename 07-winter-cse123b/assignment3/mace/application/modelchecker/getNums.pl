#!/usr/bin/perl -w 

use strict;

my $type = shift @ARGV;
my $phaseMatch = shift @ARGV;
my $phaseNum = 0;

while(<>) {
  if($phaseMatch == -1 || $phaseNum == $phaseMatch) {
    if(/$type \[ (.* )\]/) {
      print $1;
    }
  }
  if(/Now on phase (\d+) :/) {
    $phaseNum = $1;
  }
}
