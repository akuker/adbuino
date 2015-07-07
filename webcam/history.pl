#!/usr/bin/perl

use strict;
use CGI qw(:standard);
use HTML::Scrubber;
use open ':encoding(utf8)';

my $qedit = new CGI;
binmode(STDOUT, ":utf8");

#print header('text/plain');
print header (-charset => 'UTF-8',
		-type => 'text/plain');
my $log = `tail /var/www/camera/logcam.txt`;

my $scrubber = HTML::Scrubber->new;
$scrubber->default(0); ## default to no HTML

my $clean_html = $scrubber->scrub($log);

#chaque ligne : [nom] - [message]

my @lines = split /\n/, $clean_html;
foreach my $line (@lines) {

	my @words = split /-/, $line, 2;
	print qq(<span class="red-text text-darken-2">@words[0] </span>: @words[1]<br/>); 

}

