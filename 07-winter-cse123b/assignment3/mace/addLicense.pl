#!/usr/bin/perl -w

use strict;

# Declare the subroutine
sub trimwhitespace($);
sub processcontributors();
sub getlicense(@);
sub getlicensetmpl();
sub processdir(@);
sub processfile(@);
sub testskip($);

my %contributors;
my $licensetmpl; #The license template

processcontributors();
$licensetmpl = getlicensetmpl();

#1 - Get the Base License
my $license = getlicense($licensetmpl,".");

#print "License:\n";
#print $license;

#2 - Print the LICENSE file
open(LICENSE, ">LICENSE");
print LICENSE $license;
close(LICENSE);

#3 - Process other files
processdir($licensetmpl, $license);

# Remove whitespace from the start and end of the string
sub trimwhitespace($)
{
  my $string = shift;
  $string =~ s/^\s+//;
  $string =~ s/\s+$//;
  return $string;
}

sub getlicensetmpl()
{
  my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime time;
  $year+=1900;
  #print "Year: $year\n";

  open(LICENSEPROP, "-|", "svn propget copyright .") or die("Cannot get license propery");
  my @licenselines = <LICENSEPROP>;
  close(LICENSEPROP);
  my $license = join("", @licenselines);
  $license =~ s/\[YEAR\]/$year/g;
  return $license;
}

sub getlicense(@)
{
  my $license = shift;
  my $file = shift;
  my $authorsin = `svn propget authors $file`;
  return 0 unless $authorsin;
  my @authorsarr = split( /\W+/, $authorsin);
  my @authorsnew;
  foreach my $auth (@authorsarr) {
    push(@authorsnew, $contributors{$auth});
  }
  my $authors = join(", ",@authorsnew);
  #print "Authors: $authors\n";

  $license =~ s/\[AUTHORS\]/$authors/g;
  return $license;
}

sub processcontributors() 
{
  open(CONTRIBUTORS, "<CONTRIBUTORS") or die("Cannot open contributors file");

  #print "Contributors:\n";
  while(<CONTRIBUTORS>) {

    if ( $_ =~ /([^:]+):([^:]+):([^:]+):([^:]+):([^:]+)/ ) {
      my $init = trimwhitespace($1);
      my $name = trimwhitespace($2);
      $contributors{$init} = $name;
#      print "$init = $name\n";
    }

  }
  close(CONTRIBUTORS);
}

sub processdir(@) 
{
  my $licensetmpl = shift;
  my $uplicense = shift;
  my $license = getlicense($licensetmpl,".");
  if(!$license) { $license = $uplicense; }
#  print $licensetmpl."\n";
  my @files = `svn list`;
  chomp(@files);
  
  foreach (@files) {
    if ( m|/$| ) {
      print "Found directory $_\n";
      next if testskip($_);
      if(chdir($_)) {
        processdir($licensetmpl,$license);
        chdir("..") or die("Chdir .. error!");
      }
    }
    elsif( /\.(cc|h|mh|c|m|mac|mace|y|l|pl|pm|cpp|tex)$/ || /^mace(pp)?$/ || /^makenull$/ || /^macemh$/) {
      print "Found source file $_\n";
      next if testskip($_);
      processfile($_,$licensetmpl,$license);
    }
    else {
      print "Ignoring unrecognized file $_\n";
    }

  }

}

sub processfile(@) 
{
  my $filename = shift;
  my $licensetmpl = shift;
  my $uplicense = shift;

  my $header;
  my $footer;
  my $lineprefix;
  my $memo = "$filename : part of the Mace toolkit for building distributed systems";

  my $license = getlicense($licensetmpl,$filename);
  if(!$license) { $license = $uplicense; }

  my @licenselines = split(/\n/, $license);
  
  if($filename =~ /\.(cc|h|mh|c|m|mac|mace|y|l|cpp)$/) {
    $header = "/* \n * $memo\n * \n";
    $lineprefix = " * ";
    $footer = " * \n * ----END-OF-LEGAL-STUFF---- */\n";
  } 
  elsif($filename =~ /\.(pm|pl)$/ || $filename =~ /^mace(pp)?$/ || $filename =~ /^makenull$/ || $filename =~ /^macemh$/) {
    $header = "# \n# $memo\n# \n";
    $lineprefix = "# ";
    $footer = "# \n# ----END-OF-LEGAL-STUFF----\n";
  }
  elsif($filename =~ /\.tex$/) {
    $header = "% \n% $memo\n% \n";
    $lineprefix = "% ";
    $footer = "% \n% ----END-OF-LEGAL-STUFF----\n";
  }
  
  open(FILE, "<$filename") or die("Couldn't open source file!");
  
  my @filein;
  my $first = 1;
  my $filehead = "";
  while(<FILE>) {
    if($first && /^#!/) { 
      $filehead = $_; 
    }
    else {
      push @filein, $_;
    }
    if( /----END-OF-LEGAL-STUFF----/ ) {
      @filein = < >;
    }
    $first = 0;
  }
  my @fileout;
  push @fileout, $filehead, $header;
  foreach (@licenselines) {
    push @fileout, $lineprefix, $_, "\n";
  }
  push @fileout, $footer;
  open(OUTFILE, ">.license.$filename") or die("Couldn't open out file!");
  print OUTFILE @fileout, @filein;
  close(OUTFILE);

  system("diff -b --brief $filename .license.$filename");
  my $retcode = $? >> 8;
  if($retcode == 0) {
    unlink(".license.$filename");
  } elsif($retcode == 1) {
    print "License Modified on $filename\n";
    rename(".license.$filename", $filename);
  } else {
    die("Painful death, checking updates to $filename (return code $retcode)\n");
  }
}

sub testskip($) 
{
  my $filename = shift;

  my $props = `svn proplist $filename`;
  if($props =~ /(^|\W)norelease($|\W)/) {
    print "Skipping $filename because 'norelease' property is set\n";
    return 1;
  }
  elsif($props =~ /(^|\W)noautolicense($|\W)/) {
    print "Skipping $filename because 'noautolicense' property is set\n";
    return 1;
  }
  return 0;
}
