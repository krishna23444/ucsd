#!/usr/bin/perl -w

use strict;
use Carp;
use lib $ENV{HOME} . '/mace/perl5';
use Mace::Util qw(:all);
use Getopt::Long;
use IO::Handle;
use IO::File;
use FileHandle;
use Term::ReadKey;
use Term::ANSIColor;

use constant NODE_COLOR => "bold green";
use constant APP_EVENT_COLOR => "bold blue";
use constant NET_EVENT_COLOR => "bold magenta";
use constant SCHED_EVENT_COLOR => "bold yellow";
use constant TRANSITION_COLOR => "bold cyan";
use constant DROP_COLOR => "bold red";
use constant FAILURE_COLOR => "bold red";
use constant SEQUENCE_COLOR => "bold white";

use constant SINGLE_INDENT => 6;

# my @termSize = GetTerminalSize();
# my $termWidth = shift(@termSize);
# my $termHeight = shift(@termSize);

my $termWidth = 0;

my $showStep = undef;
my $graph = 1;
my $uncolored = 0;
my $ignoreSigInt = 0;
my $outfile = "";
GetOptions("width=i" => \$termWidth,
	   "step=i" => \$showStep,
	   "uncolored" => \$uncolored,
	   "ignoreint" => \$ignoreSigInt,
	   "outfile=s" => \$outfile);
if (defined($showStep)) {
    $graph = 0;
    STDERR->autoflush(1);
}

unless ($termWidth) {
    my @termSize = GetTerminalSize();
    $termWidth = shift(@termSize);
}

# $SIG{__WARN__} = sub { confess $_[0] };

if ($ignoreSigInt) {
    $SIG{INT} = "IGNORE";
}

# print "using $termWidth width\n";

my $propertyCheck = "";

my $startLog = 0;
my $seBegin = 1;

my %nodes = ();
my %rnodes = ();
my @events = ();
my @pathInputs = ();

my $count = 0;
while (my $l = <>) {
    $count++;
    chomp($l);
    if ($l =~ m|\[main\] Starting simulation|) {
	$startLog = 1;
    }
    if ($l =~ m|\[NodeAddress\] Node (\d+) address (\S+)|) {
      $nodes{$1} = $2;
      $rnodes{$2} = $1;
    }
    if ($l =~ m|\[Sim::pathComplete\] .* search sequence \[ (.+) \]|) {
	if (!$graph) {
	    print STDERR ".";
	}
	push(@pathInputs, $1);
    }
    if ($startLog) {
	if ($seBegin) {
	    if ($l =~ m|^(\S+) .*?\[SimulateEventBegin\] (\d+) (\d+) (\w+)|) {
		$seBegin = 0;
		my $id = $1;
		my $step = $2;
		my $node = $3;
		my $type = $4;
#		$nodes{$node} = $id;
#		$rnodes{$id} = $node;
		push(@events, { step => $step,
				id => $id,
				node => $node,
				type => $type,
				} );
	    }
	    elsif ($l =~ m/(Liveness|Safety) property (.*)/) {
		$propertyCheck = "$1 property $2";
	    }
	    elsif ($l =~ m/(Liveness|Safety) Properties: check/) {
		$propertyCheck = "$1 Properties: check";
	    }
	}
	else {
	    if ($l =~ m|^(\S+) .*?\[SimulateEventEnd\] (\d+) (\d+) (\w+) (.*)|) {
		$seBegin = 1;
		$events[-1]->{message} = $5;
	    }
	    elsif ($l =~ m|^(Assert.*failed)|) {
		$events[-1]->{message} = $1;
		$propertyCheck .= " - $1";
	    }
	    else {
		my $r = processLine($l);
		if (ref($r)) {
		    push(@{$events[-1]->{transitions}}, $r);
		}
	    }
	}
    }
}

if (!scalar(@events)) {
    print "no events\n";
    exit(0);
}

if (!defined($events[-1]->{message})) {
    warn "log ends in incomplete event\n";
    $events[-1]->{message} = "???";
}

my $maxStep = scalar(@events);
my $stepWidth = length($maxStep) + 1;
my $numNodes = scalar(keys(%nodes));
my $nodeWidth = int(($termWidth - $stepWidth) / $numNodes);
my $halfNodeWidth = int($nodeWidth / 2);

my $extrapad = 0;
# print "termWidth=$termWidth nodeWidth=$nodeWidth halfNodeWidth=$halfNodeWidth\n";
# print "numNodes=$numNodes stepWidth=$stepWidth\n";
if ($nodeWidth % 2) {
    $extrapad = 1;
}

my %nodeEvents = ();


my %netEdges = ();

