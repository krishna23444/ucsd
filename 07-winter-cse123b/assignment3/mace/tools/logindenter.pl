#!/usr/bin/perl -w

use strict;

my %indent;
my $indentSize = 2;

while (<>) {

    if ($_ =~ /(\d+\.\d+|\[.*?\]) (\d{2}) (.*ENDING.*)/) {
        $indent{$2} -= $indentSize;
    }

    if ($_ =~ /(\d+\.\d+|\[.*?\]) (\d{2}) (.*)/) {
        if (not defined($indent{$2})) {
            $indent{$2} = 0;
        }
        print "$1 $2 ". " "x$indent{$2} . $3 . "\n";
    }

    if ($_ =~ /(\d+\.\d+|\[.*?\]) (\d{2}) (.*STARTING.*)/) {
        $indent{$2} += $indentSize;
    }

}
