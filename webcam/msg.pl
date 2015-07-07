#!/usr/bin/perl

use strict;
use CGI qw(:standard);
use URI::Escape;

my $qedit = new CGI;
my $tmpdir = "/var/www/camera/picturening/";
my $result = "No parameter specified.";

if ($qedit->param('ip') && $qedit->param('message')) 
{
	my $message = $qedit->param('message');
	open(my $logfile, '>>', 'logcam.txt');
	say $logfile ($qedit->param('ip'))." - ".$message;	

	#vérif IP client-> plus tard 

	#vérif si y'a pas déjà un msg
	if ($message ne "")
	{
		unless (-e "/var/www/camera/picturening/playing.lock")
		{
			my $mpdStatus = `mpc`;
			my $paused = 0;
			open my $fh,">/var/www/camera/picturening/playing.lock";
			$message = uri_escape($message);
			`wget --user-agent="" -O /var/www/camera/picturening/msg.mp3 'https://translate.google.fr/translate_tts?ie=UTF-8&q="$message"&tl=fr'`;
			
			if (index($mpdStatus,"playing") != -1)
			{
				`mpc -h epicjames1337\@localhost pause`;
				$paused=1;
			}
			
			`mpg123 /var/www/camera/picturening/msg.mp3`;
			
			if ($paused == 1)
			{`mpc -h epicjames1337\@localhost play`;}
			
			unlink ("/var/www/camera/picturening/playing.lock");
			$result = "Message sent!";
		}
		else
		{
			$result = "Audio message already playing."
		}
	}
	else
	{
	$result = "Don't send empty messages faggot";
	}
}

print header('text/plain');
print $result;





