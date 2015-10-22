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
                unless (-e "/var/www/mpdmessage/playing.lock")
                {
                        my $mpdStatus = `mpc -h riparateur\@localhost`;
                        my $paused = 0;
                        open my $fh,">/var/www/mpdmessage/playing.lock";
                        #$message = uri_escape($message);
                        #`wget --user-agent="" -O /var/www/mpdmessage/msg.mp3 'https://translate.google.fr/translate_tts?ie=UTF-8&q="$message"&tl=fr'`;

                        if (index($mpdStatus,"playing") != -1)
                        {
                                `mpc -h riparateur\@localhost pause`;
                                $paused=1;
                        }

                        #`mpg123 /var/www/mpdmessage/msg.mp3`;
                        `espeak -vfrench -k5 -s100 "$message"`;

                        if ($paused == 1)
                        {`mpc -h riparateur\@localhost play`;}

                        unlink ("/var/www/mpdmessage/playing.lock");
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
