#!/usr/bin/perl

use strict;
use CGI qw(:standard);
use LWP::Simple qw/get/;
use LWP::Protocol::https; 
use JSON::Parse 'parse_json';
use Switch;
use URL::Encode qw(:all);
use Encode qw(decode encode);
use Log::Log4perl;
Log::Log4perl->init("log.conf");

my $qrequest = new CGI;
my $accesstoken="nope";

my @smileys = ("-_-'", "XD", ":p", ":)", ";p", "--'", "-__-", ":O", ":D", ":P");
my @ponctuation = ("", "!", "!!", "..", "...", "!!!", ".....", "!", "!", "!");
my @photos = ("AgADAgAD8bExG7ou5QLAQhvv4znGf-juWSoABGLfsfvbyriWE_sAAgI","AgADAgAD8LExG7ou5QJq4o60ytjlZl3pWSoABImZ0c0pmjoNyvYAAgI","AgADAgAD77ExG7ou5QL1Yrcc7XXrNqnFWSoABE__2CspbWKm5AEBAAEC");
my @stickers = ("BQADBAADEgAD2zPJBHZZS1PvFGP0Ag","BQADBAADDwAD2zPJBFLXbidWFvnhAg");

my $logger = Log::Log4perl->get_logger('wiart.bot');

$logger->info('Hello !');

#If we got sent a message and RNG hits the 5%
if ($qrequest->param() && int(rand(100))<5) 
{

	$logger->info('Request obtained!');

    # Get the message object in result
	my $jsonmessage = $qrequest->param('POSTDATA');

	$logger->info('Request looks like: '.$jsonmessage);

	my $hash = parse_json($jsonmessage);

	#structure of data: {"update_id":603591173,
				#"message":{"message_id":755,"from":{"id":64961170,"first_name":"Amaury","last_name":"B\u00e9chu","username":"MSFag"},
				#			"chat":{"id":-28889987,"title":"J -23"},"date":1438889893,"text":"mais genre ils ont meme pas rajout\u00e9 Farahlon du coup"
				#			}
				#}

	#recup du message
	my $data = $hash->{"message"};

	#datas essentielles:

	my $chatid = $data->{"chat"}->{"id"};
	my $postername = $data->{"from"}->{"first_name"};
	my $posterlastname = $data->{"from"}->{"last_name"};

	$logger->info('Message posted by '.$postername." ".$posterlastname." in ".$chatid);

	#random keywords au cas ou aucun n'est spécifié ? 
	my $keyword = "debian";
	my $sendimage = 0;
	my $sendsticker = 0;

	#le fun commence
	if (exists $data->{"text"})
		{
			$keyword = &getLongestWord($data->{"text"});
		}

	#on envoie une réponse
	my $response;

		switch(int(rand(18))){
							   case 0	{ $response = qq($postername le $keyword cuck ) }
							   case 1	{ $response = int(rand(1337))."h pour réparer mon script " }
							   case 2	{ $response = "et j'ai encore ".int(rand(1337))." warnings de variable non init "}
							   case 3	{ $response = qq($postername il manque de respect ) }
							   case 4	{ $response = qq(il est con ce denis ) }
							   case 5	{ $response = qq(il est con ce $postername ) }
							   case 6	{ $response = qq(qu'est ce que t'as foutu avec $keyword $postername ) }
							   case 7	{ $response = qq($postername cucké par $keyword ) }
							   case 8	{ $response = qq(mon script marche toujours pas ) }
							   case 9	{ $response = qq($postername le cuck ) }
							   case 10  { $response = qq(hurr durr $postername $keyword ) }
							   case 11  { $response = qq($postername $keyword $posterlastname ) }
							   case 12  { $response = qq($postername cuck $posterlastname ) }
							   case 13  { $response = qq(rends les clés denis palhais ) }
							   case 14  { $response = qq(https://www.youtube.com/watch?v=3FY4MRdQOdE Daft punk ! :D ils sont quand meme au dessus ) }
							   case 15  { $response = qq($postername ... no comment ) }
							   case 16  { $sendimage = 1 }
							   case 17	{ $sendsticker = 1}
							   else {} 
							}

	#On ajoute ponctuation et smiley
	$response .= @ponctuation[int(rand(10))]." ";
	$response .= @smileys[int(rand(10))];

	$logger->info('Final message: '.$response);

	my $uri = "";

	#On envoie la réponse.
	if ($sendimage == 1)
	{
		$uri = 'https://api.telegram.org/bot'.$accesstoken.'/sendPhoto?chat_id='.$chatid."&photo=".@photos[int(rand(3))]."&caption=Ca c'est de la voiture ! :p";
	}
	elsif ($sendsticker == 1)
	{

		$uri = 'https://api.telegram.org/bot'.$accesstoken.'/sendSticker?chat_id='.$chatid."&sticker=".@stickers[int(rand(2))];
	}
	else
	{
		$uri = 'https://api.telegram.org/bot'.$accesstoken.'/sendMessage?chat_id='.$chatid."&text=".url_encode_utf8($response);
	}

	$logger->info('Final request URL:'.$uri);
	my $req = HTTP::Request->new( 'GET', $uri );

	#Execution de la requête construite avec LWP
	my $ua = LWP::UserAgent->new; 
	my $res = $ua->request($req);

	$logger->info('Request sent to Telegram! Status:'.$res->content());
}

#on print 200 dans tous les cas en imprimant le header, pour indiquer à telegram qu'on a bien reçu la requête.
print $qrequest->header;

sub getLongestWord{

	my $logger = Log::Log4perl->get_logger('wiart.bot');
	$logger->info('Parsing words in '.$_[0]);

	my @values = split(' ', $_[0]);
	my $longestword = "";

	foreach (@values)
		{
		      if (length($_) > length($longestword))
		      	{
		      		$longestword = $_;
		      	}
		}

	return $longestword;
}