#!/usr/bin/perl

use strict;
use CGI qw(:standard);

my $qedit = new CGI;

print $qedit->header(-type    => 'text/html',
                   -charset => 'utf-8');

print $qedit->start_html
	(
	-title=>'Out of Time, Running In and Uptime',
	-author=>'David Hasselhoff',					
	-head=>[Link({-rel=>'icon',-type=>'image/png',-href=>'favicon.ico'})],		
	-encoding => "utf-8",
	-style=>[{'src'=>'css/80s.css'}],
	);
		

print "
<audio autoplay='autoplay'>
;
<source type='audio/ogg' src='css/hoff.ogg'></source>
your browser is very old
</audio>
<div class='grid-top'></div>
<div class='grid-bottom'></div>

<h1>";

my $uptime = `uptime`;
$uptime =~ s{ (?: , [^,]*){0} \K , }{<br/>}xms;
$uptime =~ s{ (?: , [^,]*){0} \K , }{<br/>}xms;

print "uptime:".$uptime;
print "<br/><br/>";

my $filesystem=qq(df -h | awk 'BEGIN{print("<table>")}{print("<tr><td>",\$1,"</td><td>",\$2,"</td><td>",\$3,"</td><td>",\$4,"</td><td>",\$5,"</td><td>",\$6,\$7,"</td></tr>")}END{print("</table>")}');

print qx($filesystem);

print "<br/>
ping:<br/>";

my $ping = "ping -c 1 ".$ENV{REMOTE_ADDR}." | tail -1| awk '{print $4}' | cut -d '/' -f 5";
print `$ping`."ms";
print "<br/>
</h1>";

print $qedit ->end_html;
