#!/usr/bin/perl -w

use strict;

my $current_class = "";
print "digraph Heirarchy {\nrankdir=RL;\n";

while(<>) {

  if ( $_ =~ /class\s+(\w+)\s*:/ ) {
    $current_class = $1;
  }

  if ( $_ =~ /class\s+(\w+)\s*:.*public\s+virtual\s+(\w+(::\w+)?).*public\s+virtual\s+(\w+(::\w+)?).*public\s+virtual\s+(\w+(::\w+)?).*/) {
    if($1 ne "Null") {
      print "$1 -> $2;\n";
      print "$1 -> $4;\n";
      print "$1 -> $6;\n";
    }
  }
  elsif ( $_ =~ /class\s+(\w+)\s*:.*public\s+virtual\s+(\w+(::\w+)?).*public\s+virtual\s+(\w+(::\w+)?).*/) {
    if($1 ne "Null") {
      print "$1 -> $2;\n";
      print "$1 -> $4;\n";
    }
  }
  elsif ( $_ =~ /class\s+(\w+)\s*:.*public\s+virtual\s+(\w+(::\w+)?).*/) {
    if($1 ne "Null") {
      print "$1 -> $2;\n";
    }
  }
  elsif ( $_ =~ /unregisterHandler\(\s*(\w+Handler)\W/ ) {
    if($current_class ne "Null") {
      print "$1 [shape=box];\n";
      print "$1 -> $current_class [style=dashed];\n";
    }
  }

}

print "}\n";
