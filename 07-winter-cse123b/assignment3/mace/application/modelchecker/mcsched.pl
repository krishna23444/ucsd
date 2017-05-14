#!/usr/bin/perl -w

use strict;

use POSIX qw(setsid);
use IO::Handle;
use Getopt::Long;
use Cwd;

use constant MESSAGE_LOG => "mcsched.log";
use constant ERROR_LOG => "mcsched_error.log";

my $dirpath = ".";
my $foreground = 0;

GetOptions("dirpath=s" => \$dirpath,
	   "foreground" => \$foreground);

unless (-d $dirpath) {
    system("mkdir -p $dirpath") or die "cannot mkdir $dirpath: $!";
}

my $cwd = getcwd();
unless (-e "modelchecker") {
    die "no modelchecker found in $cwd"
}

unless ($foreground) {
    daemonize();
}

my $modelchecker = "$cwd/modelchecker";

my @runs = ();

while (1) {
    opendir(DIR, ".");
    my @t = grep(/\.conf/, readdir(DIR));
    for my $el (@t) {
	if (!grep(/$el/, @runs)) {
	    push(@runs, $el);
	}
    }
    closedir(DIR);

    while (my $f = shift(@runs)) {
	if (not -e $f) {
	    warn "no such file $f\n";
	    next;
	}
	$f =~ m|(.*)\.conf|;
	my $d = "$dirpath/$1";
	mkdir($d) or die "cannot mkdir $d: $!";
	system("mv -v $f $d");
	chdir($d) or die "cannot chdir $d: $!";
	my $pid = fork();
	if ($pid) {
	    waitpid($pid, 0);
	}
	else {
	    print "running $f\n";
	    exec("$modelchecker $f | tee out.log");
	}
	print "completed $f\n";
	chdir($cwd);
    }

    sleep(1);
}

sub daemonize {
#     chdir('/') or die "Can't chdir to /: $!\n";
    open(STDIN, '/dev/null') or die "cannot read /dev/null: $!\n";
    open(STDOUT, '>>/dev/null') or die "cannot write to /dev/null: $!\n";
    open(STDERR, '>>/dev/null') or die "cannot write to /dev/null: $!\n";
    defined(my $pid = fork) or die "cannot fork: $!\n";
    if ($pid) {
        exit(0);
    }
    setsid() or die "Can't start a new session: $!\n";

    open(STDERR, ">@{[ERROR_LOG]}") or die "cannot write to @{[ERROR_LOG]}: $!\n";
    open(STDOUT, ">@{[MESSAGE_LOG]}") or die "cannot write to @{[MESSAGE_LOG]}: $!\n";

#     umask(0);
} # daemonize
