var userVar = 0;
var app = require('http').createServer(handler)
var io = require('socket.io')(app);
var fs = require('fs');

function handler (req, res) {
  fs.readFile(__dirname + '/index.html',
  function (err, data) {
    if (err) {
      res.writeHead(500);
      return res.end('jej');
    }
    res.writeHead(200);
    res.end(data);
  });
}

app.listen(8255);

io.on("connection", function(socket){
        /* ***************************
        EVENT FROM socket.io
        *****************************/
        //socket.on("connect", function(){
            console.log("ping : user connect");
                        userVar++;
                        console.log("users: "+userVar);
                        io.emit('users', userVar);
        //});
        socket.on("disconnect", function(){
            console.log("ping : user disconnect");
                        userVar--;
                        console.log("users: "+userVar);
                        io.emit('users', userVar);
        });
    });

console.log("Server Started.");

