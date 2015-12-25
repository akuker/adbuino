#!/usr/bin/perl

use strict;
use CGI qw(:standard);
use LWP::Simple qw/get/;
use LWP::Protocol::https; 
use JSON::Parse 'parse_json';
use URL::Encode qw(:all);
use Encode;
use Log::Log4perl;
use Acme::Zalgo;
use Text::UpsideDown;
use Redis;
use Switch;
use utf8;
use IPC::Filter qw(filter);
use open ':std', ':encoding(UTF-8)';

Log::Log4perl->init("log.conf");
binmode STDOUT, ':encoding(UTF-8)';


#Initialization variables
my $qrequest = new CGI;
my $accesstoken="non";
my $redisaddress = "localhost:6379";

my @smileys = ("-_-'", "XD", ":p", ":)", ";p", "--'", "-__-", "(o.o)~", ":O", ":D", "^^", ":^)","Ψ(｀∇´)Ψ","(´・ｪ・｀)","ヽ(;´Д｀)ﾉ","(´・ω・｀)","ヽ(´―｀)ノ","(＃ﾟДﾟ)","Σ(゜д゜;)","щ(ﾟДﾟщ)","(ﾟ∀ﾟ)","（･∀･)つ⑩","♪┏(・o･)┛♪┗ ( ･o･) ┓♪┏ ( ) ┛♪┗ (･o･ ) ┓♪┏(･o･)┛♪","(◑_◑)","┻━┻ ︵ヽ(`▭´)ﾉ︵﻿ ┻━┻","（‐＾▽＾‐）","(☞ﾟヮﾟ)☞ ☜(ﾟヮﾟ☜)","(>‿◠)✌");
my @ponctuation = ("", "!", "!!", "..", "...", "!!!", ".....", "!", "!", "!","ｷﾀ━━━━━━(ﾟ∀ﾟ)━━━━━━!!!!!");
my @stickers = ("BQADBAADEgAD2zPJBHZZS1PvFGP0Ag","BQADBAADDwAD2zPJBFLXbidWFvnhAg","BQADAgADaAADkjrfA8n7SFh4h4GeAg","BQADAgADVQADL1GyBWan0Lfa7fjmAg","BQADAgADTwADkjrfA4a3XwyXRbekAg","BQADAgADXwADkjrfA4OOCuDz__KmAg","BQADAQADpwQAAiBWmAJ8Va4tRld5nwI","BQADAgADcQADAqHaBWGmCCRjI3B2Ag","BQADAgADiQADAqHaBafsuyj2YD9uAg","BQADAgADlwADAqHaBR7ddBmJUxUfAg","BQADAgADrQADAqHaBTxjs2wKySKeAg","BQADAgADrwADAqHaBfeNSV_mSm0wAg","BQADAgADsQADAqHaBYkuGpLymUtbAg","BQADBAADXAADXSupAaa2MS1LMn26Ag","BQADBAADZgADXSupATsC2Pm0xdADAg","BQADBAADagADXSupAcBZICwpXF7JAg","BQADBQADpgEAAprnOwMzs6BU5mJvjgI","BQADAgADDAEAArou5QIaKkhJBeilzAI","BQADAgADGgEAArou5QIU-lmAYLxG5gI","BQADAgADKgEAArou5QJCHPl7rsjMnAI","BQADAgADBAEAArou5QKs0i4SszwmpAI","BQADBAAD-wEAAjqZlwG8FkVYTPAOngI","BQADBAAD3gEAAjqZlwHna8_WKMZ_xAI","BQADBAADAwIAAjqZlwFcuqAHOlAxIQI","BQADBAADFgAD6ZtlAAKpVijjJ0tPAg","BQADBAADJAAD6ZtlAAEQGjncHDFxpwI");

my $logger = Log::Log4perl->get_logger('wiart.bot');

#Default redis server location is localhost:6379. 
#Auto-reconnect on, one attempt every 100ms up to 2 seconds. Die after that.
my $redis = Redis->new(server => $redisaddress, 
						reconnect => 100,
						every     => 3000);

$logger->info('Hello !');

