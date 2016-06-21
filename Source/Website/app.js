var PORT = 3000;

// Initialisierung des Express Servers
var express = require("express");

var bodyParser = require("body-parser");
var app = express();
app.use(bodyParser.urlencoded({extended: false}));
app.use(bodyParser.json());

// Der Server braucht eine Warteschlange
var inputQueue = [];
// nutze inputQueue.push(..) und inputQueue.shift() zum Ein-/Auslesen

app.use(express.static(__dirname + "/public"));
var server = app.listen(PORT);
// Initialisierung Websockets
var socketio = require("socket.io");
var io = socketio.listen(server);

var outputInterval;
var OUTPUTTIME = 20000;
var lines_drawn = [];

// for saving images
var fs = require("fs");
var mkdirp = require('mkdirp');


// Eventhandler für die Verbindung eines neuen Clients
io.sockets.on("connection", function(socket){
	
	console.log("device connected");
	// Wenn der Client connected, muss einmal die ContentQueue auf Anfrage geladen werden
	
	socket.emit("initializeContentQueue", JSON.stringify(inputQueue));
	

	for(var i in lines_drawn){
		socket.emit("draw_line", { line: lines_drawn[i] } );
	}

	socket.on("draw_line", function(data){
		lines_drawn.push(data.line);
		io.emit("draw_line", { line: data.line });
	});
});

app.post('/', function(request, response){
	//console.log("username: " + request.body.data);

	var img = request.body.data;

	// strip off the data: url prefix to get just the base64-encoded bytes
	var data = img.replace(/^data:image\/\w+;base64,/, "");
	var buf = new Buffer(data, 'base64');

	mkdirp('images', function(err){
		if(err) console.error("directory could not be created");
		else console.log("pow!");
	});

	fs.writeFile('images/image.png', buf);
});


var updateImage = function(){
	io.sockets.emit('imageUpdate');
};

setInterval(updateImage, 10000);
// TODO: Senden des aktuellen Textes an den Controller