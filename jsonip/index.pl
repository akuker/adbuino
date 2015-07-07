#!/usr/bin/perl

use strict;
use locale;
use CGI qw(:standard);

my $qjson = new CGI;

my $clientip = $ENV{'REMOTE_ADDR'};
print header(-type=>'text/plain',
		-access_control_allow_origin => '*');
#print 'Access-Control-Allow-Origin: http://cam.e-eisti.science';
#print 'Access-Control-Allow-Origin: http://eeisti.eistiens.net';
print $clientip;
#print "{\"ip\": \"$clientip\", \"address\":\"$clientip\"}";