#If we got sent a message and RNG hits the designed value
if ($qrequest->param()) #int(rand(100))<$RNG) 
{
	$logger->info('Request obtained!');

	my $RNG = &getRNGValue();

    # Get the message object in result
	my $jsonmessage = $qrequest->param('POSTDATA');

	$logger->info('Request looks like: '.$jsonmessage);

	my $hash = parse_json($jsonmessage);

	#recup du message
	my $data = $hash->{"message"};

	#datas essentielles:
	my $chatid = $data->{"chat"}->{"id"};
	my $postername = $data->{"from"}->{"first_name"};
	my $posterlastname = $data->{"from"}->{"last_name"};
	my $posterid = $data->{"from"}->{"id"};
	my $text = $data->{"text"};

	#$text = decode_utf8($text);

	my $command;
	my $commandargument;
	($command, $commandargument) = split(/ /, $text, 2);

	$logger->info('command is '.$command.' and argument is '.$commandargument );

	#RNG command ? Checks for specific user ID and text message.
	if ($posterid eq "48574138" && exists $data->{"text"})
	{
		
		$logger->info('Potential RNG changing command sent by master: '.$text);

		if ($command eq "/rng")
			{
				#fucking confirmed
				$RNG = $commandargument;
				#quick number check
				$RNG += 0;

				&setRNGValue($RNG);
				&sendMessage("Posting occurence has been modified to ".$RNG."%. Have a nice day.",$chatid);
				$logger->info('Occurence changed to '.$RNG);
			}

	}

	#User added?
	if (exists $data->{"new_chat_participant"})
	{
		&sendMessage("Welcome, to my DEATH MACHINE, ".$data->{"new_chat_participant"}->{"first_name"}."!",$chatid);
	}

	#If adding quote
	if ($command eq "/add" || $command eq "/add\@WiartBot")
		{
			my $newquote = $commandargument;
			$logger->info('New quote is '.$newquote);

			unless($newquote eq "1")
			{
			if (length($newquote)<100 && ($newquote=~ m/\[NAME\]/ || $newquote=~ m/\[KEYWORD\]/ || $newquote=~ m/\[INT\]/ ))
				{
					&addToRedis($newquote,"wiart");
					&sendMessage("Quote added in common pool.",$chatid);
				}
			elsif (length($newquote)<250)
				{
					&addToRedis($newquote,"wiart_rare");
					&sendMessage("Quote added in rare pool.",$chatid);
				}
			else
				{
					&addToRedis($newquote,"wiart_ultra");
					&sendMessage("Quote added in ULTRA rare pool !",$chatid);
				}	
			}
			else
			{
				&sendMessage("(◑_◑)",$chatid);
			}
		}
	
	#Regular use case, RNG verification
	if (int(rand(100))<$RNG)
	{
		$logger->info('Message posted by '.$postername." ".$posterlastname." in ".$chatid);

		#random keyword au cas ou aucun n'est spécifié
		my $keyword = "tuning";

		#Si c'est un message texte, on peut récupérer un keyword pour l'utiliser dans la réponse finale.
		if (exists $data->{"text"})
			{
				my $textout;
				#Utilisation de treetagger
				#my $textout = `echo "\$text" | tree-tagger-french | grep NOM`;
				#run3("tree-tagger-french",$text,$textout);

				$textout = filter($text, "tree-tagger-french");

				#$logger->info('Treetagger return :'.$textout);

				my @noms;
				my @output = split /\n/, $textout;

				foreach my $nom (@output) {
					$logger->info('From treetagger: '.$nom);
					if (index($nom,"NOM") !=-1)
						{
							push (@noms, (split ' ',$nom)[0]);
						}
				}

				unless (@noms == 0)
				{ 
					$keyword = $noms[rand @noms];
				}
				else
				{
					$keyword = (split ' ',@output[0])[0];
				}

				#$keyword = &getLongestWord($data->{"text"});
			}

		#on construit une réponse.
		my $response;
		#On a une chance de piocher dans 3 pools: Commun, Rare(10%), et UltraRare(2%).
		switch (int(rand(100))) {
			case [1..10] { $response = &getFromRedis("wiart_rare")}
			case [11..12] { $response = &getFromRedis("wiart_ultra")}
			else { $response = &getFromRedis("wiart")}
			}

		#On ajoute ponctuation et smiley
		$response .= @ponctuation[int(rand(scalar @ponctuation))]." ";
		$response .= @smileys[int(rand(scalar @smileys))];

		#On remplace les variables par les valeurs qu'on possède
		my $int = int(rand(1337));
		my $namecaps = uc($postername);
		my $kwcaps = uc($keyword);
		$response =~ s/\[NAME\]/$postername/g;
		$response =~ s/\[INT\]/$int/g;
		$response =~ s/\[LASTNAME\]/$posterlastname/g;
		$response =~ s/\[KEYWORD\]/$keyword/g;
		$response =~ s/\[NAMECAPS\]/$namecaps/g;
		$response =~ s/\[KEYWORDCAPS\]/$kwcaps/g;

		$logger->info('Final message: '.$response);

		#Applique t-on un modifier ? 
			#Modifiers possibles:
			#Envoi d'image (auquel cas pas d'envoi de message)
			#Envoi de sticker
			#Zalgo 
			#Reverse text

		 switch (int(rand(100))) {
			case [1..2] { $logger->info("zalgofying.."); $response = zalgo($response); &sendMessage($response,$chatid) }
			case [3..4] { $logger->info("turning text upside down."); $response = upside_down($response); &sendMessage($response,$chatid) }
			case [5..7] { $logger->info("imagesearch."); &sendImage("voiture",$chatid) }
			case [8..9] { $logger->info("imagesearch."); &sendImage("daft punk",$chatid) }
			case [10..29] { $logger->info("sending sticker."); &sendSticker(@stickers[int(rand(scalar @stickers))],$chatid) } #oh putain cette ligne est dégueu
			case [30..31] { $logger->info("zalgo and upside down!"); $response = zalgo($response); $response = upside_down($response); &sendMessage($response,$chatid) }
			else { &sendMessage($response,$chatid)}
			}
		
	}
}