my @rows = ();
for my $e (@events) {
    my $s = "";
    my $step = $e->{step};
    if (defined($showStep)) {
	if ($step != $showStep) {
	    next;
	}
    }
    my $stepStr =  ' ' x (length($maxStep) - length($step)) . $step . ' ';
    if ($graph) {
	$s .= ccolor($stepStr, TRANSITION_COLOR);
    }
    my $type = substr($e->{type}, 0, 3);
    my $emsg = $e->{message};
    my $id = $e->{id};
    my $node = $e->{node};

    my $dotSpace = "";
    for my $i (1..$node * $nodeWidth) {
	if ($i % 2 == 1) {
	    $dotSpace .= '.';
	}
	else {
	    $dotSpace .= ' ';
	}
    }
    if (substr($dotSpace, -1) eq ".") {
	$dotSpace = substr($dotSpace, 0, -1) . " ";
    }
    
    my $space = ' ' x ($node * $nodeWidth);
    if ($graph) {
	$s .= ccolor($dotSpace, TRANSITION_COLOR);
    }

    my $netId = -1;
    my $color = "";
    if ($type eq "NET") {
	if ($emsg =~ m|^(.* from \S+)|) {
	    $emsg = $1;
	}
	if ($emsg =~ m|id (\d+) dest_not_ready|) {
	    my $dropid = $1;
	    for my $r (@rows) {
		if ($r =~ m|route pkt\(id=$dropid|) {
		    $r =~ s/(<|>)/x/;
		}
	    }
	    $color = DROP_COLOR;
	}
	else {
	    if ($emsg =~ m|id (\d+)|) {
		$netId = $1;
	    }
# 	    else {
# 		die "could not find net id in $emsg\n";
# 	    }
	    $color = NET_EVENT_COLOR;	
	}
    }
    elsif ($type eq "SCH") {
        if ($emsg !~ m/Assert/) {
	    if ($emsg =~ m|(.*)Service::(.*?)\(\d+\)|) {
		my $service = $1;
		my $timer = $2;
		$service =~ tr/a-z_//d;
		$emsg = "$service $timer";
	    }
	    else {
		warn "could not parse service from $emsg in step $step\n";
	    }
        }
	$color = SCHED_EVENT_COLOR;
    }
    else {
	if ($type ne "APP") {
	    die "unknown type: $type\n";
	}
	$color = APP_EVENT_COLOR;
	if ($emsg =~ m|FAILURE|) {
	    $type = "APP FAILURE";
	    $emsg = "";
	    $color = FAILURE_COLOR;
	}
    }

    while (my ($k, $v) = each(%nodes)) {
	$emsg =~ s|$v|$k|g;
    }

    if ($graph) {
	$s .= ccolor("$type", $color, "/$node ", NODE_COLOR,
		     $emsg, TRANSITION_COLOR, $nodeWidth - 1);
    }
    else {
	$s .= ccolor("$type", $color, "/$node ", NODE_COLOR,
		     trim($emsg), TRANSITION_COLOR, " [node $node]", NODE_COLOR);

    }
    push(@rows, $s);
    $s = "";

    if ($emsg =~ m|Assert\((.*)\) failed|) {
	$emsg = "\nassertion failed\n$1";
    }
    my $nodeLabel = "$step: $emsg";

    my $indent = 2;
    my $dwidth = $nodeWidth - ($indent + 1);
    if (defined($e->{transitions})) {
	for my $t (@{$e->{transitions}}) {
	    $stepStr =~ s/\d/ /g;
	    my $pre = $graph ? $stepStr . $space : "";
	    $pre .= ' ' x $indent;
	    my $src = $t->{src};
	    my $dest = $t->{dest};
	    my $service = $t->{service};
	    my $message = $t->{message};
	    my $messageFields = $t->{messageFields};
	    my $method = $t->{method};
	    my $detail = $t->{detail};
	    my $direction = $t->{direction};
	    my $sim = $t->{sim};
	    my $drop = $t->{drop};
	    
	    if ($method =~ m/^(route|routeRTS|send|deliver)$/) {
		if (!defined($rnodes{$src})) {
		    die "unknown node '$src' $service $method $direction\n";
		}
		if (!defined($rnodes{$dest})) {
		    die "unknown node '$dest'\n";
		}

		$src = $rnodes{$src};
		$dest = $rnodes{$dest};
	    }

	    while (my ($k, $v) = each(%nodes)) {
		$messageFields =~ s|$v|$k|g;
		$detail =~ s|$v|$k|g;
	    }

	    my $str = "";
	    if ($direction eq "upcall") {
		$str .= "u ";
	    }
	    elsif ($direction eq "scheduler") {
		$str .= "s ";
	    }
	    elsif ($direction eq "error") {
		$str .= "! ";
	    }
	    else {
		$direction eq "downcall" or die "bad direction: $direction\n";
		$str .= "d ";
	    }
	    my $abbrvService = $service;
	    $abbrvService =~ tr/a-z_//d;
	    $str .= "$abbrvService ";
	    if ($method =~ /^(multicast|route|routeRTS|send)$/) {
		$str .= "$method $message";

		if ($message eq "pkt" && $messageFields =~ m|\(id=(\d+)\)|) {
		    $netId = $1;
#		    $netEdges{$netId}->{src} = $gn;
		}

		my $fields = "";
		if ($graph && length($str . $messageFields) > $dwidth) {
		    $fields = $pre . substr($messageFields, 0, $dwidth);
		}
		else {
		    $str .= $messageFields;
		}
		if ($graph) {
		    $str = substr($str, 0, $dwidth);
                    if ($method ne "multicast") {
                        if ($dest < $node) {
                            my $arrowch = $sim ? "--" : " -";
                            my $nodediff = $node - $dest - 1;
                            $pre = $stepStr . (' ' x ($halfNodeWidth + ($dest * $nodeWidth)));
                            my $n = $halfNodeWidth + $extrapad + ($nodediff * $nodeWidth);
                            my $pad = "";
                            if ($n % 2) {
                                $pad = "-";
                            }
                            $n = int($n / 2);
                            my $head = $drop ? "X" : "<";
                            $pre .= $head . $pad . ($arrowch x $n) . " ";
                        }
                        elsif ($dest > $node) {
                            my $arrowch = $sim ? "--" : "- ";
                            my $n = $nodeWidth - length($str) + $halfNodeWidth - 5;
                            $n += (($dest - $node - 1) * $nodeWidth);
                            my $pad = "";
                            if ($n % 2) {
                                $pad = "-";
                            }
                            $n = int($n / 2);
                            my $head = $drop ? "X" : ">";
                            $str .= " " . ($arrowch x $n) . $pad . $head;
                        }
                        else {
                            $str .= " to self";
                        }
                    }
		}
		else {
		    if ($dest != $node) {
			$str .= " -> $dest";
		    }
		    else {
			$str .= "to self";
		    }
		}
		$s .= $pre;
		$s .= $str;
		push(@rows, substr($s, 0, $termWidth - SINGLE_INDENT));
		$s = "";
		if ($fields) {
		    push(@rows, $fields);
		}		
		next;
	    }
	    elsif ($method =~ /^deliver$/) {
		$str .= "$method $src -> $message";
		if ($graph && length($str . $messageFields) > $dwidth) {
		    push(@rows, $pre . substr($str, 0, $dwidth));
		    $str = $messageFields;
		}
		else {
		    $str .= $messageFields;
		}
	    }
	    elsif ($method) {
		$str .= $method;
		if ($graph && length($str . $detail) > $dwidth) {
		    push(@rows, $pre . substr($str, 0, $dwidth));
		    $str = $detail;
		}
		else {
		    $str .= $detail;
		}
	    }

	    if ($graph) {
		$str = substr($str, 0, $dwidth);
	    }
	    else {
		$str = substr($str, 0, $termWidth - SINGLE_INDENT);
	    }

	    push(@rows, $pre . $str);
	}
    }
    if (!$graph) {
	push(@rows, "----PATH----");
	my $i = shift(@pathInputs);
        unless (defined $i) {$i = "?";}
	push(@rows, ccolor($i, SEQUENCE_COLOR, $termWidth - 10));
    }
}

if (!$graph) {
    print join("\n", @rows) . "\n";
    exit(0);
}

my $fh = new IO::File();
if ($outfile) {
    $fh->open(">$outfile") or confess "cannot write to $outfile: $! $@\n";
}
else {
    $fh->fdopen(fileno(STDOUT), "w");
}

if ($propertyCheck) {
    print $fh "$propertyCheck\n";
}

# print ' ' x $stepWidth;
# for my $i (sort(keys(%nodes))) {
#     print ' ' x ($halfNodeWidth - length($i)), $i, ' ' x ($nodeWidth - $halfNodeWidth);
# }
# print "\n";

print $fh join("\n", @rows) . "\n";

#while (my ($k, $el) = each(%netEdges)) {
#    if (scalar(keys(%$el)) == 2) {
#	$g->add_edge($el->{src} => $el->{dest});
#    }
#}

# $g->as_canon("graph.dot");
#$g->as_ps("graph.ps");

# printCols();

# for my $event (@events) {
#     print 
# }

# for my $el (@events) {
#     print($el->{step} . " " . $el->{id} . " " . $el->{node} . " " . $el->{type} . " " .
# 	  $el->{message} . "\n");
#     if (defined($el->{transitions})) {
# 	my $ref = $el->{transitions};
# 	for my $t (@$ref) {
# 	    print("\t" . $t->{src} . " -> " . $t->{dest} . " " . $t->{method} . " " .
# 		  $t->{message} . "\n");
# 	}
#     }
# }

sub processLine {
    my $l = shift;
    my $service = "";
    my $method = "";
    my $message = "";
    my $messageFields = "";
    my $detail = "";
    my $direction = "";
    my $src = "";
    my $dest = "";
    my $sim = 0;
    my $drop = 0;
    
    if ($l =~ m/^(\S+) .*?\[(COMPILER|SIMULATOR)::(\w+)::(multicast|route|routeRTS|send)::(\w+)::\(downcall\)\] \4(.*)/) {
	$src = $1;
	my $which = $2;
	$service = $3;
	$method = $4;
	$message = $5;
	$detail = $6;

	$detail =~ m/\[\w+=(.*?)\]\[s_deserialized=$message(.*?)\]/;
	$dest = $1;
	$messageFields = $2;
	$direction = "downcall";
	if ($which eq "SIMULATOR") {
	    $sim = 1;
	    if ($messageFields =~ m|SIM_DROP=1|) {
		$drop = 1;
	    }
	}

    }
    elsif ($l =~ m/\[COMPILER::(\w+)::(deliver)::(\w+)::\(demux\)\] deliver(.*)/) {
	$service = $1;
	$method = $2;
	$message = $3;
	$detail = $4;

	$detail =~ m/\[\w+=(.*?)\]\[\w+=(.*?)\]\[s_deserialized=$message(.*?)\]/;
	$src = $1;
	$dest = $2;
	$messageFields = $3;
	$direction = "upcall";
    }
    elsif ($l =~ m/\[COMPILER::(\w+)::expire_(\w+)::\(demux\)\] expire_\2(.*)/) {
	$service = $1;
	$method = $2;
	$detail = $3;
	$direction = "scheduler";
    }
    elsif ($l =~ m/\[COMPILER::(\w+)::(\w+)::\(upcall\)\] \2(.*)/) {
	$service = $1;
	$method = $2;
	$detail = $3;
	$direction = "upcall";
    }
    elsif ($l =~ m/\[COMPILER::(\w+)::(\w+)::\(downcall\)\] \2(.*)/) {
	$service = $1;
	$method = $2;
	$detail = $3;
	$direction = "downcall";
    }
    elsif ($l =~ m/\[(ERROR|WARNING)::(\w+::\w+)::.*\] (.*)/) {
	$service = $1;
	$method = $2;
	$detail = " $3";
	$direction = "error";
    }
    elsif ($l =~ m/\[(ERROR|WARNING)\] (.*)/) {
	$service = $1;
	$detail = $2;
	$direction = "error";
    }

    my @skip = qw(forward getLocalAddress);
    if (grep(/$method/, @skip)) {
	return 0;
    }

    if ($method eq "deliver" && !$src) {
	return 0;
    }

    if ($service) {
# 	print("returing src=$src dest=$dest service=$service method=$method " .
# 	      "message=$message messageFields=$messageFields detail=$detail " .
# 	      "direction=$direction sim=$sim drop=$drop\n");
# 	print "$l\n";

	
	my $r = {
	    src => $src,
	    dest => $dest,
	    service => $service,
	    method => $method,
 	    message => $message,
	    messageFields => $messageFields,
	    detail => $detail,
	    direction => $direction,
	    sim => $sim,
	    drop => $drop,
	};
	return $r;
    }
    else {
	return 0;
    }
} # processLine

sub printCols {
    my $cols = 176;
    for my $i (1..int($cols / 100)) {
	print ' ' x 99, $i % 10;
    }
    print "\n";
    for my $i (1..int($cols / 10)) {
	print ' ' x 9, $i % 10;
    }
    print "\n";
    for my $i (1..$cols) {
	print $i % 10;
    }
    print "\n";
}

sub ccolor {
    my $r = "";
    my $len = 0;
    my $sslen = 0;
    if (scalar(@_) % 2) {
	$sslen = pop(@_);
    }
    while (defined(my $t = shift(@_))) {
	my $c = shift(@_);
	if ($sslen && (scalar(@_) == 0)) {
	    if ($len >= $sslen) {
		$t = "";
	    }
	    else {
		$t = substr($t, 0, $sslen - $len);
	    }
	}
	if ($uncolored) {
	    $r .= $t;
	}
	else {
	    $r .= colored($t, $c);
	}
    }
    return $r;
}
