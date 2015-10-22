#!/usr/bin/perl

use strict;
use CGI qw(:standard);
use URI::Escape;
use LWP::Simple;
use Net::LDAP;

my $qedit = new CGI;
#my $tmpdir = "/var/www/camera/picturening/";
my $result = "No parameter specified.";

my @banlist = ["banned", "LDAP IDs", "go here"];

if ($qedit->param('login') && $qedit->param('passw') && $qedit->param('message'))
{
        my $login = $qedit->param('login');
        my $message = $qedit->param('message');
        #open(my $logfile, '>>', 'logcam.txt');
        #say $logfile ($qedit->param('login'))." - ".$message;
        #$message = uri_escape($message);

        if (loginLDAP($login,$qedit->param('passw'))== 1)
        {
        	if( lc($login) ~~ @banlist )
        		{
        		 $result = "T bani pd";  
        		}
        	else
        		{
			open(my $logfile, '>>', 'logcam.txt');
		        say $logfile ($qedit->param('login'))." - ".$message;
        		$message = uri_escape($message);
        		$result = get qq(/msg_on_speaker_server.pl?ip=$login&message=$message);
        		}
        }
        else
        {
        	$result = "Login error.";
        }
}

print header('text/plain');
print $result;

sub loginLDAP
	{
	my $login = $_[0];
	my $pass = $_[1];
	
	my $ldap = Net::LDAP->new( 'ldap.eisti.fr' );
	$ldap->start_tls();

	# bind to a directory with dn and password
	my $mesg = $ldap->bind( 'cn=webuser, dc=eisti,dc=fr',
	                     password => 'nn' #change LDAP server adress and password here
	                   );

	$mesg = $ldap->search(
	                        base   => 'ou=people,dc=eisti,dc=fr',
	                        scope  => 'subtree',
	                        filter => '(uid='.$login.')',
	                        attrs  => ['dn']
	                      );

	# Get the user's dn and try to bind:
	my $user_dn = $mesg->entry->dn;
	#print $user_dn;
	
	my $login = $ldap->bind( $user_dn, password => $pass );
	
	if($login->code == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}

	$ldap->unbind;
	}

