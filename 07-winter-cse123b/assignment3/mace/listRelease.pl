#!/usr/bin/perl -w

use strict;
use Cwd;

my $cwd = getcwd();
$cwd =~ m|.*/(.*)|;
my $dir = $1;
processdir("$dir/");

# Remove whitespace from the start and end of the string
sub trimwhitespace
{
  my $string = shift;
  $string =~ s/^\s+//;
  $string =~ s/\s+$//;
  return $string;
}

sub processdir
{
  my $curdir = shift;
  my @files = `svn list`;
  chomp(@files);
  
  foreach (@files) {
    if ( m|/$| ) {
      next if testskip($_);
      if(chdir($_)) {
        processdir("$curdir$_");
        chdir("..") or die("Chdir .. error!");
      }
    }
    else {
      next if testskip($_);
      processfile($_,$curdir);
    }

  }

}

sub processfile
{
  my $filename = shift;
  my $dirname = shift;

  print "$dirname$filename\n";
}

sub testskip 
{
  my $filename = shift;

  my $props = `svn proplist $filename`;
  if($props =~ /(^|\W)norelease($|\W)/) {
    return 1;
  }
  return 0;
}
