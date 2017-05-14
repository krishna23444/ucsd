#!/usr/bin/perl -w

use strict;

my $path = ".";
if ($0 =~ m|(.*)/run-modelchecker.pl|) {
    $path = $1;
}
my $mc = "$path/modelchecker @ARGV ";
my $gengraph = "$path/generateEventGraph.pl --uncolor";
my $findnail = "$path/findLastNailStep.pl";

my $out = "mc.log";
my $errorlog = "error.log";
my $livelog = "live.log";
my $errorgraph = "error.graph";
my $livegraph = "live.graph";

my $mcRet2 = system("$mc | tee $out");
my $mcRet = $?;
print "Execution returned $mcRet $mcRet2\n";
my $l = `tail -20 $out | grep "ERROR::SIM::"`;
chomp($l);
system(qq/echo "Execution returned $mcRet $mcRet2" >> $out/);

my $lastnail = 0;
my $lastnailStep = 0;
if ($l =~ m|SIM::MAX_STEPS| or $l =~ m|SIM::NO_MORE_STEPS|) {
    print "\nrunning last nail...\n\n";
    system("echo '*** LAST NAIL LOG ***' >> $out");
    system("$mc -LAST_NAIL_REPLAY_FILE error1.path | tee -a $out");
    $l = `tail -10 $out`;
    if ($l =~ m|was random number position (\d+)|s) {
	$lastnail = $1 - 1;

        print "\nfound last nail position $lastnail\n\n";

        if ($lastnail) {

            print "generating live path to $livelog...\n";
            system("$mc -RANDOM_REPLAY_FILE live${lastnail}.path -TRACE_ALL 1 > $livelog");

            print "generating live graph to $livegraph...\n";
            system("$gengraph -o $livegraph $livelog");

            system("echo '*** LAST NAIL STEP ***' >> $out");
            system("$findnail $lastnail $livelog | tee -a $out");

            my $sl = `tail -1 $out`;
            if($sl =~ /step (\d+)/) {
              $lastnailStep = $1;
            }

        } else {
            warn "lastnail position was 0\n";
        }
    }
    else {
	warn "could not find last nail";
    }
    $lastnail = 1;
}

if ($lastnail || ($l =~ m/ERROR::SIM/)) {
    print "\ngenerating error path to $errorlog...\n\n";
    if($lastnail) {
      my $stopstep = $lastnailStep + 200;
      system("$mc -RANDOM_REPLAY_FILE error1.path -max_num_steps $stopstep -TRACE_ALL 1 > $errorlog");
    } else {
      system("$mc -RANDOM_REPLAY_FILE error1.path -TRACE_ALL 1 > $errorlog");
    }

    print "generating error graph to $errorgraph...\n";
    system("$gengraph -o $errorgraph $errorlog");
    exit(1);
}