#on print 200 dans tous les cas en imprimant le header, pour indiquer à telegram qu'on a bien reçu la requête.
print $qrequest->header;
#c'est terminé
$redis->quit();

#-------------------------------------------------
#-------------------------------------------------
#-------------------------------------------------
#fonctions annexes issi

#sendMessage(Message,chatID)
sub sendMessage{

	my $ua = LWP::UserAgent->new; 
	my $res;

	my $encodedmessage = encode("UTF-8",$_[0]);

	$res = $ua->post(
			'https://api.telegram.org/bot'.$accesstoken.'/sendMessage',
			[
			'chat_id' => $_[1],
			'text' => $encodedmessage,
			],
			'Content_Type' => 'form-data',
		);

	$logger->info('Request sent to Telegram! Status:'.$res->content());

}

#sendImage(BingKeyword,chatID)
sub sendImage{

	my $ua = LWP::UserAgent->new; 
	my $res;

	my $image = &getBingImage($_[0]);

		$res = $ua->post(
			'https://api.telegram.org/bot'.$accesstoken.'/sendPhoto',
			[
			'chat_id' => $_[1],
			'photo' => [$image],
			],
			'Content_Type' => 'form-data',
			);

	unlink $image;

	$logger->info('Request sent to Telegram! Status:'.$res->content());

}

#sendSticker(StickerID,chatID)
sub sendSticker{

	my $ua = LWP::UserAgent->new; 
	my $res;

		$res = $ua->post(
			'https://api.telegram.org/bot'.$accesstoken.'/sendSticker',
			[
			'chat_id' => $_[1],
			'sticker' => $_[0],
			],
			'Content_Type' => 'form-data',
			);

	$logger->info('Request sent to Telegram! Status:'.$res->content());

}

#addToRedis(Message,SetName)
sub addToRedis{
	if (length($_[0])>0)
	{
		$redis->sadd($_[1], encode_utf8($_[0]));
		return 1;
	}
	else
	{
		return 0;
	}
	
}

#getFromRedis(SetName)
sub getFromRedis{

	my $res = $redis->srandmember($_[0]);

	return $res;
}

#getRNGValue()
sub getRNGValue{

	return $redis->get("wrng");
}

#setRNGValue(newVal)
sub setRNGValue{

	$redis->set("wrng",$_[0]);
}

#getLongestWord(Message)
sub getLongestWord{

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

#getBingImage(keyword)
sub getBingImage{

	$logger->info('Searching Bing for '.$_[0]);

	my $keyword = $_[0];
	#We query the bing API for this.
	my $uri = qq(https://api.datamarket.azure.com/Bing/Search/v1/Composite?\$format=json&Sources=%27image%27&Query=%27$keyword%27&Options=%27DisableLocationDetection%27&Adult=%27Off%27);
	my $bingtoken = "nein";

	#Query Execution
	my $ua = LWP::UserAgent->new; 
	$ua->credentials("api.datamarket.azure.com".':443', '', '', $bingtoken);

	my $res = $ua->get($uri);

	#$res contains the JSON answer.
	my $jsonresponse = $res -> decoded_content;
	my $hash = parse_json($jsonresponse);
	my $images = $hash->{"d"}->{"results"}[0]->{"Image"};

	#size of the array
	my $img = scalar @$images;
	my $selector = int(rand($img));

	$logger->info('Request sent to Bing! ');
	$logger->info('Image array size: '.$img);

	my $imageurl = @$images[$selector]->{"MediaUrl"};
	#contentType = image/xxxx
	my $imagetype = @$images[$selector]->{"ContentType"};
	my $extension = substr $imagetype, 6;

	$logger->info('URL obtained from Bing: '.$imageurl);

	#we download the image
	$ua = LWP::UserAgent->new(agent=>' Mozilla/5.0 (Windows NT 6.1; WOW64; rv:24.0) Gecko/20100101 Firefox/24.0', cookie_jar=>{});
	my $resp = $ua->mirror($imageurl, '/tmp/img.'.$extension);

	$logger->info('Image saved to '.'/tmp/img.'.$extension);

	#we return the path to the downloaded image
	return '/tmp/img.'.$extension

}