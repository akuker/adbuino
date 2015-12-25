var https = require('https');
process.env['NODE_TLS_REJECT_UNAUTHORIZED'] = '0';
var apiKeyPB = "pushbulletAPIKey";
var apiUrl = "https://smsapi.free-mobile.fr/sendmsg?user=XXXX&pass=YYYYY&msg=";

var WebSocketClient = require('websocket').client;
 
var client = new WebSocketClient();
 
client.on('connectFailed', function(error) {
    console.log('Connect Error: ' + error.toString());
});
 
client.on('connect', function(connection) {
    console.log('WebSocket Client Connected');

    connection.on('error', function(error) {
        console.log("Connection Error: " + error.toString());
    });

    connection.on('close', function() {
        console.log('echo-protocol Connection Closed');
    });

    connection.on('message', function(message) {
        json = JSON.parse(message.utf8Data);

    if (json.type !="nop")
	    if (json.push.type == "mirror")
	        {
	         	console.log(json.push.title);
	         	console.log(json.push.body);

	         	if (json.push.package_name != "com.android.mms") //avoid crazy feedback loop
	         	https.get(apiUrl+json.push.title+" "+json.push.body, function(res) {
				    if(res != 200){
				    	// Envoi depuis le Sim900, mail, etc.
				    	console.log("sa a pa envoyé mdr");
				    }

				    res.resume();
				}).on('error', function(e) {
				    console.error(e);
				});

	        }
        
    });
});
    
client.connect('wss://stream.pushbullet.com/websocket/' + apiKeyPB, null);  