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
        console.log('Connection Closed');
    });

    connection.on('message', function(message) {
        json = JSON.parse(message.utf8Data);
        //console.log(message.utf8Data);

    if (json.hasOwnProperty("push"))
	    if (json.push.type == "mirror")
	        {
	         
	         	if (json.push.package_name != "com.android.mms") //avoid crazy feedback loop
	         	{

              console.log(json.push.title);
              console.log(json.push.body);

		         	https.get(apiUrl+json.push.title+" "+encodeURIComponent(json.push.body), function(res) {
	
					    //console.log(res);

					    res.resume();

    					}).on('error', function(e) {
    					    console.error(e);
    					});

				    }
            else
            {
              console.log("sms received");
              //If it's a sms, we auto-dismiss it after 30 seconds. Avoids clutter on the phone while giving enough time to read.
              //This is the best workaround we can do, since the watch doesn't allow us to dismiss notifications. (hella gay)
              setTimeout(PostDismissal, 30000, json.push.notification_id,json.push.package_name,json.push.source_user_iden);

            }

	        }
        
    });
});
    
client.connect('wss://stream.pushbullet.com/websocket/' + apiKeyPB, null);  


function PostDismissal(notification_id,package_name,source_user_iden) {
  // Build the post string from an object
  var post_data = '{"push":{"notification_id":"'+notification_id+'","notification_tag":null,"package_name":"'+package_name+'","source_user_iden":"'+source_user_iden+'","type":"dismissal"},"type":"push"}';

  // An object of options to indicate where to post to
  var post_options = {
      host: 'api.pushbullet.com',
      port: '443',
      path: '/v2/ephemerals',
      method: 'POST',
      headers: {
          'Access-Token': apiKeyPB,
          'Content-Type': 'application/json'
      }
  };

  // Set up the request
  var post_req = https.request(post_options, function(res) {
      res.setEncoding('utf8');
      res.on('data', function (chunk) {
          console.log('Response: ' + chunk);
      });
  });

  // post the data
  post_req.write(post_data);
  post_req.end();

}